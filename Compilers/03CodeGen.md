# 03 Code generation to LLVM IR

主要是讲述如何将抽象语法树转换成llvm IR 



在每个 AST 类中定义虚拟代码生成（codegen）方法; codegen() 方法表示为该 AST 节点发出 IR 以及它所依赖的所有事物，它们都返回一个 LLVM Value 对象。



```c++
/// ExprAST - Base class for all expression nodes.
class ExprAST {
public:
  virtual ~ExprAST() = default;
  virtual Value *codegen() = 0;
};

/// NumberExprAST - Expression class for numeric literals like "1.0".
class NumberExprAST : public ExprAST {
  double Val;

public:
  NumberExprAST(double Val) : Val(Val) {}
  Value *codegen() override;
};
```

用于解析器的“LogError”方法，它将用于报告在代码生成期间发现的错误（例如，使用未声明的参数）：



```c++
static std::unique_ptr<LLVMContext> TheContext;
static std::unique_ptr<IRBuilder<>> Builder(TheContext);
static std::unique_ptr<Module> TheModule;
static std::map<std::string, Value *> NamedValues;

Value *LogErrorV(const char *Str) {
  LogError(Str);
  return nullptr;
}
```

















































