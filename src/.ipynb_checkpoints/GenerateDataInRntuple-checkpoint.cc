#include "GenerateDataInRntuple.h"
#include <ROOT/RNTuple.hxx>
#include <ROOT/RNTupleModel.hxx>
#include "DuneDataModel.h"
#undef R__HAS_VDT


using RNTupleModel = ROOT::Experimental::RNTupleModel;
using RNTupleReader = ROOT::Experimental::RNTupleReader;
using RNTupleWriter = ROOT::Experimental::RNTupleWriter;


GenerateDataInRntuple::GenerateDataInRntuple(){
  rmodel_ = RNTupleModel::Create();
}

GenerateDataInRntuple::~GenerateDataInRntuple(){}
/*
GenerateDataInRntuple::GenerateDataInRntuple(std::string const &fname,std::unique_ptr<RNTupleModel> rmodel){
  auto ntuple = RNTupleWriter::Recreate(std::move(rmodel),"APA01",fname.c_str());
  
}
*/


GenerateDataInRntuple::GenerateDataInRntuple(std::string const &fname):fname_(fname),rmodel_(RNTupleModel::Create()){
 // rmodel_ = RNTupleModel::Create();
  
}

void GenerateDataInRntuple::WriteDataImpl(std::vector<int>const &data){
}

void GenerateDataInRntuple::CreateHeader(int chan_){
  std::string header_name = "ChannelHeader_"+std::to_string(chan_);
  auto chan_header = rmodel_->MakeField<DuneDataModel::DuneRawDataHeader>(header_name.c_str());
 // *chan_header = header_info;
//  rntuple_->Fill();
  
}

void GenerateDataInRntuple::WriteHeader(int chan_no,DuneDataModel::DuneRawDataHeader header_info){
  std::string ch_str = "ChannelHeader_"+std::to_string(chan_no);
  std::string_view ch_view(ch_str.data());
  auto ch_header = rmodel_->Get<DuneDataModel::DuneRawDataHeader>(ch_view);
  *ch_header = header_info;
}
//template<typename T>
void GenerateDataInRntuple::CreateDataSet(int chan_no){
  auto dset_name = "Channel_"+std::to_string(chan_no);
  
  auto chan_data = rmodel_->MakeField<std::vector<uint32_t>>(dset_name.c_str());
  
}

void GenerateDataInRntuple::WriteDataSet(int chan_no, std::vector<uint32_t>data){
  std::string dset_name = "Channel_"+std::to_string(chan_no);
  std::string_view ch_view(dset_name.data());
  
  auto ch_data = rmodel_->Get<std::vector<uint32_t>>(ch_view);
  for(auto val:data)ch_data->emplace_back(val);
 // *ch_data = data;
  
}
std::unique_ptr<RNTupleWriter>GenerateDataInRntuple::ReturnRNtuple(){
  
 return std::move(rntuple_); 
}

 std::unique_ptr<RNTupleModel>GenerateDataInRntuple::GenerateRntupleHeader(std::string apa_name){
  
  std::unique_ptr<RNTupleModel> model = RNTupleModel::Create();
  return model;
}


void GenerateDataInRntuple::AddDataSet(std::unique_ptr<RNTupleModel>rmodel,int chan_no){
  auto chan_dat = rmodel->MakeField<std::vector<float>>("Channel_"+std::to_string(chan_no));
  
}

template<typename T>
void GenerateDataInRntuple::WriteRntuple(std::shared_ptr<std::vector<T>>dat_chan,std::vector<T>data){
 // auto ntuple = 
  dat_chan->clear();
  for(auto val:data)
    dat_chan->emplace_back(val);
  
}

void GenerateDataInRntuple::PrintRNTupleInfo(){
// rntuple_->PrintInfo(); //needs to be a reader..
  
}

void GenerateDataInRntuple::FillRntuple(){
  rntuple_ = RNTupleWriter::Recreate(std::move(rmodel_),"APA",fname_.c_str());
  rntuple_->Fill();
  
}

