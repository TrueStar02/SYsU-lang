|      -BinaryOperator  
|       -ArraySubscriptExpr  
|        -ImplicitCastExpr  
|         -ArraySubscriptExpr  
|          -ImplicitCastExpr  
|           -DeclRefExpr arr1 
|          -ImplicitCastExpr  
|           -DeclRefExpr x 
|        -IntegerLiteral  0
|       -ArraySubscriptExpr  
|        -ImplicitCastExpr  
|         -ArraySubscriptExpr  
|          -ImplicitCastExpr  
|           -DeclRefExpr arr1 
|          -ImplicitCastExpr  
|           -DeclRefExpr x 
|        -IntegerLiteral  1
| -FunctionDecl main 
|  -CompoundStmt  
|   -DeclStmt  
|    -VarDecl arr 
|     -InitListExpr  
|      -InitListExpr  
|       -IntegerLiteral  3
|       -IntegerLiteral  7
|       -IntegerLiteral  0
|      -InitListExpr  
|       -IntegerLiteral  5
|       -IntegerLiteral  0
|       -IntegerLiteral  0
|      -InitListExpr  
|       -IntegerLiteral  8
|       -IntegerLiteral  1
|       -IntegerLiteral  0
|      -InitListExpr  
|       -IntegerLiteral  4
|       -IntegerLiteral  6
|       -IntegerLiteral  0
|      -InitListExpr  
|       -IntegerLiteral  9
|       -IntegerLiteral  2
|       -IntegerLiteral  0
|   -DeclStmt  
|    -VarDecl y 
|     -IntegerLiteral  1
|   -CallExpr  
|    -ImplicitCastExpr  
|     -DeclRefExpr add 
|    -ImplicitCastExpr  
|     -DeclRefExpr arr 
|    -IntegerLiteral  5
|   -ReturnStmt  
|    -ImplicitCastExpr  
|     -DeclRefExpr y 