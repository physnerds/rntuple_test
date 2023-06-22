#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <vector>

template<typename U>
class DataModel{
  public:
  DataModel(){};
  ~DataModel(){};
  
  virtual std::vector<U>ReturnFakeData(std::size_t length)=0;
  
  private:
  
  
};

#endif