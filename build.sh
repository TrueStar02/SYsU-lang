rm -rf $HOME/sysu
cmake -G Ninja   -DCMAKE_C_COMPILER=clang   -DCMAKE_CXX_COMPILER=clang++   -DCMAKE_INSTALL_PREFIX=$HOME/sysu   -DCMAKE_MODULE_PATH=$(llvm-config --cmakedir)   -DCPACK_SOURCE_IGNORE_FILES=".git/;tester/third_party/"   -B $HOME/sysu/build
cmake --build $HOME/sysu/build
cmake --build $HOME/sysu/build -t install
( export PATH=$HOME/sysu/bin:$PATH   CPATH=$HOME/sysu/include:$CPATH   LIBRARY_PATH=$HOME/sysu/lib:$LIBRARY_PATH   LD_LIBRARY_PATH=$HOME/sysu/lib:$LD_LIBRARY_PATH &&   sysu-compiler --unittest=parser-1 "/workspace/SYsU-lang/tester/**/**.sysu.c" )