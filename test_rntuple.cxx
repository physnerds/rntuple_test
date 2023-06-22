#undef R__HAS_VDT
    
#include <ROOT/RNTuple.hxx>
#include <ROOT/RNTupleModel.hxx>
#include <TRandom.h>
#include "DuneSoADataModel.h"
#include "SoACommon.h"
#include "SoAView.h"

#include "TInterpreter.h"
#include <TROOT.h>

R__LOAD_LIBRARY(ROOTNTuple)
using RNTupleModel = ROOT::Experimental::RNTupleModel;
using RNTupleReader = ROOT::Experimental::RNTupleReader;
using RNTupleWriter = ROOT::Experimental::RNTupleWriter;

#include <iostream>
void WriteStuff(){

  constexpr char const* filename = "mytest.root";   
  constexpr int kNEvents = 25000;
    
  //creating a data model...
  auto model = RNTupleModel::Create();
  
  //std::shared_ptr<std::vector<float> px = model->MakeField<std::vector<float>>("px");
  
  auto px = model->MakeField<std::vector<float>>("px");
  auto py = model->MakeField<std::vector<float>>("py");
  auto pz = model->MakeField<std::vector<float>>("pz");
  
  //ntuple is a unique_ptr
  auto ntuple = RNTupleWriter::Recreate(std::move(model),"rdir",filename);
  
  gRandom->SetSeed();
  
  for(int i =0; i<kNEvents;i++){
    int npx = static_cast<int>(gRandom->Rndm(1)*15);
    px->clear();
    py->clear();
    pz->clear();
    
    for(int j = 0;j<npx;j++){
      float _px,_py,_pz;
      gRandom->Rannor(_px,_py);
      
      _pz = sqrt(_px*_px+_py*_py);
      
      auto random = gRandom->Rndm(1);
      px->emplace_back(_px);
      py->emplace_back(_py);
      pz->emplace_back(_pz);      
    }
    ntuple->Fill();
    px->emplace_back(0.004);
    ntuple->Fill();
  }
  
}

void ReadStuff(){
 // Get a unique pointer to an empty RNTuple model
   auto model = RNTupleModel::Create();
  constexpr char const* filename = "mytest.root";   
   // We only define the fields that are needed for reading
   auto fldVpx = model->MakeField<std::vector<float>>("px");
 
   // Create an ntuple without imposing a specific data model.  We could generate the data model from the ntuple
   // but here we prefer the view because we only want to access a single field
   auto ntuple = RNTupleReader::Open(std::move(model), "rdir", filename);
 
   // Quick overview of the ntuple's key meta-data
   ntuple->PrintInfo();
 
   std::cout << "Entry number 42 in JSON format:" << std::endl;
   ntuple->Show(41);
   // In a future version of RNTuple, there will be support for ntuple->Scan()
 
 
   // Iterate through all the events using i as event number and as an index for accessing the view
   for (auto entryId : *ntuple) {
      ntuple->LoadEntry(entryId);
 
      for (auto px : *fldVpx) {
         std::cout<<px<<std::endl;
      }
 
   }
 
   // Prevent the histogram from disappearing

}
using SoA = DuneSoADataModel::SoATemplate<>;
int main(int argc, char* argv[]){
  
  const char* header_view = " #include </global/u2/a/abashyal/rntuple_test/include/SoAView.h>";
  gInterpreter->ProcessLine(header_view);
  const char* header_layout= "#include </global/u2/a/abashyal/rntuple_test/include/SoALayout.h>";
  gInterpreter->ProcessLine(header_layout);
  const char* header_common = "#include </global/u2/a/abashyal/rntuple_test/include/SoACommon.h>";
  gInterpreter->ProcessLine(header_common);
  
  const unsigned int numelements = 65537;
  size_t h_size = SoA::computeDataSize(numelements);
  std::byte* d_buf = nullptr;
  d_buf = (std::byte*)malloc(h_size); 
  SoA(d_buf,64);
  
  constexpr char* const _fname = "myfile.root";
  
  auto model = RNTupleModel::Create();
  std::shared_ptr<SoA>fSoA = model->MakeField<SoA>("SoA");

  auto ntuple = RNTupleWriter::Recreate(std::move(model),"Test",_fname);
  
  //WriteStuff();
  //ReadStuff();
  return 1;
  
}