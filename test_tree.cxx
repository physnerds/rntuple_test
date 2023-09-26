//RNTUPLE STUFF...

#include "TInterpreter.h"
#include <TROOT.h>
#include <TTree.h>
#include <TFile.h>
#include <iostream>
#include <boost/preprocessor.hpp>
#include <vector>
#include <cstring>
#include <thread>

#include "DuneSoADataModel.h"

int readTree(){
 //DuneSoADataModel::soa_arrays 
  constexpr char* const _fname = "myfile_tree.root";
  DuneSoADataModel::soa_arrays* Soa;
  
  auto _rfile = new TFile(_fname,"READONLY");
  
  Soa = (DuneSoADataModel::soa_arrays*)_rfile->Get("mySoa");
  
  for(int i = 0;i<10;i++)
    std::cout<<Soa->xx[i]<<" "<<Soa->yy[i]<<" "<<Soa->zz[i]<<std::endl; 
  
  delete Soa;
  return 1;
}

int createTree(){

    DuneSoADataModel::soa_arrays Soa;
  
  for(int i=0;i<10000;i++){
    
    Soa.xx[i] = i;
    Soa.yy[i] = (i)*3.1415;
    Soa.zz[i] = i+8;
  }
  constexpr char* const _fname = "myfile_tree.root";
  
  for(int i = 0;i<10;i++)
    std::cout<<Soa.xx[i]<<" "<<Soa.yy[i]<<" "<<Soa.zz[i]<<std::endl;
  
  TFile* _rfile = new TFile(_fname,"RECREATE");
  _rfile->WriteObjectAny(&Soa,"DuneSoADataModel::soa_arrays","mySoa");
  
  delete _rfile;
  return 1;
}

int main(){
 // createTree();
  readTree();
  return EXIT_SUCCESS;
}
 

