# Overview of UDO

User-Defined Operation. 支持用户自定义SDK不支持的神经网络算子。SDK提供了无缝方式执行这些算子，与内部算子相比，几乎无额外的开销。

**Anatomy of a UDO package** UDO包的解剖

SDK允许用户以动态库的方式提供UDO实现，可以查询，加载和运用动态，以及使用该动态库定义的内核进行推理。

The figure below illustrates the concept of a UDO package:

![image-20230719155652785](images/image-20230719155652785.png)

从图中可以看出，**UDO包由注册组件和实现组件组成**。 它们通常用**一个注册库和一组实现库**分别表示，每个注册库对应一个实现内核可用的硬件加速器。 如果用户愿意，可以选择将这两个组件构建到一个库中。

**UDO workflow**

![image-20230719160353337](images/image-20230719160353337.png)

1. 工作流程的第一步是识别模型中需要表示为用户定义算子，并通过配置文件描述其属性。 定义 UDO 中描述了该文件的格式和内容。

   ... TBD

# Defining a UDO

 This section explains how such a UDO can be specified. 

See Overview of UDO for more details about UDO and Preparing a model with UDO for details on how to convert a model that contains a UDO into a Qualcomm® Neural Processing SDK DLC for supported frameworks.

用户可以通过配置规范文件来表达自定义算子的属性。UDO配置使用JavaScript对象表示法（Json）语法格式来创建算子的描述。**配置文件定义了可以用键值对指定的字段，并按照JSON规则排列成树形结构。** **这些信息将用于时别框架模型内的操作，然后最终序列化到DLC模型中，这意味着配置中任何更改都需要重新生成DLC模型，以保证序列化正确的信息。**

UDO Config Field Description

```json
{
    "UdoPackage_0":
    {
        "Operators": [
            {
                "type": "",
                "inputs":[
                    {"name":"", "per_core_data_types":{"CPU":"FLOAT_32", "GPU":"FLOAT_32", "DSP":"UINT_8"},
                    "static": true, "tensor_layout": "NHWC"},
                    {"name":"", "data_type": "FLOAT_32",
                    "static": true, "tensor_layout": "NHWC"},
                ],
                "outputs":[
                    {"name":"", "per_core_data_types":{"CPU":"FLOAT_32", "GPU":"FLOAT_32", "DSP":"UINT_8"}},
                    {"name":"", "data_type": "FLOAT_32"}
                ],
                "scalar_params": [
                    {"name":"scalar_param_1", "data_type": "INT_32"}
                ],
                "tensor_params": [
                    {"name":"tensor_param_1", "data_type": "FLOAT_32", "tensor_layout": "NHWC"},
                ],
                "core_types": ["CPU", "GPU", "DSP"],
                "dsp_arch_types": ["v66", "v68", "v69", "v73"]
            }
        ],
        "UDO_PACKAGE_NAME": "MyCustomUdoPackage"
    }
}
```

**上面的描述只是一个通用配置文件，用于帮助定义用户可以填写的字段。 必填字段提供有特定值，而可选字段则用空字符串表示。** 请注意，可选字段仅意味着如果未提供则有默认值，或者将使用空字符串。 每个可用字段的完整详细信息按层次结构描述如下：

- **UdoPackage**  每个 UDO 包都可以描述为“UdoPackage_i”，其中 i 表示包的生成顺序。 用户也可以自由使用空字符串，但字典结构是必要的。可以在单个配置文件中定义多个 UDO 包。 

