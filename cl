  -FunctionDecl main  int()
|   -CompoundStmt   
|    -DeclStmt   
|     -VarDecl t  int
|     -VarDecl m  int
|    -DeclStmt   
|     -VarDecl a  int
|     -VarDecl b  int
|    -BinaryOperator   int
|     -DeclRefExpr t  int
|     -IntegerLiteral  1 int
|    -WhileStmt   
|     -DeclRefExpr t  int
|     -CompoundStmt   
|      -BinaryOperator   int
|       -DeclRefExpr t  int
|       -BinaryOperator   int
|        -ImplicitCastExpr   int LValueToRValue
|         -DeclRefExpr t  int
|        -IntegerLiteral  1 int
|      -BinaryOperator   int
|       -DeclRefExpr n  int
|       -IntegerLiteral  4 int
|      -BinaryOperator   int
|       -DeclRefExpr m  int
|       -IntegerLiteral  10 int
|      -DeclStmt   
|       -VarDecl i  int
|        -IntegerLiteral  0 int
|      -DeclStmt   
|       -VarDecl flag  int
|        -IntegerLiteral  0 int
|      -CallExpr   void
|       -ImplicitCastExpr   void FunctionToPointerDecay
|        -DeclRefExpr init  void
|       -ImplicitCastExpr   int LValueToRValue
|        -DeclRefExpr n  int
|      -DeclStmt   
|       -VarDecl k  int
|        -BinaryOperator   int
|         -BinaryOperator   int
|          -ImplicitCastExpr   int LValueToRValue
|           -DeclRefExpr n  int
|          -ImplicitCastExpr   int LValueToRValue
|           -DeclRefExpr n  int
|         -IntegerLiteral  1 int
