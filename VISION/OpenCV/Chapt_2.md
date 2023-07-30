# 第二章 opencv知识导论

## 使用cmake配置项目

1. cmake 提供了library

```cmake
# create our hello library
add_library(Hello hello.cpp hello.h)
# create our application that uses our new library
add_executable(executable main.cpp)
# Link our executable with the new library
target_link_libraries(executable Hello)
```

2.  管理依赖

   ```cmake
   cmake_minimum_required(VERSION 3.0)
   cmake_policy(SET CMP0012 NEW)
   
   ```

   



## 从/向磁盘中读取/写入图片

## 读取视频和访问相机设备

