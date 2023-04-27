[TOC]

## 装饰器

装饰器其实就是一个函数，这个函数的形参是一个函数对象，大多数情况下，返回的是一个函数对象。



```python
import time

def timeit(f):
    def wrapper(*args, **kargs):
        print("start")
        start = time.time()
        ret = f(*args, **kargs)
        print(time.time() - start)
        print("end")
        return ret
    return wrapper

@timeit
def my_func(x):
    print("Sleep {}s.".format(x))
    time.sleep(x)


my_func(1)
```

等价于：

```python
import time

def timeit(f):
    def wrapper(x):
        print("start")
        start = time.time()
        ret = f(x)
        print(time.time() - start)
        print("end")
        return ret
    return wrapper

def my_func(x):
    print("Sleep {}s.".format(x))
    time.sleep(x)


my_func = timeit(my_func)

my_func(1)
```

```python
import time

def timeit(iteration):
    print(2)
    def inner(f):
        print(3)
        def wrapper(*args, **kargs):
            print(4)
            start = time.time()
            for _ in range(iteration):
                ret = f(*args, **kargs)
                print("ret = ", ret)
            print(time.time() - start)
            return ret
        return wrapper
    return inner


@timeit(5)
def double(x):
    print(1)
    return x * 2

print(double(2))
```



