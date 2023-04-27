#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"

#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include <iostream>

/*

# Compute the x'th fibonacci number.
def fib(x)
  if x < 3 then
    1
  else
    fib(x-1)+fib(x-2)

# This expression will compute the 40th number.
fib(40)

*/


using namespace llvm;

// Lexer    词法分析
/*
    identifier  :   标识符
    def         ：  函数定义
    extern      :   外部引用
*/

enum Token {
    // end
    tok_eof = -1,
    
    // command
    tok_def = -2,
    tok_extern = -3,

    // primary
    tok_identifier = -4,
    tok_number = -5
};

static std::string IdentifierStr;       // 当tok_identifier时，Identifier负责吸收该值
static double NumVal;                   // 当tok_number时，NumVal负责吸收该值

/**********************************************************************************************************/
/// gettok() 从标准输入中，返回下一个token.
static int gettok() {
    static int LastChar = ' ';

    // 越过所有的空格键
    while (isspace(LastChar))
        LastChar = getchar();
    
    if (isalpha(LastChar)) { /// 这是判断第一个标识符的第一位： 必须是大小写字母
        /// 标识符： [a-zA-Z][a-zA-Z0-9]*
        IdentifierStr += LastChar;
        while(isalnum(LastChar = getchar())) {
            IdentifierStr += LastChar;
        }

    if (IdentifierStr == "def") 
        return tok_def;
    if (IdentifierStr == "extern")
        return tok_extern;
    return tok_identifier;
    }

    /// Number [0-9.]*
    if (isdigit(LastChar)) {
        std::string NumStr;
        do
        {
            NumStr += LastChar;
            LastChar = getchar();

        } while (isdigit(LastChar) || LastChar == '.');
        NumVal = strtod(NumStr.c_str(), nullptr);
        return tok_number;
    }

    /// command until end of line
    if (LastChar == '#')
    {
        do
        {
            LastChar = getchar();
        } while (LastChar != EOF && LastChar != '\n' && LastChar !='\r');

        if (LastChar != EOF)
            return gettok();
    }

    if (LastChar == EOF)
        return tok_eof;
    
    /// 其他情况，直接返回这个char的ASSII码
    int ThisChar = LastChar;
    LastChar = getchar();
    return ThisChar;
}


/// Abstract Syntax Tree 抽象语法树
namespace {

/// ExprAST 基类
class ExprAST {
public:
    virtual ~ExprAST() = default;

    virtual Value *codegen() = 0;
};

/// NumberExprAST - 数字文字的表达式类，如“1.0”
class NumberExprAST : public ExprAST {
private:
    double Val;
public:
    NumberExprAST(double Val) : Val(Val) {
        std::cout<<"NumberExprAST: Val = "<< Val << std::endl;
    };
    Value *codegen() override;
};

/// VariableExprAST - 变量的表达式类， 如 “a”
class VariableExprAST : public ExprAST {
private:
    std::string Name;

public:
    VariableExprAST(const std::string &Name) : Name(Name) {
        std::cout<<"VariableExprAST: Name = "<< Name << std::endl;
    }

    Value *codegen() override;
};

/// BinaryExprAST - Expression class for a binary operator.
class BinaryExprAST : public ExprAST {
private:
    char Op;
    std::unique_ptr<ExprAST> LHS, RHS;

public:
    BinaryExprAST(char Op,
                    std::unique_ptr<ExprAST> LHS,
                    std::unique_ptr<ExprAST> RHS) : Op(Op), LHS(std::move(LHS)), RHS(std::move(RHS)){
                        std::cout<<"BinaryExprAST: Op ="<< Op << std::endl;
                    }
    
    Value *codegen() override;
};

/// CallExprAST - 函数调用的表达式类
class CallExprAST : public ExprAST {
private:
    std::string Callee;
    std::vector<std::unique_ptr<ExprAST>> Args;

public:
    CallExprAST(const std::string &Callee,
                std::vector<std::unique_ptr<ExprAST>> Args) : Callee(Callee), Args(std::move(Args)) {
                    std::cout<<"CallExprAST: Callee= "<< Callee << std::endl;
                }
    Value *codegen() override;
};

/// PrototypeAST 此类表示函数的“原型”，它捕获其名称及其参数名称（因此隐式地表示函数采用的参数数量）。
class PrototypeAST {
    std::string Name;
    std::vector<std::string> Args;

public:
    PrototypeAST(const std::string &Name, std::vector<std::string> Args)
        : Name(Name), Args(std::move(Args)) {
        std::cout<<"PrototypeAST: Name ="<< Name << std::endl;
        for (int i = 0; i < Args.size(); ++i) {
            std::cout<<"Args[ =" << i << "]" << Args[i] << std::endl;
        }
        }
    Function *codegen();
    const std::string &getName() const {return Name;}
};

/// FunctionAST - 用于表示函数定义的类
class FunctionAST{
    std::unique_ptr<PrototypeAST> Proto;
    std::unique_ptr<ExprAST> Body;

public:
    FunctionAST(std::unique_ptr<PrototypeAST> Proto,
                std::unique_ptr<ExprAST> Body) : Proto(std::move(Proto)), Body(std::move(Body)) {
                    std::cout<<"FunctionAST: Name =" << std::endl;
                }
    
