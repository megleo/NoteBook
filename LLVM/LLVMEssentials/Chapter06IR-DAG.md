## 第六章 中间表示到Selection DAG

经过一系列的分析和转换 Pass,中间表示最终成为最优化的机器无关代码。然而,它依然是实际机器代码的抽象表示。编译器必须为目标架构生成可执行的机器代码。

LLVM利用DAG，即一种有向无环图表示，实现代码生成。

LLVM 利用 DAG,即一种有向无环图表示,实现代码生成。它的思路是,将中间表示(IR)变换为 Selection DAG,经历一系列的阶段,包括 DAG 结合,合法化,指令选择,指令调度等,最后分配寄存器并输出机器代码。

### 中间代码变换到Selection DAG

```shell
define i32 @test(i32 %a, i32 %b, i32 %c) {
%add = add nsw i32 %a, %b
%div = sdiv i32 %add, %c
ret i32 %div
}
```

LLVM 提供了接口 SelectionDAGBuilder,为 IR 指令创建相应的 DAG 节点。调用：

SelectionDAGBuilder::visit(unsigned Opcode, const User &I)

根据操作码(这里是 Add),将调用相应的访问函数。在这个例子中,将调用 visitAdd(),

它进而调用 visitBinary()函数。visitBinary()函数如下:

SelectionDAGBuilder::visitBinary(const User &I, unsigned OpCode)

### 合法化Selection DAG

整个处理过程并未涉及任何目标架构的信息,而我们是在为它生成代码。对于给定的目标架构,一个 DAG 节点可能是不合法的。例如,X86 架构不支持 sdiv 指令。