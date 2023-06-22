#ifndef GENERATEDATA_H
#define GENERATEDATA_H

#include <vector>
#include <string>
#include <iostream>
#include <tuple>
#include "HDFCxx.h"

//template<typename T>
class GenerateData{
  public:
   GenerateData(){};
   GenerateData(std::string const &fname, int &chunksize);
   GenerateData(GenerateData&&);
   GenerateData(GenerateData const&);
   virtual void WriteDataImpl( std::vector<int>const &data)=0;
  // virtual std::vector<int>ReturnFakeData(size_t dsize)=0;
   
    ~GenerateData(){};
   //common to all derived classes...
  private:
  //  std::tuple<Ts...>args_;
    
};

#endif