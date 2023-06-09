[toc]

# SNPE examples

SNPE Version: snpe-2.5.0.4052

SNPE SDK 文件下有一个example文件夹，这个文件夹是提供开发人员开发的样例，本节主要讲述该文件下的项目如何编译，以及其代码流程。

该文件下共有三个目录，可以理解为三个不同的项目android  GoogleNAS  NativeCpp。

## examples 目录

```ABAP
(snpe) ts@menglei:~/SNPE/snpe-2.5.0.4052/examples$ tree -L 3
.
├── android
│   ├── image-classifiers
│   │   ├── app
│   │   ├── build.gradle
│   │   ├── gradle
│   │   ├── gradle.properties
│   │   ├── gradlew
│   │   ├── gradlew.bat
│   │   ├── settings.gradle
│   │   ├── setup_alexnet.sh
│   │   └── setup_inceptionv3.sh
│   └── psnpe-demo
│       ├── app
│       ├── build.gradle
│       ├── gradle
│       ├── gradle.properties
│       ├── gradlew
│       ├── gradlew.bat
│       ├── resources
│       └── settings.gradle
├── GoogleNAS
│   ├── build.sh
│   ├── config.json
│   ├── custom_processing.py
│   ├── Dockerfile
│   ├── mnist_example_data.py
│   ├── readme.md
│   ├── run_example.sh
│   └── setup.sh
└── NativeCpp
    ├── SampleCode
    │   ├── jni
    │   ├── Makefile.aarch64-oe-linux-gcc8.2
    │   ├── Makefile.aarch64-oe-linux-gcc9.3
    │   ├── Makefile.aarch64-ubuntu-gcc7.5
    │   ├── Makefile.x86_64-linux-clang
    │   └── README.txt
    └── UdoExample
        ├── Conv2D
        └── Softmax
```

##  NativeCpp

### SampleCode

#### 编译android 版本

需要用到ndk-build

```shell
cd /home/ts/SNPE/snpe-2.5.0.4052/examples/NativeCpp/SampleCode/jni
ndk-build
```

编译结果会放在jni的上一级目录下obj 下

```shell
(snpe) ts@menglei:~/SNPE/snpe-2.5.0.4052/examples/NativeCpp/SampleCode/obj$ tree -L 3
.
└── local
    ├── arm64-v8a
    │   ├── libc++_shared.so
    │   ├── libSNPE.so
    │   ├── objs
    │   └── snpe-sample
    └── armeabi-v7a
        ├── libc++_shared.so
        ├── libSNPE.so
        ├── objs
        └── snpe-sample
```

#### 项目代码解读



