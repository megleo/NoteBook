# GPU 基本知识

![image-20230317193658604](images/image-20230317193658604.png)



> CPUs延迟导向设计
>
> ![image-20230317194121493](images/image-20230317194121493.png)
>
> - 内存大：多级缓存机制
> - 控制复杂
> - - 分支预测
>   - 流水线数据前送
> - 运算单元强大
> - - 整型浮点型复杂运算速度快



> GPUs: 吞吐导向设计
>
> ![image-20230317194714797](images/image-20230317194714797.png)



## GPU编程

- 计算密集型

  数值计算的比例要远大于内存操作， 因此内存访问的延时，可以被计算覆盖。

- 数据并行

  大任务可以拆解成执行相同指令的小任务，因此对复杂流程控制的需求较低。



# GPU 编程与CUDA

`CUDA(Compute Unified Device Architecture),由英伟达公司2007年开始推出,初衷是为GPU增加一个易用的编程
接口,让开发者无需学习复杂的着色语言或者图形处理原语。`



`OpenCL(Open Computing Languge)是2008年发布的异构平台并行编程的开放标准,也是一个编程框架。OpenCL
相比CUDA,支持的平台更多,除了GPU还支持CPU、DSP、FPGA等设备。`



##  CUDA 编程并行计算整体流程

```c++
void GPUKernel(float* A, float* B, float* C, int n)
{
    // 1. Allocate device memory for A, B, and C, 
    // Copy A and B to device memory
    
    // 2. Kernel launch code - to have the device
    // to perform the actual vector addition
    
    // 3. copy C from the device memory 
    // Free device vectors
}
```

![image-20230317200008958](images/image-20230317200008958.png)

## CUDA 编程术语： 硬件

![image-20230317200136485](images/image-20230317200136485.png)

- Device=GPU
- Host = CPU
- Kernel = GPU上运行的函数

## CUDA 编程术语： 内存模型

- CUDA中的内存模型分为以下几个层次
  - 每个线程处理器(SP)都有自己的registers（寄存器）
  - 每个SP都有自己的local memory局部存储器， register 和local memory 只能被自己的线程访问
  - 每个多核处理器（SM）都哟有自己的shared memory（共享内存），shared memory可以被线程块内所有的线程访问。
  - 一个GPU的所有SM共有一块global memory(全局内存)， 不同线程块的线程可以使用。

## CUDA 编程术语： 软件

-  CUDA中的内存模型分为几个层次
  - 线程处理器（SP）对应线程（thread）
  - 多核处理器（SM）对应线程块（thread block）
  - 设备端（device）对应线程块组合体（grid）
- 一个kernel其实由一个grid来执行。
- 一个kernel一次只能在一个GPU上执行。

## 线程块： 可扩展的集合体

将线程数组分为多个块

- 快内

