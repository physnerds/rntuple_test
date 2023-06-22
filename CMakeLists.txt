cmake_minimum_required(VERSION 3.20 FATAL_ERROR)
project(h5demos LANGUAGES CXX C CUDA)
set(CMAKE_C_FLAGS $ENV{CFLAGS})
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(MPI REQUIRED)
#components and extra components required....
find_package(ROOT REQUIRED COMPONENTS ROOTNTuple)
find_package(HDF5 REQUIRED)
find_package(TBB REQUIRED)
find_package(Eigen3 3.4 REQUIRED NO_MODULE)
message("STATUS OF Eigen3_FOUND is ${Eigen3_FOUND}")

message("STATUS OF TBB_FOUND IS ${TBB_FOUND}")
enable_language(CUDA)
set(CMAKE_CUDA_STANDARD 17) #NEED TO FIX WITH 17
set(CMAKE_CUDA_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

message("STATUS OF HDF5_FOUND IS ${HDF5_FOUND}")
#set(HDF5_PREFER_PARALLEL TRUE)

set(CUDA_DIR $ENV{CUDATOOLKIT_HOME})
set(CUDA_INCLUDE_DIRS ${CUDA_DIR}/include)

#VDT for RNTUPLE STUFF
set(VDT_INCLUDE_DIRS $ENV{VDT_DIR}/include)
set(TBB_INCLUDE_DIR $ENV{TBB__HOME}/include)
set(TBB_LIBRARIES $ENV{TBB__HOME}/lib/libtbb.so)
set(BOOST_INCLUDE_DIRS $ENV{BOOST_DIR}/include)
#set(EIGEN_INCLUDE_DIRS $ENV{EIGEN_DIR}/include/eigen3)

message(STATUS "C++ compiler is : ${CMAKE_CXX_COMPILER}")
message(STATUS "C++ compiler identification is : ${CMAKE_CXX_COMPILER_ID}")


include_directories (SYSTEM 
		    ${MPI_CXX_INCLUDE_DIRS}
		    ${MPI_C_INCLUDE_DIRS}
            ${MPI_INCLUDE_DIRS}
		    ${HDF5_INCLUDE_DIRS}
            ${TBB_INCLUDE_DIR}
            ${CUDA_INCLUDE_DIRS}
		    ${ROOT_INCLUDE_DIRS}
            ${VDT_INCLUDE_DIRS}
            ${BOOST_INCLUDE_DIRS}
            ${CMAKE_SOURCE_DIR}/include
            )
		    
#link_directories ($ENV{ROOTSYS}/lib)

set (libraries ${libraries}
               ${ROOT_LIBRARIES}
	       #${MPI_CXX_LIBRARIES}
	       #${MPI_C_LIBRARIES}
           ${MPI_LIBRARIES}
           ${TBB_LIBRARIES}
	       ${HDF5_LIBRARIES}
              ROOT::Core
              ROOT::RIO
              ROOT::Tree
              ROOT::Hist)
              
              
#Generate Dictionary for the data model...
REFLEX_GENERATE_DICTIONARY(G__dunedatamodel include/DuneDataModel.h SELECTION include/dunedatamodel_def.xml)

REFLEX_GENERATE_DICTIONARY(G__dunesoadatamodel include/DuneSoADataModel.h include/SoAView.h include/SoACommon.h SELECTION include/dunesoamodel_def.xml)


add_library(dunedatamodel_dict SHARED G__dunedatamodel.cxx)
target_link_libraries(dunedatamodel_dict PUBLIC ${libraries})

add_library(dunesoadatamodel_dict SHARED G__dunesoadatamodel.cxx)
target_link_libraries(dunesoadatamodel_dict PUBLIC ${libraries})

add_executable(test_rntuple 
                src/DuneSoADataModel.cc 
                test_rntuple.cxx)
target_link_libraries(test_rntuple ${libraries} dunesoadatamodel_dict)

#now test the modular implementation...
add_executable(getdunehdf5 
              src/GenerateDataInHDF5.cc
              src/DuneDataModel.cc
              getdunehdf5.cc)
target_link_libraries(getdunehdf5 ${libraries})


add_executable(getdunerntuple
              src/GenerateDataInRntuple.cc
              src/DuneDataModel.cc
              getdunerntuple.cc)
target_link_libraries(getdunerntuple ${libraries} dunedatamodel_dict)


add_executable(test_SoA 
              test_SoA.cu)
set_property(TARGET test_SoA PROPERTY CUDA_ARCHITECTURES OFF)

target_link_libraries(test_SoA ${libraries} Eigen3::Eigen)

add_executable(readdunerntuple
               src/DuneDataModel.cc
               src/GenerateDataInRntuple.cc
              readdunerntuple.cc)
target_link_libraries(readdunerntuple ${libraries} dunedatamodel_dict)