
#include "/workspace/SYsU-lang/generator/asg.h"



int main() {
  auto llvmin = llvm::MemoryBuffer::getFileOrSTDIN("-");
  auto json = llvm::json::parse(llvmin.get()->getBuffer());
  
  Tree * root = CreateFromJson(json->getAsObject());
  
  root -> accept(new Print);
  root -> accept(new ToLLVM);
  TheModule.print(llvm::errs(), nullptr);
  TheModule.print(llvm::outs(), nullptr);
  
  return 0;
}