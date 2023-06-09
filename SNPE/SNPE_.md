[TOC]

# SNPE

## Quantized vs Non-Quantized Models

非量化的DLC文件使用32位浮点数表示网络参数。

量化的DLC文件使用固定点表示网络参数，通常是8位权重以及8位或者32位biases. 固定点和Tensorflow量化模型使用的一样。

### ONNX

snpe-onnx-to-dlc的默认输出是一个非量化模型。这意味着所有的网络参数都保留在32位浮点数表示中，就像原始的ONNX模型中一样。要将模型量化为8位固定点，请参见snpe-dlc-quantize。

请注意，要使用snpe-dlc-quantize进行量化的模型必须将批处理维度设置为1。在推理过程中可以使用不同的批处理维度，通过在初始化过程中调整网络大小来实现。

### Tensorflow

使用到的工具是snpe-tensorflow-to-dlc， 情况与ONNX相似。

### Summary

| Runtime    | 量化模型                                                     | 未量化模型                                                   |
| :--------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| CPU or GPU | 兼容。模型会在运行时进行去量化，增加了网络初始化时间。可能会影响准确性。 | 兼容。该模型是这些运行时环境的本地格式。模型可以直接传递给运行时环境。可能比量化模型更准确。 |
| DSP        | 兼容。该模型是 DSP 运行时环境的本地格式。模型可以直接传递给运行时环境。准确性可能与非量化模型不同。 | 兼容。模型会在运行时进行量化，增加了网络初始化时间。准确性可能与量化模型不同。 |
| AIP        | 兼容。该模型是 AIP 运行时环境支持的格式。模型可以直接传递给运行时环境。 | 不兼容。AIP 运行时环境不支持非量化模型。                     |

如果网络主要在GPU和CPU上运行，建议在开发过程中尝试量化和非量化模型。如果量化模型提供足够的准确性，则可以在增加网络初始化时间的情况下使用它。好处是DLC文件更小。准确性、网络初始化时间和DLC文件大小之间的权衡是应用程序特定的。

如果网络主要在DSP上运行，则使用非量化模型没有任何好处。如前所述，它会大幅增加网络初始化时间和DLC文件大小，但不提供准确性方面的好处。

### Quantization Algorithm

本节介绍Qualcomm® Neural Processing SDK中使用的量化算法的概念。这些概念被snpe-dlc-quantize使用，并且在使用DSP运行时环境时，Qualcomm® Neural Processing SDK还会用于输入量化。

#### 算法细节

1. 计算输入浮点数据的真实范围。查找输入数据中的最小值和最大值,代表输入数据的真实范围。

2. 计算`encoding-min`和`encoding-max`。

   这些参数定义了可以表示为固定点格式的范围和浮点值。

   `encoding-min`：指定由固定点值0表示的最小浮点值。

   `encoding-max`：指定由固定点值255表示的最大浮点值。 

   每个步长大小的浮点值，其中步长=（encoding-max - encoding-min）/ 255，都可以表示。

   1. 将`encoding-min`和`encoding-max`设置为在上一步计算得到的真实最小值和真实最大值。

   2. 第一个要求：编码范围必须至少为0.01。

      将`encoding-max`调整为max（真实最大值，真实最小值+0.01）。
      
      第二个要求：浮点值0必须可以精确地表示。
      
       可以进一步调整`encoding-min`或`encoding-max`。
   

#### 确保浮点数0可以被精确的表示为固定点值

如何处理浮点数值0的情况。在输入数据包含0的情况下，需要特殊处理以确保浮点数值0可以被精确地表示为固定点值。

情况1：输入数据都是正数。在这种情况下，`encoding-min`设置为0.0，浮点数值0可以被最小的固定点值0精确地表示。例如，如果输入范围为[5.0，10.0]，则`encoding-min`= 0.0，`encoding-max`= 10.0。

情况2：输入数据都是负数。在这种情况下，`encoding-max`设置为0.0，浮点数值0可以被最大的固定点值255精确地表示。例如，如果输入范围为[-20.0，-6.0]，则`encoding-min`= -20.0，`encoding-max`= 0.0。

情况3：输入数据既有正数又有负数。在这种情况下，需要将`encoding-min`和`encoding-max`略微移动以使浮点数值0可以被精确地表示为固定点值。例如，如果输入范围为[-5.1，5.1]，则首先将`encoding-min`和`encoding-max`分别设置为-5.1和5.1。此时，浮点数值0无法被精确地表示为固定点值。最接近0的固定点值是-0.02和+0.02，对应的固定点值分别为127和128。为了使浮点数值0精确地表示为固定点值，需要将`encoding-min`和`encoding-max`都向左移动0.02。新的`encoding-min`为-5.12，新的`encoding-max`为5.08。这样，浮点数值0就可以被精确地表示为固定点值128。

#### 量化输入浮点值

在确定了`encoding-min`和`encoding-max`参数之后，可以使用它们将所有输入浮点值量化为它们的固定点表示。

量化公式：
$$
Quantized value = round(255*(float point value - encoding-min) / (encoding-max - encoding-min))
$$
量化值还应该被夹紧在0和255之间。

#### Quantization Example

- Inputs:
  - input values = [-1.8, -1.0, 0, 0.5]
- encoding-min is set to -1.8 and encoding-max to 0.5
- encoding range is 2.3, which is larger than the required 0.01
- encoding-min is adjusted to −1.803922 and encoding-max to 0.496078 to make zero exactly representable
- step size (delta or scale) is 0.009020
- Outputs:
  - quantized values are [0, 89, 200, 255]

#### Dequantization Example

- Inputs:
  - quantized values = [0, 89, 200, 255]
  - encoding-min = −1.803922, encoding-max = 0.496078
- step size is 0.009020
- Outputs:
  - dequantized values = [−1.8039, −1.0011, 0.0000, 0.4961]



https://docs.qualcomm.com/bundle/publicresource/topics/80-63442-2/quantized_models.html

















