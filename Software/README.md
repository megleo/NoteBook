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



## OpenCV

```shell
cmake  -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local -D CMAKE_PREFIX_PATH=/opt/Qt5.12.12/5.12.12/gcc_64/lib/cmake/Qt5 -D WITH_TBB=ON -D WITH_V4L=ON -D WITH_QT=ON  -D WITH_GTK=ON -D WITH_OPENGL=ON  -D WITH_VTK=ON -D OPENCV_GENERATE_PKGCONFIG=YES -D INSTALL_C_EXAMPLES=ON -D INSTALL_PYTHON_EXAMPLES=ON -D OPENCV_GENERATE_PKGCONFIG=ON -D OPENCV_ENABLE_NONFREE = ON -D OPENCV_EXTRA_MODULES_PATH=/home/m/software/opencv_contrib-4.5.5/modules -D BUILD_EXAMPLES=ON ..
```

