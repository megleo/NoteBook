# SNPE ReleaseNote 2.10

## 重要的Note

1. SNPE默认使用Unsigned PD, 并且交付的骨架代码也没有签名。

   如果想签名的化，请签署skels。并且在platform的config选项中使能签名。

2. SNPE2和SNPE1支持相同的SOC平台。

## Unsigned PD

SNPE now defaults to using Unsigned PD for the DSP

- To set SNPE to use Signed PD:

  - Net run

    ```shell
    snpe-net-run ... --platform_options="unsignedPD:OFF"
    ```

  - C++ API

    ```c++
    zdl::DlSystem::PlatformConfig platformConfig;
    platformConfig.setPlatformOptions("unsignedPD:OFF");
    zdl::SNPE::SNPEBuilder snpeBuilder;
    snpeBuilder.setPlatforConfig(platformConfig);
    ```

  - C API

    ```;
    Snpe_PlatformConfig_Handle_t platformConfigHandle = Snpe_PlatformConfig_Create();
    Snpe_PlatformConfig_SetPlatformOptions(platformConfigHandle, "unsignedPD:OFF");
    Snpe_SNPEBuilder_Handle_t snpeBuilderHandle = Snpe_SNPEBuilder_Create(containerHandle);
    ....
    ```

  > 与SNPE 1.x相比，SNPE2.x使用了新的磁盘上模型表示。新的DLC格式与以前的格式和SNPE 2不兼容。SNPE.2x库实现中不包含任何使用SNPE1.x 旧格式加载网络的功能。
  >
  > \
  >
  > 造成的结果
  >
  > 1. 必须重新转换模型以在2.0上部署
  >
  > 2. Another consequence is that layers (now generally referred to as "ops") and tensors 的名称可能和转换模型中相应的层和张量的名称不相同。
  >
  >    Inputs(generally referred to as Data layer types) and "Const" layer types are now folded into consuming Ops and not considered as standalone layers. As a result, they will no longer be visable in snpe-diagview 和snpe-dlc-info中了。

  ## Handling Integer Input/Output Tensors

  SNPE2.x 支持直接整型输入。转换器/量化器将不再将输入表示为浮点张量或者Q8 tensor, 而是一个实际的UIntN tensor： N可以是8， 18， 32。UInt8tensor的有效范围是0-255

  SNPE不能将Q8输入张量转换为UInt8输入张量， 因此必须修改此类网络中的任何现有代码，以使用新的api来创建实际的UInt8 UserBuffer输入

  ```
  Snpe_UserBufferEncodingUintN_Create()
  ```

  ## Workflow Changes

  > 为了支持更大的灵活性并改进对每个新版本SNPE SDK的采用，SNPE -dlc-quantize被重构为两个独立的工具snpe-dlc-quant -用于量化浮点dlcsnpe-dlc-graph-prepare -用于执行dlc的离线图形准备。为了简化转换，snpe-dic-quantize工具已经被一个脚本取代，该脚本根据传递的命令行参数调用新的量化器和图形准备工具。
  >
  > 
  >
  > 当使用SNPEBuilder指定要包含在execute()返回的张量映射中的特定输出时，应该使用setOutputTensors() API而不是setOutputLayers()。在C API中，基于层的API将很快被弃用，然后只能通过指定张量名称来请求特定的输出。除了新的整数张量，SNPE2现在支持fp16输入和输出张量ArgMax现在总是输出一个uint32，所以如果SNPE1使用int8输出可能会有一些性能影响，因为数据大小更大。