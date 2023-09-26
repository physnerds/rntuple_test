#include <iostream>
#include <array>
#include <vector>
/*
 Attributes of the soa....
 1. Pointers should be smart enough to access correct members


 Things to note:
 1. How are the pointer address accessed?
 2. How are the objects stored?
 3. What should be the layout of the object?
 */

//Rudimentary macros to create class...
//note that I should not make it over-complicated....

namespace soa {
template<typename T,size_t Size>
struct Array{
  std::array<T,Size>Data;
  Array(){
    Data.fill(T());
  }
  
};

template<typename T, size_t NumArrays, size_t ArraySize>
struct SoA{
  std::array<Array<T,ArraySize>,NumArrays>arrays;
  

};


}
