# SystemC Install

## Pre tools

```shell
sudo apt-get install g++ cmake build-essential  gdb doxygen graphviz
```

## Make Debug SystemC

```
mkdir buildDebug
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_DEBUG_POSTFIX="-d" -DCMAKE_INSTALL_PREFIX=/home/ts/software/systemc_install -DCMAKE_CXX_STANDARD=11 -DBUILD_SOURCE_DOCUMENTATION=ON ..
```

