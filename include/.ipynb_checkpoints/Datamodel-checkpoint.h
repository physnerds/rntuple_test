#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <vector>

template<typename U>
class DataModel{
  public:
  DataModel(){};
  ~DataModel(){};
  
  virtual std::vector<U>ReturnFakeData(int seed)=0;
  
  private:
  
  
};

#endif