- **Operators**  This is a child node of a particular UdoPackage indicating the number of operators present.

  - **type** defines the type of the operation.

  - **inputs:** a list of input tensors to the operation. Each input is a dictionary object.

    - **name:** An optional field that describes the name of the input tensor. Since the name of the input tensor is variable, the user is not required to provide this.

    - **per_core_data_type** A dictionary object specifying the data-type of this input tensor in each core. Alternatively, if the user wishes to have the same data-type across all specified cores, then the user can specify the option “data_type” followed by the data-type. The supported data-types are:

      - FLOAT_16
      - FLOAT_32
      - FIXED_8
      - FIXED_4
      - FIXED_16
      - UINT_8
      - UINT_16
      - UINT_32
      - STRING

    - **static:** A boolean field that is required if the input data is static, i.e data is provided in the model. This field needs to be set if the input tensor will contain data, otherwise the input will be treated dynamically, and the data will not be serialized.

    - **tensor_layout:** A string field that describes the canonical dimension format of the input tensor. The supported values are: 

      - NCHW
      - NWCH

    - **outputs:** A list of output tensors to the operation.

    - **scalar_params:** A list of scalar-valued attributes.3

      - **name:** A required field that describes the name of the scalar parameter.

      - **data_type:** A required field that describes the data-type supported by this scalar parameter.

    - **tensor_params:** A list of tensor-valued attributes.2 3
    - **core_types:** The intended IP cores for this particular operation. The supported core_types:
      - CPU
      - GPU
      - DSP
    - **dsp_arch_types:** The intended DSP architecture types for DSP core type. The supported dsp_arch_types:
      - v65
      - v66
      - v68
      - v69
      - v73

    - **UDO_PACKAGE_NAME:** The name of the UDO Package, which can be any valid string.1

# Defining a UDO Package

UDO Package 主要是由一个注册库和多个实现库组成。主要的想法是注册库包含算子性质的信息，实现库则包含执行算子所需要的操作。UDO Package可以由一个UDO配置文件定义，其中包括操作和路径的文本规范，最终帮助定义表示UDO包的目录结构。UDO Package的定义将在此章表示，创建UDO Package部分将在下一章节。

```SHELL
(base) ts@menglei:~/udo/SoftmaxUdoPackage$ tree -L 3
.
├── common.mk
├── config
│   └── Softmax_v68.json
├── include
│   └── utils
│       ├── IUdoOpDefinition.hpp
│       ├── UdoMacros.hpp
│       └── UdoUtil.hpp
├── jni
│   ├── Android.mk
│   ├── Application.mk
│   └── src
│       ├── CPU
│       ├── DSP_V68
│       ├── GPU
│       ├── reg
│       └── utils
├── libs
│   ├── arm64-v8a
│   │   ├── libc++_shared.so
│   │   ├── libUdoSoftmaxUdoPackageImplCpu.so
│   │   └── libUdoSoftmaxUdoPackageReg.so
│   ├── dsp_v68
│   │   └── libUdoSoftmaxUdoPackageImplDsp.so
│   └── x86-64_linux_clang
│       ├── libUdoSoftmaxUdoPackageImplCpu.so
│       ├── libUdoSoftmaxUdoPackageImplDsp.so
│       └── libUdoSoftmaxUdoPackageReg.so
├── Makefile
└── obj
    └── local
        ├── arm64-v8a
        ├── dsp_v68
        └── x86-64_linux_clang
```

```shell
source ${QNN_ROOT}bin/envsetup.sh
source ${SNPE_ROOT}bin/envsetup.sh
export SNPE_UDO_ROOT=${SNPE_ROOT}/share/SNPE/SnpeUdo
export PYTHONPATH=$PYTHONPATH:/opt/qcom/aistack/snpe/2.12.0.230626/lib/python
export LD_LIBRARY_PATH=/home/ts/anaconda3/envs/py3.8/lib
```

```SHELL
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/ts/udo/SoftmaxUdoPackage/libs/x86-64_linux_clang
snpe-tensorflow-to-dlc -i tensorflow/inception_v3_2016_08_28_frozen.pb -d input 1,299,299,3 --out_node InceptionV3/Predictions/Reshape_1 --udo_config_paths $SNPE_ROOT/examples/SNPE/NativeCpp/UdoExample/Softmax/config/Softmax_v68.json -o dlc/inception_v3_udo.dlc
snpe-dlc-quantize --input_dlc dlc/inception_v3_udo.dlc --input_list target_raw_list.txt --udo_package_path /home/ts/udo/SoftmaxUdoPackage/libs/x86-64_linux_clang/libUdoSoftmaxUdoPackageReg.so  --output_dlc dlc/inception_v3_udo_qantized.dlc 
```



```shell
snpe-net-run --container dlc/inception_v3_udo_qantized.dlc --input_list target_raw_list.txt --udo_package_path /home/ts/udo/SoftmaxUdoPackage/libs/x86-64_linux_clang/libUdoSoftmaxUdoPackageReg.so
```





