    Function *codegen();
};
} /// end of namespace


/*                                                      Parser                                  */
/*
CurTok/getNextToken - 提供一个简单的令牌缓冲区。 
CurTok 是解析器正在查看的当前标记。 
getNextToken 从词法分析器读取另一个标记并用其结果更新 CurTok。
*/

static int CurTok;
static int getNextToken() {return CurTok = gettok();}

/// BinopPrecedence - 这包含定义的每个二元运算符的优先级
static std::map<char, int> BinopPrecedence;

/// GetTokPrecedence - 获取挂起的二元运算符令牌的优先级。
static int GetTokPrecedence() {
    if (!isascii(CurTok))
        return -1;
    
    // 确保它是已声明的 binop。
    int TokPrec = BinopPrecedence[CurTok];
    if (TokPrec <= 0)
    {
        return -1;
    }
    return TokPrec;
}

/// LogError* - These are little helper functions for error handling.
std::unique_ptr<ExprAST> LogError(const char *Str) {
  fprintf(stderr, "Error: %s\n", Str);
  return nullptr;
}

std::unique_ptr<PrototypeAST> LogErrorP(const char *Str) {
  LogError(Str);
  return nullptr;
}

static std::unique_ptr<ExprAST> ParseExpression();

/// numberexpr ::= number
static std::unique_ptr<ExprAST> ParseNumberExpr() {
    auto Result = std::make_unique<NumberExprAST>(NumVal);
    getNextToken();
    return std::move(Result);
}

/// parsnexpr ::= '(' expression ')'
static std::unique_ptr<ExprAST> ParseParenExpr() {
    getNextToken(); // eta '('
    auto V = ParseExpression();
    if (!V)
        return nullptr;
    
    if (CurTok != ')')
        return LogError("expected ')'");
    
    getNextToken(); // eta ')'
    return V;
}

/// identifierexpr
/// :: identifier
/// :: identifier '(' expression ')'
static std::unique_ptr<ExprAST> ParseIdentifierExpr() {
    std::string IdName = IdentifierStr;
    getNextToken();     /// eat identifier 
    if (CurTok != '(')
        return std::make_unique<VariableExprAST>(IdName);

    getNextToken(); /// eat '('
    std::vector<std::unique_ptr<VariableExprAST>> Args;
    if(CurTok != ')') {
        while (true) {
            if (auto Arg = ParseExpression())
                Args.push_back(std::move(Arg));
            else
                return nullptr;

            if (CurTok == ')')
                break;

            if (CurTok != ',')
                return LogError("Expected ')' or ',' in argument list");
        getNextToken();
        }
    }
    // eat ')'
    getNextToken();

    return std::make_unique<CallExprAST>(IdName, std::move(Args));
}

/// primary
/// :: = identiferexpr
/// :: = numberexpr
/// :: = parenexpr
static std::unique_ptr<ExprAST> ParsePrimary() {
    switch (CurTok)
    {
    default:
        return LogError("unknow token when expecting an expression.")
    case tok_identifier:
        return ParseIdentifierExpr();
    case tok_number:
        return ParseNumberExpr();
    case '(':
        return ParseParenExpr();
    }
}

/// binoprhs
/// ::= ('+' primary)*
static std::unique_ptr<ExprAST> ParseBinOpRHS(int ExprPrec,
                        std::unique_ptr<ExprAST> LHS) {
    /// if this is a binop, find its precedence.
    while (true)
    {
        int TokPrec = GetTokPrecedence();

        // if this is a binop that binds at least as tightly as the current binop, consume it , otherwise we are done.
        if (TokPrec < ExprPrec)
            return LHS;
        
        // ok, we know this is a binop.
        int BinOp = CurTok;
        getNextToken();

        // parse the primary expression after the binary operator.
        auto RHS = ParsePrimary();
        if(!RHS)
            return nullptr;
        
        // if binop binds less tightly with RHS than the operator after RHS, let the pending operator take RHS as its LHS
        int NextPrec = GetTokPrecedence();
        if(TokPrec < NextPrec) {
            RHS = ParseBinOpRHS(TokPrec + 1, std::move(RHS));
            if (!RHS)
                return nullptr;            
        }

        // Merge LHS/RHS
        LHS = std::make_unique<BinaryExprAST>(BinOp, std::move(LHS), std::move(RHS));
    }
}

/// expression
/// ::= primary binoprhs
///
static std::unique_ptr<ExprAST> ParseExpression() {
    auto LHS = ParsePrimary();
    if(!LHS)
        return nullptr;
    return ParseBinOpRHS(0, std::move(LHS));
}

/// prototype
/// ::= id '(' id* ')'
static std::unique_ptr<PrototypeAST> ParsePrototype() {
    if (CurTok != tok_identifier)
        return LogErrorP("Expected function name in prototype.");

    std::string FnName = IdentifierStr;
    getNextToken();

    if (CurTok != '(')
        return LogErrorP("Expected '(' in prototype");
    
    std::vector<std::string> ArgNames;
    while (getNextToken() == tok_identifier)
        ArgNames.push_back(IdentifierStr);
    
    if (CurTok != ')')
        return LogErrorP("Excepted '(' in prototype.");

    /// success
    getNextToken(); // eat ')'

    return std::make_unique<PrototypeAST>(FnName, std::move(ArgNames));    
}

