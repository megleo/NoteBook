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

### 优化Selection DAG	

LLVM IR 变换为 Selection DAG 之后,可能出现很多优化 DAG 自身的机会。这些优化发生在 DAGCombiner 阶段。这些机会出现的原因在于架构特定的指令集。

```c++
\#include <arm_neon.h> 

unsigned hadd(uint32x4_t a) { 
return a[0] + a[1] + a[2] + a[3]; 
} 
```

它的 LLVM IR 是这样的： 

```shell
define i32 @hadd(<4 x i32> %a) nounwind { 
%vecext = extractelement <4 x i32> %a, i32 3 
%vecext1 = extractelement <4 x i32> %a, i32 2 
%add = add i32 %vecext, %vecext1 
%vecext2 = extractelement <4 x i32> %a, i32 1 
%add3 = add i32 %add, %vecext2
%vecext4 = extractelement <4 x i32> %a, i32 0
%add5 = add i32 %add3, %vecext4
ret i32 %add5 }
```

**上面的过程是一个一个元素增加的，但是对于ARM高级架构，它是可以进行单向量整体加。 为了使用这条指令，需要在Selection DAG中时别这种模式。将这些DAG结合为单个DAG节点。**

这可以在 AArch64DAGToDAGISel 作指令选择时实现。 

```shell
SDNode *AArch64DAGToDAGISel::Select(SDNode *Node) 
{ ... ...case ISD::ADD: if (SDNode *I = SelectMLAV64LaneV128(Node)) 
return I;
if (SDNode *I = SelectADDV(Node)) return I; break; }
```

### 指令选择

Selection DAG 在这个阶段已经被优化和合法化。然而，指令依然不是机器代码形式。这些指令自身需要在 Selection DAG 中被映射为架构特定的指令。TableGen 类帮助选择目标特定的指令。

CodeGenAndEmitDAG()函数调用 DoInstructionSelection()函数，它访问每个 DAG 节点，为它们调用 Select()函数。Select()函数是挂钩函数，指向特定实现，用以选 择节点。它是虚函数，每个目标会实现它。

### 调度和输出机器指令

**目的：将DAG转换成机器指令。**

第一步： 指令调度。线性化DAG。输出指令序列到 MachineBasicBlock。这是由 Scheduler 完成的。

### 寄存器分配

在 LLVM 中，虚拟寄存器的 数量是无限的，而物理寄存器的数量是有限的，根据目标架构确定。因此，通过寄存器分配， 我们的目的是将最多的物理寄存器分配给虚拟寄存器。我们必须明白物理寄存器数量有限， 不总是能够为所有虚拟寄存器分配到物理寄存器。如果在某个点一个变量 V 分配不到可用 的物理寄存器，我们就需要将一个已分配物理寄存器的变量移出到内存，释放这个物理寄存器给变量 V 使用。这个过程，即将变量从物理寄存器移出到内存，称为 spilling。

寄存器分配的另一个重要的任务是解构 SSA 形式。至此在机器指令中的 Phi 指令需要 被替换为常规指令。传统的做法是把它替换为复制指令。

LLVM phi指令是LLVM编译器基础设施中的一种指令，用于实现基本块之间的值传递。

Version:0.9 StartHTML:0000000105 EndHTML:0000000354 StartFragment:0000000141 EndFragment:0000000314

**寄存器分配映射虚拟寄存器到物理寄存器**

- 直接映射

  在合适的位置插入 load 指令从内存读取变量，或者插入 store 指令存储变量到内存。

- 间接映射

  它用 VirtRegMap 类实现插入 load 和 store 指令。它同样需要从内存读取 

  变量，或者存储变量到内存。它用 VirtRegMap::assignVirt2Phys(vreg, preg)函数 

  来映射虚拟寄存器到物理寄存器。



在本章中，我们见识了 LLVM IR 是如何变换为 SelectionDAG 的。随后，SDAG 经历 了多种转换。指令和数据类型被合法化。SelectionDAG 也经历了优化阶段，DAG 节点被 结合为优化的节点。这可能是目标特定的。结合 DAG 之后，指令选择阶段将目标架构指令 映射为 DAG 节点。然后，按线性顺序排列 DAG，使得 CPU 执行效率最高。这些 DAG 被 变换为 MachineInstr，随后它们被销毁。然后，为代码中的所有虚拟寄存器分配物理寄存器。接着，MC 层出场，它用于生成目标代码和汇编代码。进