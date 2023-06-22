#undef R__HAS_VDT

#include "GenerateDataInRntuple.h"
#include "DuneDataModel.h"
#include "SoACommon.h"

#include "DuneSoADataModel.h"
R__LOAD_LIBRARY(ROOTNTuple)
  
using RNTupleModel = ROOT::Experimental::RNTupleModel;
using RNTupleReader = ROOT::Experimental::RNTupleReader;
using RNTupleWriter = ROOT::Experimental::RNTupleWriter;


int main(int argc, char* argv[]){

  //The DUNE DATA....
    int channel_id = 2500;
    int chan_ = 2500;
    float pedestal_ = 2305;
    float sigma_ = 8.32;
    int Nadc_ = 600000;
    int compression_ = 0;
    
    auto dm = DuneDataModel();
    std::vector<uint32_t> fake_data = dm.ReturnFakeData(Nadc_);
    std::cout<<"Size of Fake Data "<<fake_data.size()<<std::endl;
    auto header_info = DuneDataModel::DuneRawDataHeader(channel_id, pedestal_,sigma_,Nadc_,compression_);
  std::string fname = "dune_file.root";

  auto rdune = GenerateDataInRntuple(fname); 
  rdune.CreateHeader(chan_);
  rdune.WriteHeader(chan_, header_info);
  rdune.CreateDataSet(chan_);
  rdune.WriteDataSet(chan_,fake_data);

  rdune.FillRntuple();
  return 1;
}