# 第九章 顺序容器

## 9.3 顺序容器操作

**顺序容器和关联容器的不同指出在于两者的组织元素的方式。 不同的组织方式，直接影响了容器的存储和增删改查操作。**

**本节讲述顺序容器的所有特有操作。**

### 9.3.1 向顺序容器添加元素

**除了array外， 所有的顺序容器都是支持灵活的内存管理的。在运行时，动态添加删除元素来改变容器的大小。**

| **-**                    | **-**                                                                                                              |
| ------------------------------ | ------------------------------------------------------------------------------------------------------------------------ |
| **forward_list**         | **有自己版本的insert、和emplace**                                                                                  |
| **vector\string**        | **不支持push_front\emplace_front**                                                                                 |
| **c.push_back(t)**       | **在c的尾部创建一个由t创建的元素，返回VOID**                                                                       |
| **c.emplace_back(arg)**  | **在c的尾部创建一个由参数arg创建的元素，返回VOID**                                                                 |
| **c.push_front(t)**      | **在c的头部创建一个由t创建的元素，返回VOID**                                                                       |
| **c.emplace_front(arg)** | **在c的头部创建一个由参数arg创建的元素，返回VOID**                                                                 |
| **c.insert(p, t)**       | **在迭代器p指向元素的前面插入一个由t创建的元素，返回新添加的第一元素的迭代器。**                                   |
| **c.insert(p, arg)**     | **在迭代器p指向元素的前面插入一个由参数arg创建的元素，返回新添加的第一元素的迭代器。**                             |
| **c.insert(p, n, t)**    | **在迭代器p指向元素的前面插入n个由t创建的元素，返回新添加的第一元素的迭代器。**                                    |
| **c.insert(p, b, e)**    | **将迭代器b和e指定范围内的元素插到迭代器p指向的元素之前，b和e中不能指向c中的元素。返回新添加的第一元素的迭代器。** |
| **c.insert(p, il)**      | **il是一个花括号包围的元素值列表。**                                                                               |

#### 使用emplace 操作

新标准引入了三个成员，emplace、emplace_front、emplace_back，这三个函数操作构造函数而不是拷贝元素。对应这操作insert、push_front、push_back.

```C++
// 在c的末尾构造一个Sales_data对象， 
c.emplace_back("1000", 24, 15.99)
    
c.push_back("1000", 24, 15.99)		// 操作，只能操作对象进行拷贝。可以是临时对象
```

### 9.3.2 访问元素

| -        |                              -                               |
| -------- | :----------------------------------------------------------: |
| at       |     适用于string、vector、deque、array 物理空间是连续的      |
| back     |                     不适用于forward_list                     |
| c.font() |         返回c中首元素的引用，若c为空，函数行为未定义         |
| c[n]     | 返回c中下表为n的元素的引用，n是一个无符号整数，若n>=c.size(), 则函数行为未定义。 |
| c.at(n)  | 返回c中下表为n的元素的引用，若下标越界抛出一个out_of_range 异常 |
|          | <font color=red>对空容器调用front和back，就像使用越界的下标一样，是一种严重的程序设计错误，这种行为是未定义的。</font> |

```c++



```







:point_left:[上一节](/ProjectDocs/chap8.md):heart:  :heart:   :heart:   :heart:   :heart:   :heart:   :heart:   :heart:  :heart:  :heart:   :heart:   :heart:   :heart:   :heart:   :heart:   :heart:  :heart:  :heart:   :heart:   :heart:   :heart:   :heart:   :heart:   :heart:  :heart:  :heart:   :heart:   :heart:   :heart:   :heart:   :heart:   :heart:  :heart:  :heart:   :heart:   :heart:   :heart:   :heart:   :heart:   :heart:  :heart:  :heart:   :heart:   :heart:   :heart:   :heart:   :heart:   :heart:  :heart:  :heart:    :point_right:[下一节](/ProjectDocs/chap10.md)
