#include "macros.h"
#include "val.h"
#include "Memory.cuh"
#include <iostream>

#define NTOT 1028*5

using namespace soa;


Generate_Arrays(soa_arrays,
                AddArray(int, xx, NTOT);
                AddArray(double, yy, NTOT);
                AddArray(uint32_t, zz, NTOT);
                );

Generate_Arrays(soa_simple,
                AddArray(myval<int>, xx,NTOT);
                AddArray(myval<double>,yy,NTOT);
                AddArray(myval<uint32_t>,zz,NTOT);
                );

//template wont work here...
__global__ void testVector(float a,soa_simple* C, soa_simple* A, soa_simple* B, int N=NTOT){
  int id = blockIdx.x*blockDim.x+threadIdx.x;
  if(id<N){
  B->xx[id] = A->xx[id].val()*a+ C->xx[id].val();
  B->yy[id] = A->yy[id].val()*12.3+C->yy[id].val();
  B->zz[id] = A->zz[id].val()*2+C->zz[id].val();
  }
}


template<typename T>
__global__ void testvector(float a, T* C,T* A, T* B, int N=NTOT){
  int id = blockIdx.x*blockDim.x+threadIdx.x;
  if(id<N){
  B->xx[id] = A->xx[id]*a+ C->xx[id];
  B->yy[id] = A->yy[id]*12.3+C->yy[id];
  B->zz[id] = A->zz[id]*2+C->zz[id];
  }
}


int test_soa(){
  int numThreads = NTOT;
  dim3 threadsPerBlock(1024,1,1);
  dim3 numberofBlocks((numThreads + threadsPerBlock.x-1)/threadsPerBlock.x,1,1);
    
  const int nentries = NTOT;
  soa_arrays Soa, Soaa;
  for(int i=0;i<nentries;i++){
    Soa.xx[i] = i;
    Soa.yy[i] = (i)*3.1415;
    Soa.zz[i] = i+8;
  }

  for(int i=0;i<nentries;i++){
    Soaa.xx[i] = i;
    Soaa.yy[i] = (i)*4.33;
    Soaa.zz[i] = i+81;
  }
  //create soa_simple objects for host and device..
  soa_arrays *d_Soa,*d_Soaa, *d_o_Soa, h_o_Soa;
  
  //allocate the memory....
  cudaMalloc((void**)&d_Soa,sizeof(soa_arrays));
  cudaMalloc((void**)&d_Soaa,sizeof(soa_arrays));
  
  cudaMalloc((void**)&d_o_Soa,sizeof(soa_arrays));


  cudaEvent_t start,stop;
  cudaEventCreate(&start);
  cudaEventCreate(&stop);
  
  cudaMemcpyAsync(d_Soa, &Soa, sizeof(soa_arrays),cudaMemcpyHostToDevice);
  cudaMemcpyAsync(d_Soaa, &Soaa, sizeof(soa_arrays),cudaMemcpyHostToDevice);
 
  float a = 4.3f;
  cudaEventRecord(start);
  testvector<<<threadsPerBlock,numberofBlocks>>>(a,d_Soaa,d_Soa, d_o_Soa);
  cudaEventRecord(stop);  cudaMemcpyAsync(&h_o_Soa,d_o_Soa,sizeof(soa_arrays),cudaMemcpyDeviceToHost);

  cudaEventSynchronize(stop);
  float milliseconds = 0;
  cudaEventElapsedTime(&milliseconds, start, stop);
//  for(int i=0;i<nentries;i++)std::cout<<h_o_Soa.xx[i]<<" "<<h_o_Soa.yy[i]<<" "<<h_o_Soa.zz[i]<<std::endl;
  auto var_size = sizeof(int)+sizeof(double)+sizeof(uint32_t);
  printf("Effective Bandwidth soa_arrays (GB/s): %f %f %d\n", NTOT*var_size*3/milliseconds/1e6, milliseconds,var_size);
  cudaFree(d_o_Soa);
  cudaFree(d_Soa);
  return 1;
}

int test_soa_simple(){ 
  int numThreads = NTOT;
  dim3 threadsPerBlock(1024,1,1);
  dim3 numberofBlocks((numThreads + threadsPerBlock.x-1)/threadsPerBlock.x,1,1);
    
  const int nentries = NTOT;
  soa_simple sa, saa;
  for(int i=0;i<nentries;i++){
    sa.xx[i] = i;
    sa.yy[i] = (i)*3.1415;
    sa.zz[i] = i+5;
  }
  
  for(int i=0;i<nentries;i++){
    saa.xx[i] = i;
    saa.yy[i] = (i)*4.33;
    saa.zz[i] = i+81;
  }

  //create soa_simple objects for host and device..
  soa_simple *d_Soa,*d_Soaa,*d_o_Soa, h_o_Soa;
  
  cudaEvent_t start,stop;
  cudaEventCreate(&start);
  cudaEventCreate(&stop);
  
  //allocate the memory....
  cudaMalloc((void**)&d_Soa,sizeof(soa_simple));
  cudaMalloc((void**)&d_Soaa,sizeof(soa_simple));
  
  cudaMalloc((void**)&d_o_Soa,sizeof(soa_simple));

  //now copy Soa content to d_Soa
  cudaMemcpyAsync(d_Soa, &sa, sizeof(soa_simple),cudaMemcpyHostToDevice);

  cudaMemcpyAsync(d_Soaa, &saa, sizeof(soa_simple),cudaMemcpyHostToDevice);
  
  float a = 4.3f;
  cudaEventRecord(start);
  testVector<<<threadsPerBlock,numberofBlocks>>>(a,d_Soaa,d_Soa, d_o_Soa);
  cudaEventRecord(stop);  
  cudaEventSynchronize(stop);  cudaMemcpyAsync(&h_o_Soa,d_o_Soa,sizeof(soa_simple),cudaMemcpyDeviceToHost);
  cudaEventSynchronize(stop);
  float milliseconds = 0;
  cudaEventElapsedTime(&milliseconds, start, stop);  
 // for(int i=0;i<nentries;i++)std::cout<<i<<" "<<h_o_Soa.xx[i]<<" "<<h_o_Soa.yy[i]<<" "<<h_o_Soa.zz[i]<<std::endl;

  printf("Effective Bandwidth soa_simple (GB/s): %f %f %d\n", NTOT*128*3/milliseconds/1e6, milliseconds,128);  
  cudaFree(d_o_Soa);
  cudaFree(d_Soa);
  
  return 1;
}
int main(){
  
  test_soa_simple(); 
  test_soa();
  //1 means code exited with error code=1 (i.e. True)
  return EXIT_SUCCESS;
}