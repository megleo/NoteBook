# Chapter 04 Basic IR Transformations

很多的优化是发生在IR。

1. IR is retargetable and the same set of the optimizations would be valid for a number of targets.It reduces the effort of writring the same optimizations for every target. they happend in DAG level.
2. LLVM IR is in SSA form.

## Opt Tool

Opt 是LLVM的优化器和分析器，在LLVM IR上运行去分析和优化IR。

优化等级：O0（优化粒度最小），O1，O2（优化粒度最大）,Oz or Os (deal with space optimization)