#ifndef DUNEDATAMODEL_H
#define DUNEDATAMODEL_H

#include "DataModel.h"
#include <cstdint>
#include <vector>
class DuneDataModel: DataModel<uint32_t>{
  public:
  DuneDataModel();
  ~DuneDataModel();
 
  
  struct alignas(128) DuneRawDataHeader {
    int chan_;
    float pedestal_;
    float sigma_;
    int Nadc_;
    int compression_;
    
    DuneRawDataHeader(int chan, float pedestal,float sigma, int Nadc, int compression):    chan_(chan),pedestal_(pedestal),sigma_(sigma),Nadc_(Nadc),compression_(compression){};
    DuneRawDataHeader(){};
    };
   
  
  std::vector<uint32_t>ReturnFakeData(std::size_t length) override;
  
  private:
  
};

#endif