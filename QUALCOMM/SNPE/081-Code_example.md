# C++ Tutorial - Build the sample

## 介绍

大多数的程序都将遵循下面的格式.

`请注意，虽然此示例代码不执行任何错误检查，但强烈建议用户在使用 Qualcomm® 神经处理 SDK API 时检查错误。`

1. Get Available Runtime
2. Load Network
3. Load UDO
4. Set Network Builder Options
5. Load Network Inputs
   1. Using User Buffers
   2. Using ITensors

6. Execute the Network & Process Output
   1. Using User Buffers
   2. Using ITensors

7. Using IOBufferDataTypeMap

```c++
static zdl::DlSystem::Runtime_t runtime = checkRuntime();
std::unique_ptr<zdl::DlContainer::IDlContainer> container = loadContainerFromFile(dlc);
std::unique_ptr<zdl::SNPE::SNPE> snpe = setBuilderOptions(container, runtime, useUserSuppliedBuffers);
std::unique_ptr<zdl::DlSystem::ITensor> inputTensor = loadInputTensor(snpe, fileLine); // ITensor
loadInputUserBuffer(applicationInputBuffers, snpe, fileLine); // User Buffer
executeNetwork(snpe , inputTensor, OutputDir, inputListNum); // ITensor
executeNetwork(snpe, inputMap, outputMap, applicationOutputBuffers, OutputDir, inputListNum); // User Buffer
```

##  获取当前的可用的运行时

