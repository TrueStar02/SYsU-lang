| -TranslationUnitDecl   
|  -FunctionDecl _sysy_starttime  void(int)
|   -ParmVarDecl lineno  int
|  -FunctionDecl _sysy_stoptime  void(int)
|   -ParmVarDecl lineno  int
|  -FunctionDecl _sysy_getch  int()
|  -FunctionDecl _sysy_putch  void(int)
|   -ParmVarDecl a  int
|  -FunctionDecl _sysy_getint  int()
|  -FunctionDecl _sysy_putint  void(int)
|   -ParmVarDecl a  int
|  -FunctionDecl _sysy_getarray  int(int)
|   -ParmVarDecl a  int
|  -FunctionDecl _sysy_putarray  void(int,int)
|   -ParmVarDecl n  int
|   -ParmVarDecl a  int
|  -FunctionDecl _sysy_getfloat  float()
|  -FunctionDecl _sysy_putfloat  void(float)
|   -ParmVarDecl a  float
|  -FunctionDecl _sysy_getfarray  int(float)
|   -ParmVarDecl a  float
|  -FunctionDecl _sysy_putfarray  void(int,float)
|   -ParmVarDecl n  int
|   -ParmVarDecl a  float
|  -FunctionDecl _sysy_putf  void(char)
|   -ParmVarDecl a  char
|  -VarDecl RADIUS  const float
|   -ImplicitCastExpr   const float FloatingCast
|    -FloatingLiteral  5.5 double
|  -VarDecl PI  const float
|   -ImplicitCastExpr   const float FloatingCast
|    -FloatingLiteral  3.141592653589793 double
|  -VarDecl EPS  const float
|   -ImplicitCastExpr   const float FloatingCast
|    -FloatingLiteral  1e-06 double
|  -VarDecl PI_HEX  const float
|   -ImplicitCastExpr   const float FloatingCast
|    -FloatingLiteral  3.141592741012573 double
|  -VarDecl HEX2  const float
|   -ImplicitCastExpr   const float FloatingCast
|    -FloatingLiteral  0.078125 double
|  -VarDecl FACT  const float
|   -ImplicitCastExpr   const float FloatingCast
|    -UnaryOperator   double
|     -FloatingLiteral  33000 double
|  -VarDecl EVAL1  const float
|   -BinaryOperator   const float
|    -BinaryOperator   const float
|     -ImplicitCastExpr   const float LValueToRValue
|      -DeclRefExpr PI  const float
|     -ImplicitCastExpr   const float LValueToRValue
|      -DeclRefExpr RADIUS  const float
|    -ImplicitCastExpr   const float LValueToRValue
|     -DeclRefExpr RADIUS  const float
|  -VarDecl EVAL2  const float
|   -BinaryOperator   const float
|    -BinaryOperator   const float
|     -ImplicitCastExpr   const float IntegralToFloating
|      -IntegerLiteral  2 int
|     -ImplicitCastExpr   const float LValueToRValue
|      -DeclRefExpr PI_HEX  const float
|    -ImplicitCastExpr   const float LValueToRValue
|     -DeclRefExpr RADIUS  const float
|  -VarDecl EVAL3  const float
|   -BinaryOperator   const float
|    -BinaryOperator   const float
|     -ImplicitCastExpr   const float LValueToRValue
|      -DeclRefExpr PI  const float
|     -ImplicitCastExpr   const float IntegralToFloating
|      -IntegerLiteral  2 int
|    -ImplicitCastExpr   const float LValueToRValue
|     -DeclRefExpr RADIUS  const float
|  -VarDecl CONV1  const float
|   -ImplicitCastExpr   const float IntegralToFloating
|    -IntegerLiteral  233 int
|  -VarDecl CONV2  const float
|   -ImplicitCastExpr   const float IntegralToFloating
|    -IntegerLiteral  4095 int
|  -VarDecl MAX  const int
|   -ImplicitCastExpr   const int FloatingToIntegral
|    -FloatingLiteral  1000000000 double
|  -VarDecl TWO  const int
|   -ImplicitCastExpr   const int FloatingToIntegral
|    -FloatingLiteral  2.9 double
|  -VarDecl THREE  const int
|   -ImplicitCastExpr   const int FloatingToIntegral
|    -FloatingLiteral  3.2 double
|  -VarDecl FIVE  const int
|   -BinaryOperator   const int
|    -ImplicitCastExpr   const int LValueToRValue
|     -DeclRefExpr TWO  const int
|    -ImplicitCastExpr   const int LValueToRValue
|     -DeclRefExpr THREE  const int
|  -FunctionDecl float_abs  float(float)
|   -ParmVarDecl x  float
|   -CompoundStmt   
|    -IfStmt   
|     -BinaryOperator   float
|      -ImplicitCastExpr   float LValueToRValue
|       -DeclRefExpr x  float
|      -ImplicitCastExpr   float IntegralToFloating
|       -IntegerLiteral  0 int
|     -ReturnStmt   
|      -UnaryOperator   float
|       -ImplicitCastExpr   float LValueToRValue
|        -DeclRefExpr x  float
|    -ReturnStmt   
|     -ImplicitCastExpr   float LValueToRValue
|      -DeclRefExpr x  float
|  -FunctionDecl circle_area  float(int)
|   -ParmVarDecl radius  int
|   -CompoundStmt   
|    -ReturnStmt   
|     -BinaryOperator   const float
|      -ParenExpr   const float
|       -BinaryOperator   const float
|        -BinaryOperator   const float
|         -BinaryOperator   const float
|          -ImplicitCastExpr   const float LValueToRValue
|           -DeclRefExpr PI  const float
|          -ImplicitCastExpr   const float IntegralToFloating
|           -ImplicitCastExpr   int LValueToRValue
|            -DeclRefExpr radius  int
|         -ImplicitCastExpr   const float IntegralToFloating
|          -ImplicitCastExpr   int LValueToRValue
|           -DeclRefExpr radius  int
|        -BinaryOperator   const float
|         -ImplicitCastExpr   const float IntegralToFloating
|          -ParenExpr   int
|           -BinaryOperator   int
|            -ImplicitCastExpr   int LValueToRValue
|             -DeclRefExpr radius  int
|            -ImplicitCastExpr   int LValueToRValue
|             -DeclRefExpr radius  int
|         -ImplicitCastExpr   const float LValueToRValue
|          -DeclRefExpr PI  const float
|      -ImplicitCastExpr   const float IntegralToFloating
|       -IntegerLiteral  2 int
|  -FunctionDecl float_eq  int(float,float)
|   -ParmVarDecl a  float
|   -ParmVarDecl b  float
|   -CompoundStmt   
|    -IfStmt   
|     -BinaryOperator   float
|      -CallExpr   float
|       -ImplicitCastExpr   float FunctionToPointerDecay
|        -DeclRefExpr float_abs  float
|       -BinaryOperator   float
|        -ImplicitCastExpr   float LValueToRValue
|         -DeclRefExpr a  float
|        -ImplicitCastExpr   float LValueToRValue
|         -DeclRefExpr b  float
|      -ImplicitCastExpr   const float LValueToRValue
|       -DeclRefExpr EPS  const float
|     -CompoundStmt   
|      -ReturnStmt   
|       -ImplicitCastExpr   int FloatingToIntegral
|        -BinaryOperator   double
|         -BinaryOperator   double
|          -ImplicitCastExpr   double IntegralToFloating
|           -IntegerLiteral  1 int
|          -FloatingLiteral  2 double
|         -ImplicitCastExpr   double IntegralToFloating
|          -IntegerLiteral  2 int
|     -CompoundStmt   
|      -ReturnStmt   
|       -IntegerLiteral  0 int
|  -FunctionDecl error  void()
|   -CompoundStmt   
|    -CallExpr   void
|     -ImplicitCastExpr   void FunctionToPointerDecay
|      -DeclRefExpr _sysy_putch  void
|     -IntegerLiteral  101 int
|    -CallExpr   void
|     -ImplicitCastExpr   void FunctionToPointerDecay
|      -DeclRefExpr _sysy_putch  void
|     -IntegerLiteral  114 int
|    -CallExpr   void
|     -ImplicitCastExpr   void FunctionToPointerDecay
|      -DeclRefExpr _sysy_putch  void
|     -IntegerLiteral  114 int
|    -CallExpr   void
|     -ImplicitCastExpr   void FunctionToPointerDecay
|      -DeclRefExpr _sysy_putch  void
|     -IntegerLiteral  111 int
|    -CallExpr   void
|     -ImplicitCastExpr   void FunctionToPointerDecay
|      -DeclRefExpr _sysy_putch  void
|     -IntegerLiteral  114 int
|    -CallExpr   void
|     -ImplicitCastExpr   void FunctionToPointerDecay
|      -DeclRefExpr _sysy_putch  void
|     -IntegerLiteral  10 int
|  -FunctionDecl ok  void()
|   -CompoundStmt   
|    -CallExpr   void
|     -ImplicitCastExpr   void FunctionToPointerDecay
|      -DeclRefExpr _sysy_putch  void
|     -IntegerLiteral  111 int
|    -CallExpr   void
|     -ImplicitCastExpr   void FunctionToPointerDecay
|      -DeclRefExpr _sysy_putch  void
|     -IntegerLiteral  107 int
|    -CallExpr   void
|     -ImplicitCastExpr   void FunctionToPointerDecay
|      -DeclRefExpr _sysy_putch  void
|     -IntegerLiteral  10 int
|  -FunctionDecl assert  void(int)
|   -ParmVarDecl cond  int
|   -CompoundStmt   
|    -IfStmt   
|     -UnaryOperator   int
|      -ImplicitCastExpr   int LValueToRValue
|       -DeclRefExpr cond  int
|     -CompoundStmt   
|      -CallExpr   void
|       -ImplicitCastExpr   void FunctionToPointerDecay
|        -DeclRefExpr error  void
|     -CompoundStmt   
|      -CallExpr   void
|       -ImplicitCastExpr   void FunctionToPointerDecay
|        -DeclRefExpr ok  void
|  -FunctionDecl assert_not  void(int)
|   -ParmVarDecl cond  int
|   -CompoundStmt   
|    -IfStmt   
|     -ImplicitCastExpr   int LValueToRValue
|      -DeclRefExpr cond  int
|     -CompoundStmt   
|      -CallExpr   void
|       -ImplicitCastExpr   void FunctionToPointerDecay
|        -DeclRefExpr error  void
|     -CompoundStmt   
|      -CallExpr   void
|       -ImplicitCastExpr   void FunctionToPointerDecay
|        -DeclRefExpr ok  void
|  -FunctionDecl main  int()
|   -CompoundStmt   
|    -CallExpr   void
|     -ImplicitCastExpr   void FunctionToPointerDecay
|      -DeclRefExpr assert_not  void
|     -CallExpr   int
|      -ImplicitCastExpr   int FunctionToPointerDecay
|       -DeclRefExpr float_eq  int
|      -ImplicitCastExpr   const float LValueToRValue
|       -DeclRefExpr HEX2  const float
|      -ImplicitCastExpr   const float LValueToRValue
|       -DeclRefExpr FACT  const float
|    -CallExpr   void
|     -ImplicitCastExpr   void FunctionToPointerDecay
|      -DeclRefExpr assert_not  void
|     -CallExpr   int
|      -ImplicitCastExpr   int FunctionToPointerDecay
|       -DeclRefExpr float_eq  int
|      -ImplicitCastExpr   const float LValueToRValue
|       -DeclRefExpr EVAL1  const float
|      -ImplicitCastExpr   const float LValueToRValue
|       -DeclRefExpr EVAL2  const float
|    -CallExpr   void
|     -ImplicitCastExpr   void FunctionToPointerDecay
|      -DeclRefExpr assert  void
|     -CallExpr   int
|      -ImplicitCastExpr   int FunctionToPointerDecay
|       -DeclRefExpr float_eq  int
|      -ImplicitCastExpr   const float LValueToRValue
|       -DeclRefExpr EVAL2  const float
|      -ImplicitCastExpr   const float LValueToRValue
|       -DeclRefExpr EVAL3  const float
|    -CallExpr   void
|     -ImplicitCastExpr   void FunctionToPointerDecay
|      -DeclRefExpr assert  void
|     -CallExpr   int
|      -ImplicitCastExpr   int FunctionToPointerDecay
|       -DeclRefExpr float_eq  int
|      -CallExpr   float
|       -ImplicitCastExpr   float FunctionToPointerDecay
|        -DeclRefExpr circle_area  float
|       -ImplicitCastExpr   int FloatingToIntegral
|        -ImplicitCastExpr   const float LValueToRValue
|         -DeclRefExpr RADIUS  const float
|      -CallExpr   float
|       -ImplicitCastExpr   float FunctionToPointerDecay
|        -DeclRefExpr circle_area  float
|       -ImplicitCastExpr   const int LValueToRValue
|        -DeclRefExpr FIVE  const int
|    -CallExpr   void
|     -ImplicitCastExpr   void FunctionToPointerDecay
|      -DeclRefExpr assert_not  void
|     -CallExpr   int
|      -ImplicitCastExpr   int FunctionToPointerDecay
|       -DeclRefExpr float_eq  int
|      -ImplicitCastExpr   const float LValueToRValue
|       -DeclRefExpr CONV1  const float
|      -ImplicitCastExpr   const float LValueToRValue
|       -DeclRefExpr CONV2  const float
|    -IfStmt   
|     -FloatingLiteral  1.5 double
|     -CallExpr   void
|      -ImplicitCastExpr   void FunctionToPointerDecay
|       -DeclRefExpr ok  void
|    -IfStmt   
|     -UnaryOperator   double
|      -UnaryOperator   double
|       -FloatingLiteral  3.3 double
|     -CallExpr   void
|      -ImplicitCastExpr   void FunctionToPointerDecay
|       -DeclRefExpr ok  void
|    -IfStmt   
|     -BinaryOperator   int
|      -FloatingLiteral  0 double
|      -IntegerLiteral  3 int
|     -CallExpr   void
|      -ImplicitCastExpr   void FunctionToPointerDecay
|       -DeclRefExpr error  void
|    -IfStmt   
|     -BinaryOperator   int
|      -IntegerLiteral  0 int
|      -FloatingLiteral  0.3 double
|     -CallExpr   void
|      -ImplicitCastExpr   void FunctionToPointerDecay
|       -DeclRefExpr ok  void
|    -DeclStmt   
|     -VarDecl i  int
|      -IntegerLiteral  1 int
|     -VarDecl p  int
|      -IntegerLiteral  0 int
|    -DeclStmt   
|     -VarDecl arr  float[10]
|      -InitListExpr   void
|    -DeclStmt   
|     -VarDecl len  int
|      -CallExpr   int
|       -ImplicitCastExpr   int FunctionToPointerDecay
|        -DeclRefExpr _sysy_getfarray  int
|       -ImplicitCastExpr   float[10] LValueToRValue
|        -DeclRefExpr arr  float[10]
|    -WhileStmt   
|     -BinaryOperator   int
|      -ImplicitCastExpr   int LValueToRValue
|       -DeclRefExpr i  int
|      -ImplicitCastExpr   const int LValueToRValue
|       -DeclRefExpr MAX  const int
|     -CompoundStmt   
|      -DeclStmt   
|       -VarDecl input  float
|        -CallExpr   float
|         -ImplicitCastExpr   float FunctionToPointerDecay
|          -DeclRefExpr _sysy_getfloat  float
|      -DeclStmt   
|       -VarDecl area  float
|        -BinaryOperator   const float
|         -BinaryOperator   const float
|          -ImplicitCastExpr   const float LValueToRValue
|           -DeclRefExpr PI  const float
|          -ImplicitCastExpr   float LValueToRValue
|           -DeclRefExpr input  float
|         -ImplicitCastExpr   float LValueToRValue
|          -DeclRefExpr input  float
|       -VarDecl area_trunc  float
|        -CallExpr   float
|         -ImplicitCastExpr   float FunctionToPointerDecay
|          -DeclRefExpr circle_area  float
|         -ImplicitCastExpr   int FloatingToIntegral
|          -ImplicitCastExpr   float LValueToRValue
|           -DeclRefExpr input  float
|      -BinaryOperator   float
|       -ArraySubscriptExpr   float
|        -ImplicitCastExpr   float[10] ArrayToPointerDecay
|         -DeclRefExpr arr  float[10]
|        -ImplicitCastExpr   int LValueToRValue
|         -DeclRefExpr p  int
|       -BinaryOperator   float
|        -ImplicitCastExpr   float LValueToRValue
|         -ArraySubscriptExpr   float
|          -ImplicitCastExpr   float[10] ArrayToPointerDecay
|           -DeclRefExpr arr  float[10]
|          -ImplicitCastExpr   int LValueToRValue
|           -DeclRefExpr p  int
|        -ImplicitCastExpr   float LValueToRValue
|         -DeclRefExpr input  float
|      -CallExpr   void
|       -ImplicitCastExpr   void FunctionToPointerDecay
|        -DeclRefExpr _sysy_putfloat  void
|       -ImplicitCastExpr   float LValueToRValue
|        -DeclRefExpr area  float
|      -CallExpr   void
|       -ImplicitCastExpr   void FunctionToPointerDecay
|        -DeclRefExpr _sysy_putch  void
|       -IntegerLiteral  32 int
|      -CallExpr   void
|       -ImplicitCastExpr   void FunctionToPointerDecay
|        -DeclRefExpr _sysy_putint  void
|       -ImplicitCastExpr   int FloatingToIntegral
|        -ImplicitCastExpr   float LValueToRValue
|         -DeclRefExpr area_trunc  float
|      -CallExpr   void
|       -ImplicitCastExpr   void FunctionToPointerDecay
|        -DeclRefExpr _sysy_putch  void
|       -IntegerLiteral  10 int
|      -BinaryOperator   int
|       -DeclRefExpr i  int
|       -ImplicitCastExpr   int FloatingToIntegral
|        -BinaryOperator   double
|         -ImplicitCastExpr   double IntegralToFloating
|          -ImplicitCastExpr   int LValueToRValue
|           -DeclRefExpr i  int
|         -UnaryOperator   double
|          -UnaryOperator   double
|           -FloatingLiteral  10 double
|      -BinaryOperator   int
|       -DeclRefExpr p  int
|       -BinaryOperator   int
|        -ImplicitCastExpr   int LValueToRValue
|         -DeclRefExpr p  int
|        -IntegerLiteral  1 int
|    -CallExpr   void
|     -ImplicitCastExpr   void FunctionToPointerDecay
|      -DeclRefExpr _sysy_putfarray  void
|     -ImplicitCastExpr   int LValueToRValue
|      -DeclRefExpr len  int
|     -ImplicitCastExpr   float[10] LValueToRValue
|      -DeclRefExpr arr  float[10]
|    -ReturnStmt   
|     -IntegerLiteral  0 int