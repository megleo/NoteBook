## ubuntu

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



---

