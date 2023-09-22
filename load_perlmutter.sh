#gcc, hdf5, cuda, root,tbb
module load cudatoolkit
module load cray-hdf5-parallel
module  load cray-mpich
module load PrgEnv-gnu
module load cmake
module load eigen

export VDT_DIR=/cvmfs/sft.cern.ch/lcg/releases/LCG_102/vdt/0.4.3/x86_64-centos8-gcc11-opt
#export JSON_CMAKE=/cvmfs/atlas-nightlies.cern.ch/repo/sw/master_Athena_x86_64-centos7-gcc11-opt/sw/lcg/releases/LCG_102b_ATLAS_13/jsonmcpp/3.10.5/x86_64-centos7-gcc11-opt/lib64/cmake
export JSON_CMAKE=/cvmfs/sft.cern.ch/lcg/releases/LCG_102/jsonmcpp/3.10.5/x86_64-centos9-gcc11-opt/lib64/cmake
export BOOST_DIR=/cvmfs/sft.cern.ch/lcg/releases/LCG_102/Boost/1.78.0/x86_64-centos9-gcc11-opt
export M_EIGEN_DIR=$EIGEN_DIR/include/eigen3

alias serialcmake='cmake ..  -DCMAKE_PREFIX_PATH=$JSON_CMAKE'
alias makeclean='make clean; rm -rf CMakeFiles; rm -rf CMakeCache.txt; rm -rf G__*; rm Makefile'
