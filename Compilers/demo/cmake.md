# demo 项目的CMakeLists.txt

## 设置编译选项

`cmake_cxx_flags`

```cmake
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -Wall -g -pedantic")
message(INFO ${CMAKE_CXX_FLAGS})
```

**少优化->多优化：**

**O0 -->> O1 -->> O2 -->> O3**

**-O0 ** **表示没有优化,-O1为缺省值，-O3优化级别最高**

原来-Os相当于-O2.5。是使用了所有-O2的优化选项，但又不缩减代码尺寸的方法



--Wall **开启所有警告（可以理解为warinig all** 使用它能够使GCC产生尽可能多的警告信息。**（非常推荐使用该选项）**



-g 生成调试信息。GNU 调试器可利用该信息。



-pedantic 选项，那么使用了扩展语法的地方将产生相应的警告信息



## Bison

```cmake
# add macros to generate ANTLR Cpp code from grammar
find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

BISON_TARGET(demoParser ${CMAKE_CURRENT_SOURCE_DIR}/grammar/demoParser.yy ${CMAKE_CURRENT_BINARY_DIR}/demoParser.cpp
             COMPILE_FLAGS "-Wall -Wcounterexamples --defines=${CMAKE_CURRENT_BINARY_DIR}/demoParser.h")
FLEX_TARGET(demoScanner ${CMAKE_CURRENT_SOURCE_DIR}/grammar/demoScanner.ll  ${CMAKE_CURRENT_BINARY_DIR}/demoScanner.cpp
            COMPILE_FLAGS "--debug --c++")
ADD_FLEX_BISON_DEPENDENCY(demoScanner demoParser)

```



```cmake
BISON_TARGET(<Name> <YaccInput> <CodeOutput>
             [COMPILE_FLAGS <flags>]
             [DEFINES_FILE <file>]
             [VERBOSE [<file>]]
             [REPORT_FILE <file>]
             )
```















































