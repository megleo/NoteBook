# 常见软件安装

## opencv

## cmake

## Systemc

### Pre tools

```shell
sudo apt-get install g++ cmake build-essential  gdb doxygen graphviz
```

### Make Debug SystemC

```
mkdir buildDebug
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_DEBUG_POSTFIX="-d" -DCMAKE_INSTALL_PREFIX=/home/ts/software/systemc_install -DCMAKE_CXX_STANDARD=11 -DBUILD_SOURCE_DOCUMENTATION=ON ..
```

## LLVM

需要编译 llvm\lldb\clang\lld\

```
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=release ../
make -j16
make install  // 默认安装位置：/usr/local/bin下

```

编译万花筒

```
clang toy.cpp -lstdc++ -fuse-ld=lld `llvm-config --cxxflags --ldflags --system-libs --libs` -O3 -o toy
```



