## 03. Backend

客户端通过后端与Qualcomm AI Engine Direct 交互。一般而言，QNN的后端是一个实现QNN API的软件实体， 通常以编译的共享库的方式提供。QNN backend 和QNN backend library 通常可以互换使用。

QNN 后端库为每个硬件加速器核心提供集成模块化。

QNN SDK提供多个后端库，这些库文件可以在<QNN_SDK_ROOT>/lib/\<target-platform\>文件夹中找到。QNN后端库的命名规则如下：

- CPU 

  libQnnCpu.so

  QnnCpu: CPU backend library, same name used across all targets.

- DSP backend

  1. hexagon-v##

     - `libQnnDspV##Skel.so`
     - `libQnnDspV##.so`

  2. aarch64-android

     - `libQnnDsp.so`
     - `libQnnDspV##Stub.so`

  3. aarch64-windows-msvc

     - `QnnDsp.dll`

     - `QnnDspV##Stub.dll`

  > libQnnDspV##Skel.so ：DSP 本机库，将使用相应的 QnnDspV##Stub 从设备 CPU 端加载，并负责在 DSP 加速器上执行图形，作为 CPU 端后端的代理。
  >
  > QnnDspV##Stub ：DSP CPU 端代理库，通过 RPC 通道与 DSP 端的 libQnnDspV##Skel.so 进行通信。
  >
  > QnnDsp ：DSP设备CPU端后端库，根据SoC加载相应的DSP stub库。
  >
  > libQnnDspV##.so ：DSP 本机后端库，允许直接集成在 DSP 上，无需 RPC。
  
- GPU backend

  1. aarch64-android

     - libQnnGpu.so

     QnnGpu: GPU backend library, same name across all targets.

- HTP backend
  
  Backend for Hexagon HTP hardware accelator
  
  - hexagon-v##
  
    - `libQnnHtpV##Skel.so`
    - `libQnnHtpV##.so`
  
  - aarch64-android
  
    - `libQnnHtp.so`
    - `libQnnHtpPrepare.so`
    - `libQnnHtpV##Stub.so`
  
  - x86_64-linux-clang
  
    - `libQnnHtp.so`
  
  - aarch64-windows-msvc
  
    - `QnnHtp.dll`
    - `QnnHtpPrepare.dll`
    - `QnnHtpV##Stub.dll`
  
    libQnnHtpV##Skel.so: HTP本机库，将使用相应的QnnHtpV##Stub 从设备CPU端加载，并负责在HTP加速器上执行graph作为CPU端后端的代理。
  
    QnnHtpV##Stub ：HTP CPU 端代理库，通过 RPC 通道与 HTP 端的 libQnnHtpV##Skel.so 进行通信。
  
    QnnHtp ：HTP 后端库。 有两种变体：（aarch64-android）Android设备CPU端后端库，根据SoC加载相应的HTP存根库； (x86_64-linux) x86 Linux 主机后端充当硬件加速器的功能模拟器，并支持 x86_64 主机上的图形准备。
  
- HTA backend
  
  HTA hardware accelerator
  
  - aarch64-android
  
    - `libQnnHta.so`
  
     HTA backend library, same name used across all supported targets.
  
- Saver

  Backend which captures QNN API calls and saves them to a file as they are issued by a client. This can later be replayed for support and troubleshooting.    

---

### DSP



