#ifndef DUNEDATAMODEL_H
#define DUNEDATAMODEL_H

#include "DataModel.h"


class DuneDataModel: DataModel<uint32_t>{
  public:
  DUNEDataModel();
  ~DUNEDataModel();
  
  struct DuneRawDataHeader{
    int chan_;
    float pedestal_;
    float sigma_;
    int Nadc_;
    int compression_;
    
    DuneRawDataHeader(int chan, float pedestal,float sigma, int Nadc, int compression):    chan_(chan),pedestal_(pedestal),sigma_(sigma),Nadc_(Nadc),compression_(compression){};
    };
  std::vector<uint32_t>ReturnFakeData(int seed) override;
  
  private:
  
}

#endif