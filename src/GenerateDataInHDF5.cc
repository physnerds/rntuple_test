#include "HDFCxx.h"
#include "hdf5.h"
#include "GenerateDataInHDF5.h"
#include "DuneDataModel.h"
#include <iostream>
#include <cstring>
#include <cmath>
#include <set>
#include <time.h>
#include <random>

//start with the constructors I guess..
//template<typename U>
GenerateDataInHDF5/*<U>*/::GenerateDataInHDF5(std::string const& fname, int &chunksize):
      file_(hdf5::File::create(fname.c_str())),chunksize_(chunksize){
    
    
}


//destructor
//template<typename U>
GenerateDataInHDF5/*<U>*/::~GenerateDataInHDF5(){
  file_.Close();
}


void GenerateDataInHDF5::WriteDataImpl( std::vector<int>const &data){

}


hdf5::File GenerateDataInHDF5::GetFileObject(){
    
    return file_;
}


void GenerateDataInHDF5::CreateHeader(hid_t g, DuneDataModel::DuneRawDataHeader header_info){
  
  constexpr hsize_t ndims = 1;
  constexpr hsize_t dims[ndims]={0};
  const hsize_t chunk_dims[ndims] = {static_cast<hsize_t>(chunksize_)};
  const hsize_t max_dims[ndims] = {header_info.Nadc_};

  const hsize_t hdims[ndims] = {1};
  auto dspace_h = hdf5::Dataspace::create_simple(ndims,hdims,NULL);
  auto space = hdf5::Dataspace::create_simple(ndims,dims,max_dims);
  auto prop = hdf5::Property::create();
  prop.set_chunk(ndims,chunk_dims);


  auto cid = hdf5::CompoundData::create(sizeof(DuneDataModel::DuneRawDataHeader)); 

  cid.insert_datatype<int>("Chan",HOFFSET(DuneDataModel::DuneRawDataHeader,chan_));
  cid.insert_datatype<double>("Pedestal",HOFFSET(DuneDataModel::DuneRawDataHeader,pedestal_));
  cid.insert_datatype<double>("Sigma",HOFFSET(DuneDataModel::DuneRawDataHeader,sigma_));
  cid.insert_datatype<int>("nADC",HOFFSET(DuneDataModel::DuneRawDataHeader,Nadc_));
  cid.insert_datatype<int>("Compression",HOFFSET(DuneDataModel::DuneRawDataHeader,compression_));

 // hsize_t hdims[ndims] = {1};
 // auto dspace_h = H5Screate_simple(1,hdims,NULL);
  //now the data
  std::string header_name = "ChannelHeader_"+std::to_string(header_info.chan_);
  std::string d_name = "ChannelID_"+std::to_string(header_info.chan_);
  auto dset_h = hdf5::Dataset::create_cd(g,header_name.c_str(),
                                                 cid,dspace_h, H5P_DEFAULT);

  //write the header during data-set creation and get done with it.
  dset_h.write_cd(dspace_h, cid,header_info);
  hdf5::Dataset dset_d = hdf5::Dataset::create<int>(g,d_name.c_str(),space,prop);
      
}

void GenerateDataInHDF5::WriteData(hid_t gid, DuneDataModel::DuneRawDataHeader header_info, std::vector<uint32_t>const& data){
  
  auto channel_id = header_info.chan_;
  std::string dset_name = "ChannelID_"+std::to_string(channel_id);
  auto dset_id = hdf5::Dataset::open(gid,dset_name.c_str());
  hsize_t ndims = 1;
  auto dspace_h = hdf5::Dataspace::get_space(dset_id);
  hsize_t max_dims[ndims];
  hsize_t old_dims[ndims];

  H5Sget_simple_extent_dims(dspace_h,old_dims,max_dims);
  hsize_t new_dims[ndims] = {old_dims[0]+data.size()};
  hsize_t slab_size[ndims] = {data.size()};
  dset_id.set_extent(new_dims);
  auto new_space = hdf5::Dataspace::get_space(dset_id);
  new_space.select_hyperslab(old_dims,slab_size);
  auto mem_space = hdf5::Dataspace::create_simple(ndims,slab_size,max_dims);
  dset_id.write(mem_space,new_space,data);
  
}