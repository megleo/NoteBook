### Chapter 1 LLVM IR 生成

add.c

```c
int globvar = 12;
int add(int a) {
    return globvar + a;
}
//  clang -emit-llvm -c -S add.c
```

add.ll

```ll
; ModuleID = 'add.c'
source_filename = "add.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@globvar = dso_local global i32 12, align 4

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @add(i32 noundef %0) #0 {
  %2 = alloca i32, align 4
  store i32 %0, ptr %2, align 4
  %3 = load i32, ptr @globvar, align 4
  %4 = load i32, ptr %2, align 4
  %5 = add nsw i32 %3, %4
  ret i32 %5
}

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"clang version 15.0.7 (git@github.com:llvm/llvm-project.git 8dfdcc7b7bf66834a761bd8de445840ef68e4d1a)"}

```

ModuleID,它为 add.c 文件定义 LLVM 模块;

一个 LLVM 模块是顶层数据结构,包含输入的 LLVM IR 文件的全部内容。它包含函数,全局变量,外部函数原型,符号表项。

目标 triple 告
诉我们,目标是 x86_64 处理器,运行在 Linux 系统上。字符串 datalayout 告诉我们使用什么机器字节顺序(‘e’表示小端字节顺序),什么名字粉碎(name mangling)方式(m : e 表示 elf 类型)。每项设置以’-’分隔,i64:64 表示 64 位整数,长度是 64 位。

**LLVM IR 中,全局变量都以’@’开头,局部变量都以’%’开头。**

LLVM 把全局变量当作指针,因此它用 load 指令加载数据显式解引用全局变量。类似地,它用显式 store 指令为全局变量存储数据。

**局部变量**

寄存器分配的局部变量:它们是临时存储单元和分配的虚拟寄存器。虚拟寄存器在代码生成阶段被分配为物理寄存器,这会在后续章节讲解。可以用新的符号生成这类局部变量,例如
%1 = some value

堆栈分配的局部变量:它们是用 alloca 指令在当前执行函数的堆栈帧上分配的变量。alloca 指令产生指针数据类型,必须显式使用 load 指令对它读取数据,使用 store 指令对它写入数据。



define i32 @add(i32 %a),这和 C 语言声明函数非常类似。它指定函数返回整数类型 i32,有一个整数参数。函数名字也以’@’开头,说明它是全局可见的。

