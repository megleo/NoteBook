# YoloV5

[TOC]

## 数据标注

1. 数据准备

   准备需要打标注的数据集， 新建一个名为VOC2007的文件夹

   ```TXT
   ├── VOC2007
   │   ├── Annotations		// 存放标签文件
   │   ├── JPEGImages		// 存放需要打标签的图片文件
   │   ├── predefined_classes.txt // 定义自己要标注的所有类别。（一般种类多的时候使用）
   |   └── YOLOLabels		// 
   ├── images
   │   ├── train
   │   └── val
   └── labels
       ├── train
       ├── train.cache
       ├── val
       └── val.cache
   ```

   JPEGImages这个文件夹放置待标注的图片，这里是两类图片，戴头盔和不戴头盔。

1. 标注工具：`labelling`

```shell
pip install labelimg -i https://pypi.tuna.tsinghua.edu.cn/simple
```

2. 工具使用

   



## Yolov5 训练

#### 1. 代码结构

```shell
├── data：主要是存放一些超参数的配置文件（这些文件（yaml文件）是用来配置训练集和测试集还有验证集的路径的，其中还包括目标检测的种类数和种类的名称）；还有一些官方提供测试的图片。如果是训练自己的数据集的话，那么就需要修改其中的yaml文件。但是自己的数据集不建议放在这个路径下面，而是建议把数据集放到yolov5项目的同级目录下面。
├── models：里面主要是一些网络构建的配置文件和函数，其中包含了该项目的四个不同的版本，分别为是s、m、l、x。从名字就可以看出，这几个版本的大小。他们的检测测度分别都是从快到慢，但是精确度分别是从低到高。这就是所谓的鱼和熊掌不可兼得。如果训练自己的数据集的话，就需要修改这里面相对应的yaml文件来训练自己模型。
├── utils：存放的是工具类的函数，里面有loss函数，metrics函数，plots函数等等。
├── weights：放置训练好的权重参数。
├── detect.py：利用训练好的权重参数进行目标检测，可以进行图像、视频和摄像头的检测。
├── train.py：训练自己的数据集的函数。
├── test.py：测试训练的结果的函数。
├──requirements.txt：这是一个文本文件，里面写着使用yolov5项目的环境依赖包的一些版本，可以利用该文本导入相应版本的包。
```

#### 2. 修改配置文件

预训练模型和数据集都准备好了，就可以开始训练自己的yolov5目标检测模型了，训练目标检测模型需要修改两个yaml文件中的参数。





## ModelConvert

`模型转换的工作是在X86 PC上进行的`

### 第一步： 生成量化数据集

```python
import cv2
import numpy as np
import argparse
import os
import random

def recursive_listdir(path):
    imgs = []
    files = os.listdir(path)
    for file in files:
        file_path = os.path.join(path, file)
        if os.path.isfile(file_path):
            if ".jpg" in file or ".JPG" in file:
                imgs.append(file)
        elif os.path.isdir(file_path):
            recursive_listdir(file_path)
    return imgs

def preprocess(raw_image, input_w, input_h):
    image = raw_image.copy()
    image_h, image_w, _ = image.shape

    # Resize
    scale = min(float(input_w) / image_w, float(input_h) / image_h)
    new_w = int(round(scale * image_w))
    new_h = int(round(scale * image_h))
    img = cv2.resize(image, dsize=(new_w, new_h), interpolation=cv2.INTER_LINEAR)

    # Pad
    pad_img = np.zeros((input_h, input_w, 3), dtype=np.float32)
    pad_img[0:new_h, 0:new_w, :] = img
    img = pad_img / 255.0

    return np.transpose(img, (2, 0, 1))[None , : ]


IMG_WIDTH  = 480
IMG_HEIGHT = 288
PATH = r"imgs/"
image_list = recursive_listdir(PATH)
random.shuffle(image_list)
if len(image_list) > 150:
    image_list = image_list[:150]

_count = 0
current_path = os.getcwd()
fp = open("input.txt", "w")
for i in image_list:
    img = preprocess(cv2.imread(PATH+i), input_w=IMG_WIDTH, input_h=IMG_HEIGHT)
    file_name = PATH + str(_count) + "_{}x{}".format(IMG_HEIGHT, IMG_WIDTH) + ".raw"
    _count += 1
    img.tofile(file_name)
    fp.write(current_path+"/"+file_name+"\n")
```

`做归一化的话，/=255.0;NHWC的话， 改成return np.transpose(img, (0,1,2))[None , : ]`

`Yolov5 每个版本的预处理是有区别的`

`一般来说训练的预处理、量化预处理、运行的预处理要一致`

### 第二步. 模型转换

模型的转换并不负载， 因为模型本身只是代表一些列算子， 但是不同的框架使用了不同的规范和实现， 在各个框架中做模型的转换， 通常会借用ONNX这个规范来完成。

```shell
snpe-onnx-to-dlc --input_network models/bvlc_alexnet/bvlc_alexnet/model.onnx
                 --output_path bvlc_alexnet.dlc
```

SNPE将onnx模型转换为dlc的命令很简单，转换失败最主要的原因就是算子不支持，这个需要自行去一层一层网络进行排查，转换失败的log也会给出一些提示。

`注：SNPE支持的ONNX算子可以在Support ONNX Ops中查到。`

`注：SNPE支持的网络层可以在Supported Network Layers中查到。`



