#ifndef GENERATEDATAINHDF5_H
#define GENERATEDATAINHDF5_H

#include <vector>
#include <string>
#include <fstream>
#include "HDFCxx.h"
#include "hdf5.h"
#include "GenerateData.h"
#include "DuneDataModel.h"

class GenerateDataInHDF5: public GenerateData{
  public:
    GenerateDataInHDF5(std::string const &fname, int &chunksize);
    GenerateDataInHDF5(GenerateDataInHDF5&&);
    GenerateDataInHDF5(GenerateDataInHDF5 const&);
    GenerateDataInHDF5();
    ~GenerateDataInHDF5();

    void WriteDataImpl( std::vector<int>const &data) override;
    void WriteData(hid_t gid, DuneDataModel::DuneRawDataHeader header_info, std::vector<uint32_t>const &data);
    //std::vector<int>ReturnFakeData(size_t dsize) override;
    //local implementation of the functions in the class...
    
    void CreateHeader(hid_t g,DuneDataModel::DuneRawDataHeader header_info);  
    //void CreateChannelGroup();
    hdf5::File GetFileObject();
 
 private:
      hdf5::File file_;
      int chunksize_;
      
    
};

#endif