/// definition ::= 'def' prototype expression 
static std::unique_ptr<FunctionAST> ParseDefinition() {
    getNextToken(); // eat def
    auto Proto = ParsePrototype();
    if (!Proto)
        return nullptr;
    
    if (auto E = ParseExpression())
        return std::make_unique<FunctionAST>(std::move(Proto), std::move(E));
    return nullptr;
}

/// toplevelexpr :: = expression
static std::unique_ptr<FunctionAST> ParseTopLeverExpr() {
    if (auto E = ParseExpression()) {
        // make an anonymous proto
        auto Proto = std::make_unique<PrototypeAST>("__anon_expr",
                                                    std::vector<std::string>());
        return std::make_unique<FunctionAST>(std::move(Proto), std::move(E));
    }
    return nullptr;
}

/// external ::= 'extern' prototype
static std::unique_ptr<PrototypeAST> ParseExtern() {
    getNextToken();
    return ParsePrototype();
}

//===----------------------------------------------------------------------===//
// Code Generation
//===----------------------------------------------------------------------===//

static std::unique_ptr<LLVMContext> TheContext;
static std::unique_ptr<Module> TheModule;
static std::unique_ptr<IRBuilder<>> Builder;
static std::map<std::string,Value*> NamedValues;
static std::unique_ptr<legacy::FunctionPassManager> TheFPM;
static std::unique_ptr<KaleidoscopeJIT> TheJIT;
static std::map<std::string, std::unique_ptr<PrototypeAST>> FunctionProtos;
static ExitOnError ExitOnErr;

Value *LogErrorV(const char *Str) {
    LogError(Str);
    return nullptr;
}

Value* NumberExprAST::codegen() {
    return ConstantFP::get(*TheContext, APFloat(Val));
}

Value* VariableExprAST::codegen() {
    Value* V = NamedValues[Name];
    if (!V)
        return LogErrorV("Unknown variable name.");
    return V;
}

Value *BinaryExprAST::codegen() {
    Value *L = LHS->codegen();
    Value *R = RHS->codegen();
    if(!L || !R)
        return nullptr;
    
    switch (Op) {
    case '+':
        return Builder->CreateFAdd(L, R, "addtmp");
    case '-':
        return Builder->CreateFSub(L, R, "subtmp");
    case '*':
        return Builder->CreateFMul(L, R, "multmp");
    case '<':
        L = Builder->CreateFCmpULT(L, R, "cmptmp");
        return Builder->CreateUIToFP(L, Type::getDoubleTy(*TheContext), "booltmp");
    default:
        return LogErrorV("invalid binary operator");
    }
}

Value *CallExprAST::codegen() {
    //look up the name in the global module table
    Function *CalleeF = TheModule->getFunction(Callee);

    // if argument mismatch error
    if (CalleeF->arg_size() != Args.size())
        return LogErrorV("Incorrect #arguments passed");

    std::vector<Value *> ArgsV;
    for (unsigned i = 0, e = Args.size(); i != e; ++i) {
        ArgsV.push_back(Args[i]->codegen());
        if(!ArgsV.back())
            return nullptr;
    }

    return Builder->CreateCall(CalleeF, ArgsV, "calltmp");
}

Function *PrototypeAST::codegen() {
    // Make the function type: double(double, double) etc.
    std::vector<Type*> Doubles(Args.size(), Type::getDoubleTy(*TheContext));
    FunctionType *FT = 
        FunctionType::get(Type::getDoubleTy(*TheContext), Doubles, false);
    
    Function *F = 
        Function::Create(FT, Function::ExternalLinkage, Name, TheModule.get());
    
    // set names for all arguments
    unsigned Idx = 0;
    for (auto &Arg : F->args())
        Arg.setName(Args[Idx++]);
    return F;    
}

Function *FunctionAST::codegen() {
    // First, check for an existing function from a previous 'extern' declaration
    Function *TheFunction = TheModule->getFunction(Proto->getName());
    if(!TheFunction)
        TheFunction = Proto->codegen();
    
    if(!TheFunction)
        return nullptr;
    
    if(!TheFunction->empty())
        return (Function*) LogErrorV("Function cannot be redefined.");

    // Create a new basic block to start insertion into
    BasicBlock **BB = BasicBlock::Create(*TheContext, "entry", TheFunction);
    Builder->SetInsertPoint(BB);

    // Record the function arguments in the NamedValues map
    NamedValues.clear();
    for (auto &Arg : v)
    {
        
    }
    

}

int main() {
    BinopPrecedence['<'] = 10;
    BinopPrecedence['+'] = 20;
    BinopPrecedence['-'] = 20;
    BinopPrecedence['*'] = 80;
    fprintf(stderr, "ready> ");
    getNextToken();

    // make the module, which holds all the code.

    return 0;
}

