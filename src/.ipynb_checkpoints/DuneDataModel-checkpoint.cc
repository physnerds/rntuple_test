#include "DuneDataModel.h"
#include <iostream>
#include <cstring>
#include <time.h>
#include <vector>
#include <random>

DuneDataModel::DuneDataModel(){}

DuneDataModel::~DuneDataModel(){}


std::vector<uint32_t> DuneDataModel::ReturnFakeData(std::size_t length){
  std::vector<uint32_t> fake_data;
  fake_data.reserve(length);
  std::random_device dev;
  
  std::mt19937 rng(dev());
  
  std::uniform_int_distribution<std::mt19937::result_type> some_rand(2000,3000);
  
  for(int i =0;i<length;i++)fake_data.push_back(some_rand(rng));
  
  return fake_data;
  
}
