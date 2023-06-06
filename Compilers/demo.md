## Flex 部分

## Bison 部分

### %define api.token.prefix {TOK_}

> 指令为记号类型添加了一个前缀`TOK_`，用于区分不同的记号类型。

### %type

> 指令用来定义非终结符记号，即语法分析树上的节点类型。在这个例子中，这两个指令定义了所有可能出现的记号和节点类型，以及它们的语义含义。

```bison
%type <SyntaxTree::Assembly*>CompUnit
%type <SyntaxTree::PtrList<SyntaxTree::GlobalDef>>GlobalDecl
%type <SyntaxTree::Type>BType
%type <SyntaxTree::PtrList<SyntaxTree::VarDef>>VarDecl
%type <SyntaxTree::PtrList<SyntaxTree::VarDef>>VarDefList
%type <SyntaxTree::VarDef*>VarDef
%type <SyntaxTree::InitVal*>InitVal
%type <SyntaxTree::FuncDef*>FuncDef
%type <SyntaxTree::BlockStmt*>Block
%type <SyntaxTree::PtrList<SyntaxTree::Stmt>>BlockItemList
%type <SyntaxTree::PtrList<SyntaxTree::Stmt>>BlockItem
%type <SyntaxTree::Stmt*>Stmt
%type <SyntaxTree::LVal*>LVal
%type <SyntaxTree::Expr*>Exp
%type <SyntaxTree::Literal*>Number
```

### %token

> 指令用来定义终结符记号，即词法分析器生成的记号类型，用于构造语法分析树; 包括一些算术运算符、赋值符号、分号、括号、关键字等等。每个记号都有一个唯一的记号类型，以及它对应的语义含义。

```bison
%token END
%token PLUS MINUS MULTIPLY DIVIDE MODULO
%token ASSIGN SEMICOLON
%token COMMA LPARENTHESE RPARENTHESE
%token LBRACE RBRACE
%token INT RETURN VOID
%token <std::string>IDENTIFIER
%token <int>INTCONST
%token EOL COMMENT
%token BLANK NOT
```

###  @\$

> `@$`是一个特殊的bison符号，用于表示当前规约动作所对应的位置信息。在bison代码中，`$1->loc = @$.`这一行代码的作用是将当前规约动作的位置信息赋值给语法分析树的一个节点的位置信息。

```bison
$1->loc = @$;
```

### \$\$

> `$$`表示当前规约动作的结果，也就是`CompUnit`节点的生成结果。

```bison
CompUnit: GlobalDecl{
		$$=new SyntaxTree::Assembly();
		$$->global_defs.insert($$->global_defs.end(), $1.begin(), $1.end());
  }
	;
```

> 这个语法定义了CompUnit节点的产生式，它是由GlobalDecl节点构成。具体而言，`$$=new SyntaxTree::Assembly();`创建了一个新的`SyntaxTree::Assembly`类型的节点，即`CompUnit`节点。
