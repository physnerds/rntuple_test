#ifndef GENERATEDATAINRNTUPLE_H
#define GENERATEDATAINRNTUPLE_H

//for some reason this undef statement is needed...


#include <vector>
#include <string>
#include <fstream>
#include "GenerateData.h"
#include "DuneDataModel.h"

#include <ROOT/RNTuple.hxx>
#include <ROOT/RNTupleModel.hxx>

R__LOAD_LIBRARY(ROOTNTuple);

using RNTupleModel = ROOT::Experimental::RNTupleModel;
using RNTupleReader = ROOT::Experimental::RNTupleReader;
using RNTupleWriter = ROOT::Experimental::RNTupleWriter;

class GenerateDataInRntuple: public GenerateData{
public:
//  GenerateDataInRntuple(std::string const &fname,std::unique_ptr<RNTupleModel>rmodel);
  GenerateDataInRntuple(std::string const &fname);
  GenerateDataInRntuple(GenerateDataInRntuple&&);
  GenerateDataInRntuple(GenerateDataInRntuple const&);
  GenerateDataInRntuple();
  ~GenerateDataInRntuple();
  
  void WriteDataImpl(std::vector<int>const &data)override;
  
  void CreateHeader(int chan_no);
  void WriteHeader(int chan_no,DuneDataModel::DuneRawDataHeader header_info);
 // template<typename T>
  void CreateDataSet(int chan_no);
  void WriteDataSet(int chan_no,std::vector<uint32_t>data);
  
  std::unique_ptr<RNTupleModel>GenerateRntupleHeader(std::string apa_name);
  void AddDataSet(std::unique_ptr<RNTupleModel>rmodel, int chan_no);
  
  template<typename T>
  void WriteRntuple(std::shared_ptr<std::vector<T>>dat_chan,std::vector<T>data);
  
  void FillRntuple();
  void PrintRNTupleInfo();
  std::unique_ptr<RNTupleWriter> ReturnRNtuple();
  RNTupleModel* GetModel();
  
  private:
    const int nThreads_ = 64;
    const std::string fname_;
    std::unique_ptr<RNTupleWriter>rntuple_;
    std::unique_ptr<RNTupleModel> rmodel_;
    
    

    

};

#endif
