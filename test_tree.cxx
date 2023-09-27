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
  TTree *fTree = _rfile->Get<TTree>("myTree");
  
  Soa = new DuneSoADataModel::soa_arrays;
  fTree->SetBranchAddress("Soa",&Soa);
  const int entries = fTree->GetEntries();
  std::cout<<"Total Entries "<<entries<<std::endl;

  fTree->GetEntry(0);
  
  for(int i = 0;i<10;i++)
    std::cout<<Soa->xx[i]<<" "<<Soa->yy[i]<<" "<<Soa->zz[i]<<std::endl; 
  
  delete Soa;
  
  return 1;
}

int createTree(){
   TFile *fOutFile;
  TTree* fOutTree;
  DuneSoADataModel::soa_arrays* Soa;
  Soa = new DuneSoADataModel::soa_arrays;
  constexpr char* const _fname = "myfile_tree.root";
  
  fOutFile = new TFile(_fname,"RECREATE");
  fOutTree = new TTree("myTree","Test of SoA in Tree");
  fOutTree->Branch("Soa","DuneSoADataModel::soa_arrays",&Soa, 32000,99);
  

  
  for(int i=0;i<10000;i++){
    
    Soa->xx[i] = i;
    Soa->yy[i] = (i)*3.1415;
    Soa->zz[i] = i+8;
  }
    Soa->_size = 10000;

  
  for(int i = 0;i<10;i++)
    std::cout<<Soa->xx[i]<<" "<<Soa->yy[i]<<" "<<Soa->zz[i]<<std::endl;
  
 // fOutFile->WriteObjectAny(Soa,"DuneSoADataModel::soa_arrays","mySoa");
  fOutTree->Fill();
  
  fOutTree->Write();
  fOutFile->Close();
  delete Soa;
  delete fOutFile;
  return 1;
}

int main(){
// createTree();
  readTree();
  return EXIT_SUCCESS;
}
 

