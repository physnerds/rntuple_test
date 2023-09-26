//RNTUPLE STUFF...
#undef R__HAS_VDT
    
#include <ROOT/RNTuple.hxx>
#include <ROOT/RNTupleModel.hxx>
#include "TInterpreter.h"
#include <TROOT.h>
#include <TRandom.h>
#include <TRandom3.h>

R__LOAD_LIBRARY(ROOTNTuple)
using REntry = ROOT::Experimental::REntry;
using RNTupleModel = ROOT::Experimental::RNTupleModel;
using RNTupleReader = ROOT::Experimental::RNTupleReader;
using RNTupleWriter = ROOT::Experimental::RNTupleWriter;

#include <iostream>
#include <boost/preprocessor.hpp>
#include <vector>
#include <cstring>
#include <thread>

#include "DuneSoADataModel.h"


constexpr char* const _fname = "myfile_rntuple.root";
const int NWriterThreads = 4;
const int EvtsPerThreads = 100000;
int readRNTuple(){
  
  auto ntuple = RNTupleReader::Open("NTuple",_fname);
  
  //soa -->RNTupleView<DuneSoADataModel::soa_arrays>
  auto soa = ntuple->GetView<DuneSoADataModel::soa_arrays>("SoA");
  
  std::cout<<"Entry "<<soa(0).xx[1]<<" "<<soa(0).yy[1]<<" "<<soa(0)._size<<std::endl;
  return 1;
  
}

int createRNTuple(){

  //  DuneSoADataModel::soa_arrays Soa;
/*  
  for(int i=0;i<100000;i++){
    
    Soa.xx[i] = i;
    Soa.yy[i] = (i)*3.1415;
    Soa.zz[i] = i+8;
  }
*/
  
 // for(int i = 0;i<10;i++)
 //   std::cout<<Soa.xx[i]<<" "<<Soa.yy[i]<<" "<<Soa.zz[i]<<std::endl;
  
  
  auto model = RNTupleModel::Create(); 
  std::shared_ptr<DuneSoADataModel::soa_arrays> fSoA = model->MakeField<DuneSoADataModel::soa_arrays>("SoA");
  
 // auto _val = model->Get<DuneSoADataModel::soa_arrays>("SoA");
  auto ntuple = RNTupleWriter::Recreate(std::move(model),"NTuple",_fname);
  
  
  std::vector<std::unique_ptr<REntry>>entries;
  std::vector<std::thread>threads;
 
  for(int i=0;i<NWriterThreads;i++)
    entries.emplace_back(ntuple->CreateEntry());
  
  for(int i = 0;i<NWriterThreads;i++){
    threads.emplace_back([i,&entries,&ntuple](){
      static std::mutex gLock;
      
      static std::atomic<std::uint32_t>gThreadId;
      const auto threadID = ++gThreadId;
      
      auto prng = std::make_unique<TRandom3>();
      prng->SetSeed();
      DuneSoADataModel::soa_arrays Soa;
      auto _val = entries[i]->Get<DuneSoADataModel::soa_arrays>("SoA");
      for(int j=0;j<5000;j++){
        double xx,yy;
        prng->Rannor(xx,yy);
        Soa.xx[j] = int(xx*10);
        Soa.yy[j] = yy;
        Soa.zz[j] = j;
      }
      Soa._size = 5000;
      *_val = Soa;
      std::cout<<"_val "<<_val->xx[5]<<" "<<_val->yy[20]<<" "<<_val->zz[312]<<std::endl;
      std::lock_guard<std::mutex>guard(gLock);
      ntuple->Fill(*entries[i]);
      
    });
  }
  
  for(int i = 0;i<NWriterThreads;++i)
    threads[i].join();
  
  
  return 1;
}

int main(){
 // createRNTuple();
  readRNTuple();
  return EXIT_SUCCESS;
}

