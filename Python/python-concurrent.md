# python并发编程

## Python对并发编程的支持

- 多线程：threading， 利用CPU和IO可以同时执行的原理，不让CPU干巴巴等待IO完成。
- 多进程： multiprocessing, 利用多核CPU的能力，真正的并发执行任务。
- 异步IO：asyncio， 在但线程利用CPU和IO同时执行的原理，实现函数异步执行。



- 使用Lock对资源加锁，防止冲突访问。
- 使用Queue实现不同线程/进程之间的数据通信，实现生产者-消费者模式
- 使用线程池Pool和进程池Pool, 简化线程/进程的任务提交、等待结果、获取结果
- 使用subprocess启动外部程序的进程，并进行输入输出交互。

## Python并发编程有三种方式

### CPU 密集型（CPU-bound）

- CPU密集型也称为计算密集型，是指I/O在很短的时间内就能完成，CPU需要大量的时间在数据的计算和处理上，特点：CPU的占用率相当高。

  解压缩苏算法、加密解密算法、正则表达式算法

### IO 密集型（I/O bound）

- IO密集型是指系统运作大部分的状况是CPU在等待I/O（硬盘/内存）的读写操作，CPU的占用率仍然较低。

  文件处理程序、网络爬虫程序、读写数据库程序

### 多进程 Process

- 优点：可以用多核CPU进行并行计算
- 缺点：占用资源多，可启动数目比线程少
- 场景： CPU密集型计算

### 多线程Thread

- 优点：相比进程，更轻量级、占用资源比较少

- 缺点

  - 相比进程：多线程只能并发执行，不能利用多CPU（GIL），一个线程只能用一个CPU。

  - 相比协程： 启动数目有限，占用内存资源，有线程切换开销。

 - 场景：IO密集型计算，同时运行的任务数目要求不多。

### 多协程Coroutine

- 优点：内存开销最少，启动协程数量最多
- 缺点：支持的库有限制（aiohttp vs requests）代码实现复杂。

- 适用：IO密集型计算，需要超多任务运行，但有现成库支持的场景。

![image-20230421093451460](images/image-20230421093451460.png)



## 如何选择？

![image-20230421094551479](images/image-20230421094551479.png)



## Python 为什么慢吞吞？

Python 慢，在一些特殊场景，python 比C++慢100～200倍。



### 原因：

1. Python 是解释型语言，边解释边执行。
2. GIL 无法利用多核CPU并发执行的。



### GIL

全局解释器锁 （GLOBAL INTERPRETER LOCK，缩写GIL）

是解释器用于同步线程的一种机制，它使得任何时刻仅有一个线程在执行。

即使在多核处理器上，使用GIL的解释器也只允许同一时间执行一个线程。



![image-20230421095552387](images/image-20230421095552387.png)

### 为什么有GIL这个东西？

> Python语言的设计初期，为了规避并发问题引入了GIL，现在想去掉也无法去除了， 已经形成软件生态。



为了解决多线程之间数据完整和状态同步问题

Python中对象的管理，是使用引用计数器进行的， 引用数为0则释放对象。



![image-20230421100203903](images/image-20230421100203903.png)



GIL 确实有好处：‵简化了对共享资源的管理‵





### 总结

1. 多线程threading机制依然有用，用于I/O密集计算。

   因为在I/O期间，线程会释放GIL， 实现CPU和IO的并行，因此多线程用于IO密集型可以大幅度提升速度。

   但是多线程用于CPU密集计算时，只会拖慢速度。

2. 使用multiprocessing的多进程机制实现并行计算、利用多核CPU优势。

   为了应对GIL的问题，Python提供了multiprocessing.



## Python 多线程实战

![image-20230421101629362](images/image-20230421101629362.png)







```python
import time
import threading
import requests

urls = ["https://www.cnblogs.com/#p{}".format(page) for page in range(1, 50 + 1)]


def craw(urls):
    r = requests.get(urls)

    
def single_thread():
    print("single_thread begin.")
    for url in urls:
        craw(url)
    print("single_thread end.")


def multi_thread():
    print("multi_thread begin.")
    threads = []
    for url in urls:
        threads.append(
            threading.Thread(target=craw, args=(url,))
        )
    for thread in threads:
        thread.start()
    for thread in threads:
        thread.join()
    print("multi_thread end.")


if __name__ == "__main__":
    start = time.time()
    single_thread()
    end = time.time()
    print("single thread cost: {} seconds".format(end - start))

    start = time.time()
    multi_thread()
    end = time.time()
    print("multi thread cost: {} seconds".format(end - start))
```



### python 实现生产者消费者模式

### 1. 多组件的Pipeline技术架构

复杂的事情一般不会一下子做完，而是会分为很多中间步骤一步步完成。



![image-20230421104039493](images/image-20230421104039493.png)



### 2. 生产者消费者爬虫的架构



![image-20230421104218394](images/image-20230421104218394.png)





### 3. 多线程数据通信的queue.Queue

queue.Queue可用于多线程之间的、线程安全的数据通信



> 1. 导入类库
>
>    ```python
>    import queue
>    ```
>
> 2. 创建Queue
>
>    ```python
>    q = queue.Queue()
>    ```
>
> 3. 添加元素
>
>    ```python
>    q.put(item)
>    ```
>
> 4. 获取元素
>
>    ```python
>    item = q.get()
>    ```
>
> 5. 查询状态
>
>    1. 查看元素的多少
>
>       ```python
>       q.qsize()
>       ```
>
>    2. 判断是否为空
>
>       ```python
>       q.empty()
>       ```
>
>    3. 判断是否已满
>
>       ```python
>       q.full
>       ```
>
> 

### 代码实战

```python
im
```

































