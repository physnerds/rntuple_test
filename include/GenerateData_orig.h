#ifndef GENERATEDATA_H
#define GENERATEDATA_H

#include <vector>
#include <string>
#include <fstream>
#include "HDFCxx.h"

//template<typename T>
template<typename Ts>
class GenerateData{
  public:
    GenerateData();
   ~GenerateData();
    
   virtual void CreateHeader(hid_t g,int channel_id,DuneRawDataHeader header_info)=0;
   virtual void WriteData(hid_t gid,int ch_id, std::vector<int>const &data)=0;
   virtual void CreateChannelGroup()=0;
    virtual std::vector<int>ReturnFakeData(size_t dsize)=0;

    T GetFileObject();  
    
};

#endif