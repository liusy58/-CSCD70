#include <llvm/IR/Constants.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Module.h>
#include <llvm/Pass.h>
#include <llvm/Transforms/Utils/BasicBlockUtils.h>

#include <map>
#include <queue>

using namespace llvm;

namespace {

class MultiInstOpt final : public FunctionPass {
public:
  static char ID;

  MultiInstOpt() : FunctionPass(ID) {}

  /**
   * @todo(cscd70) Please complete the methods below.
   */
  virtual void getAnalysisUsage(AnalysisUsage &AU) const override {
    
  }


  std::pair<Value*, ConstantInt*> isConstantContained(Instruction &Inst){
    // Inst.print(outs());
    Value* V = nullptr;
    ConstantInt *Const = nullptr;
    if(Inst.getOpcode() == Instruction::Add || Inst.getOpcode() == Instruction::Sub){
      for (auto *Iter = Inst.op_begin(); Iter != Inst.op_end(); ++Iter) {
        Value *Operand = *Iter;
        if (ConstantInt *C = dyn_cast<ConstantInt>(Operand)){
          Const = C;
        }else{
          V = Operand;
        }
      }
    }
    return {V, Const};
  }


  virtual bool runOnFunction(Function &F) override { 
    bool ChangeInstruction = false; 
    bool Changed = true;
    while(Changed){
      Changed = false;
      for(auto BBIter = F.begin(); BBIter != F.end(); BBIter++){
        auto &BB = *BBIter;
        for(auto InstIter = BB.begin(); InstIter != BB.end(); InstIter++){
          auto &Inst = *InstIter;
          auto IsConstantContained = isConstantContained(Inst);
          if(IsConstantContained.second != nullptr){
            Value *Operand = IsConstantContained.first;
            auto Value = IsConstantContained.second->getValue();
            if(Inst.getOpcode() == Instruction::Sub){
              Value = -Value;
            }   
            auto Offset = Value;
            while(Offset != 0 && dyn_cast<Argument>(Operand) == nullptr){
              auto *NewInst = dyn_cast<Instruction>(Operand);
              auto IsConstantContained = isConstantContained(*NewInst);
              if(IsConstantContained.second != nullptr){
                Operand = IsConstantContained.first;
                Value = IsConstantContained.second->getValue();
                if(NewInst->getOpcode() == Instruction::Sub){
                  Offset -=  Value;
                } else {
                  Offset +=  Value;
                }
              }else {
                break;
              }
            }
            if(Offset == 0){
              ChangeInstruction = Changed = true;
              ReplaceInstWithValue(BB.getInstList(), InstIter, Operand);
              // BB.print(outs());
              // outs()<< "\n";
              break;
            }
          }
        }
        if(Changed){break;}
      }
    }
  

  // for(auto BBIter = F.begin(); BBIter != F.end(); BBIter++){
  //   auto &BB = *BBIter;
  //   BB.print(outs());
  // }
  return ChangeInstruction; 
  }
}; // class MultiInstOpt

char MultiInstOpt::ID = 0;
RegisterPass<MultiInstOpt> X("multi-inst-opt",
                             "CSCD70: Multi-Instruction Optimization");

} // anonymous namespace
