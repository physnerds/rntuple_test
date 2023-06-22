#include <Eigen/Core>
#include "SoALayout.h"
#include "SoAView.h"

#include<Eigen/Dense>
#include "Memory.cuh"

//#include "CUDACore/cudaCheck.h"


  GENERATE_SOA_LAYOUT_AND_VIEW(SoAHostDeviceLayoutTemplate,
                               SoAHostDeviceViewTemplate,
                               
  SOA_COLUMN(double, x),
  SOA_COLUMN(double, y),
  SOA_COLUMN(double, z),
  SOA_EIGEN_COLUMN(Eigen::Vector3d,a),
  SOA_EIGEN_COLUMN(Eigen::Vector3d,b),
  SOA_EIGEN_COLUMN(Eigen::Vector3d,r),
  SOA_SCALAR(const char*, description),
  SOA_SCALAR(uint32_t, someNumber)
  );

using SoAHostDeviceLayout = SoAHostDeviceLayoutTemplate<>;

using SoAHostDeviceView = SoAHostDeviceViewTemplate<cms::soa::CacheLineSize::NvidiaGPU, cms::soa::AlignmentEnforcement::Enforced>;


GENERATE_SOA_LAYOUT_AND_VIEW(SoADeviceOnlyLayoutTemplate,
                             SoADeviceOnlyViewTemplate,
                             SOA_COLUMN(uint16_t, color),
                             SOA_COLUMN(double, value),
                             SOA_COLUMN(double*, py),
                             SOA_COLUMN(uint32_t, count),
                             SOA_COLUMN(uint32_t, anotherCount))

using SoADeviceOnlyLayout = SoADeviceOnlyLayoutTemplate<>;
using SoADeviceOnlyView =
    SoADeviceOnlyViewTemplate<cms::soa::CacheLineSize::NvidiaGPU, cms::soa::AlignmentEnforcement::Enforced>;


// A 1 to 1 view of the store (except for unsupported types).
GENERATE_SOA_VIEW(SoAFullDeviceViewTemplate,
            SOA_VIEW_LAYOUT_LIST(SOA_VIEW_LAYOUT(SoAHostDeviceLayout, soaHD),
           SOA_VIEW_LAYOUT(SoADeviceOnlyLayout, soaDO)),
           SOA_VIEW_LAYOUT_LIST(SOA_VIEW_VALUE(soaHD, x),
           SOA_VIEW_VALUE(soaHD, y),
           SOA_VIEW_VALUE(soaHD, z),
           SOA_VIEW_VALUE(soaDO, color),
           SOA_VIEW_VALUE(soaDO, value),
           SOA_VIEW_VALUE(soaDO, py),
           SOA_VIEW_VALUE(soaDO, count),
           SOA_VIEW_VALUE(soaDO, anotherCount),
           SOA_VIEW_VALUE(soaHD, description),
           SOA_VIEW_VALUE(soaHD, someNumber)))

using SoAFullDeviceView =
    SoAFullDeviceViewTemplate<cms::soa::CacheLineSize::NvidiaGPU, cms::soa::AlignmentEnforcement::Enforced>;

__global__ void crossProduct(SoAHostDeviceView soa, const unsigned int numElements){
  int i = blockIdx.x*blockDim.x+threadIdx.x;
  if(i>=numElements);
  auto si = soa[i];
  si.r() = si.a().cross(si.b());

}

// Device-only producer kernel
__global__ void producerKernel(SoAFullDeviceView soa, const unsigned int numElements) {
  int i = blockIdx.x * blockDim.x + threadIdx.x;
  if (i>=numElements) return;
  auto si = soa[i];
  si.color() &= 0x55 << i % (sizeof(si.color()) - sizeof(char));
  si.value() = sqrt(si.x() * si.x() + si.y() * si.y() + si.z() * si.z());
}

// Device-only consumer with result in host-device area
__global__ void consumerKernel(SoAFullDeviceView soa, const unsigned int numElements) {
  int i = blockIdx.x * blockDim.x + threadIdx.x;
  if (i>=numElements) return;
  auto si = soa[i];
  si.x() = si.color() * si.value();
}



int main(int argc, char* argv[]){
  
  cudaStream_t stream;
  CUDA_EXP_CHECK(cudaStreamCreateWithFlags(&stream,cudaStreamNonBlocking));
                 
  constexpr unsigned int numelements = 65537;
  
  size_t hostDeviceSize = SoAHostDeviceLayout::computeDataSize(numelements);
  
  std::cout<<"Size in Host "<<hostDeviceSize<<std::endl;
  
  //allocate buffers/ stores etc in host
  std::byte* h_buf = nullptr;
  
  CUDA_EXP_CHECK(cudaMallocHost(&h_buf, hostDeviceSize));
  SoAHostDeviceLayout h_soahdLayout(h_buf, numelements);
  SoAHostDeviceView h_soahd(h_soahdLayout);
  
  //allocate buffers, stores and views on device
  size_t deviceOnlySize = SoADeviceOnlyLayout::computeDataSize(numelements);
  std::cout<<"Size in Device "<<deviceOnlySize<<std::endl; 
  std::byte* d_buf = nullptr;
  CUDA_EXP_CHECK(cudaMallocHost(&d_buf,hostDeviceSize+deviceOnlySize));
    
  //create device layout and view..
  SoAHostDeviceLayout d_soahdLayout(d_buf,numelements);
  
  SoADeviceOnlyLayout d_soadoLayout(d_soahdLayout.soaMetadata().nextByte(),numelements);
  
  SoAHostDeviceView d_soahdView(d_soahdLayout);
  SoAFullDeviceView d_soaFullView(d_soahdLayout, d_soadoLayout);
 
    // Assert column alignments...do these for all SoA objects (view and layout)
  assert(0 == reinterpret_cast<uintptr_t>(h_soahd.soaMetadata().addressOf_x()) % decltype(h_soahd)::byteAlignment);
  
  std::memset(h_soahdLayout.soaMetadata().data(),0,hostDeviceSize);
  
  for(size_t i = 0;i<numelements;++i){
    auto si = h_soahd[i];
    si.x() = si.a()(0) = si.b()(2) = 1.0*i+1.0;
    si.y() = si.b()(1) = si.b()(1) = 2.0*i;
    si.z = si.a()(2) = si.b()(0) = 3.0*i-1.0;
  }
  auto& sn = h_soahd.someNumber();
  sn = numelements+2;
  
  CUDA_EXP_CHECK(cudaMemcpyAsync(d_buf, h_buf,hostDeviceSize,cudaMemcpyDefault,stream));
  
  crossProduct<<<(numelements+255)/256,256,0,stream>>>(d_soahdView,numelements);
  
  CUDA_EXP_CHECK(cudaMemsetAsync(d_soadoLayout.soaMetadata().data(),0xFF,d_soadoLayout.soaMetadata().byteSize(),stream));
  
  producerKernel<<<(numelements+255)/256,256,0,stream>>>(d_soaFullView,numelements);
  
  consumerKernel<<<(numelements+255)/256,256,0,stream>>>(d_soaFullView,numelements);
  
  CUDA_EXP_CHECK(cudaMemcpyAsync(h_buf,d_buf,hostDeviceSize,cudaMemcpyDefault,stream));
  
 //uses the memory address to move data around.  
 CUDA_EXP_CHECK(cudaStreamSynchronize(stream));
  
 for(size_t i =0;i<numelements;++i){
   auto si = h_soahd[i];
   assert(si.r()==si.a().cross(si.b()));
  // std::cout<<si.r()<<" "<<si.a().cross(si.b())<<std::endl;
 }
  return 1;
}