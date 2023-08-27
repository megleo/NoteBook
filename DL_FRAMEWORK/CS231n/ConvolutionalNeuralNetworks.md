# Convolutional Netural Networks

## Fully Connected Layer

32*32*3 image --> stretch to 3072 x 1

![image-20230827102248457](images/image-20230827102248457.png)

## Convolution Layer

32x32x3 image --> preserve spatial structure. 保留空间信息

![image-20230827103040006](images/image-20230827103040006.png)

通过增加一个过滤器实现增加一层activation maps的效果.

![image-20230827104038698](images/image-20230827104038698.png)

Preview: ConvNet is a sequence of Convolutional Layers, interspersed with activation functions.

预览：ConvNet是一系列卷积层，中间穿插着激活函数

![image-20230827105705999](images/image-20230827105705999.png)

### 计算activation maps的shape

1. No padding

![image-20230827111541389](images/image-20230827111541389.png)

2. Padding

   ![image-20230827112052124](images/image-20230827112052124.png)

Rember back to ...

32x32 input convolved repeatedly with 5x5 filters shinks volumes spatially! shrinking too fast is not good, does not work well.

![image-20230827114241630](images/image-20230827114241630.png)

### Summary 

To summarize, the conv layer

- Accepts a volume of size 
  $$
  W_1\ *\ H_1\ *\ D_1
  $$

- Requires four hyperparameters

  - Number of fileters K.
  - their spatial extent F
  - the stride S
  - the mount of zero padding P

- Produces a volume of size 

  ![image-20230827115046117](images/image-20230827115046117.png)

## Pooling layer

- make the representations smaller and more manageable

- operates over each activation map independently.

  ![image-20230827120259096](images/image-20230827120259096.png)

  **它只是简单的下采样.** 长宽变化, channel 不变.

  ### Max pooling 

  ![image-20230827120520094](images/image-20230827120520094.png)

  ![image-20230827120828443](images/image-20230827120828443.png)

## Fully Connected Layer (FC layer)

