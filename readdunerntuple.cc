#undef R__HAS_VDT
#include "GenerateDataInRntuple.h"
#include "DuneDataModel.h"

#define READVEC 
R__LOAD_LIBRARY(ROOTNTuple)

using RNTupleModel = ROOT::Experimental::RNTupleModel;
using RNTupleReader = ROOT::Experimental::RNTupleReader;
using RNTupleWriter = ROOT::Experimental::RNTupleWriter;

#include<iostream>

int main(int argc, char* argv[]){
 
  std::string fname = "dune_file.root";
  
  auto model = RNTupleModel::Create();

#ifdef READVEC  
  std::shared_ptr<std::vector<uint32_t>> fake_data = model->MakeField<std::vector<uint32_t>>("Channel_"+std::to_string(2500));
#endif
  
 std::shared_ptr<DuneDataModel::DuneRawDataHeader> header = model->MakeField<DuneDataModel::DuneRawDataHeader>("ChannelHeader_"+std::to_string(2500));

  
  auto ntuple = RNTupleReader::Open(std::move(model),"APA",fname.c_str());

  

#ifdef READVEC
  int count = 0;
  for(auto id: *ntuple){
    ntuple->LoadEntry(id);
  for(auto val: *fake_data){
    std::cout<<val<<std::endl;
     count++;
    if(count>10)break;
  }
  std::cout<<fake_data->size()<<std::endl;
  auto _val = header.get();
  std::cout<<_val->chan_<<" "<<_val->Nadc_<<std::endl;
  }
#endif  

 return 1; 
}