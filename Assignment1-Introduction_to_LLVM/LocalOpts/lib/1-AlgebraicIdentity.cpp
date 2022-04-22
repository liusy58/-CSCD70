
#include <llvm/IR/Constants.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Module.h>
#include <llvm/Pass.h>
#include <llvm/Transforms/Utils/BasicBlockUtils.h>

#include <map>
#include <queue>

using namespace llvm;

namespace {

class AlgebraicIdentity final : public FunctionPass {
private:
    // std::map<Instruction*, Instruction*> Instructions;
public:
  static char ID;

  AlgebraicIdentity() : FunctionPass(ID) {}

  /**
   * @todo(cscd70) Please complete the methods below.
   */
  virtual void getAnalysisUsage(AnalysisUsage &AU) const override {

  }

  std::pair<bool, Value*> isAlgebracIdentity(Instruction &Inst){
    Value* V= nullptr;
    bool IsAlgebracIdentity = false;
    if(Inst.getOpcode() == Instruction::Add){
      for (auto *Iter = Inst.op_begin(); Iter != Inst.op_end(); ++Iter) {
        Value *Operand = *Iter;
        if (ConstantInt *C = dyn_cast<ConstantInt>(Operand)){
          if(C->getValue() == 0){
            IsAlgebracIdentity = true;
          }
        }else{
          V = Operand;
        }
      }
    }

    if(Inst.getOpcode() == Instruction::Sub){
      V = Inst.getOperand(0);
      if (ConstantInt *C = dyn_cast<ConstantInt>(Inst.getOperand(1))){
          if(C->getValue() == 0){
            IsAlgebracIdentity = true;
          }
        }
    }


    if(Inst.getOpcode() == Instruction::Mul){
      for (auto *Iter = Inst.op_begin(); Iter != Inst.op_end(); ++Iter) {
        Value *Operand = *Iter;
        if (ConstantInt *C = dyn_cast<ConstantInt>(Operand)){
          if(C->getValue() == 1){
            IsAlgebracIdentity = true;
          }
        }else{
          V = Operand;
        }
      }
    }

    if(Inst.getOpcode() == Instruction::SDiv || Inst.getOpcode() == Instruction::UDiv){
      V = Inst.getOperand(0);
      if (ConstantInt *C = dyn_cast<ConstantInt>(Inst.getOperand(1))){
          if(C->getValue() == 1){
            IsAlgebracIdentity = true;
          }
        }
    }
    return {IsAlgebracIdentity, V};
  }
  virtual bool runOnFunction(Function &F) override {
    bool ChangeInstruction = false;

    // std::queue<std::tuple<Instruction *, BasicBlock&, Value*>>Worklist;
  for(BasicBlock &BB : F) {
    auto InstIter = BB.begin();
    while(InstIter != BB.end()) {
      Instruction& Inst = *InstIter;
      auto Res = isAlgebracIdentity(Inst);
      bool IsAlgebrac = Res.first;
      ++InstIter;
      if(IsAlgebrac){
        Value * V = Res.second;
        auto Iter = Inst.getIterator();
        ReplaceInstWithValue(BB.getInstList(), Iter, V);
        ChangeInstruction = true;
      }
    }
  }
    // bool Changed = true;
    // while(Changed){
    //   Changed = false;
    //   for(auto BBIter = F.begin(); BBIter != F.end(); BBIter++){
    //     auto &BB = *BBIter;
    //     for(auto InstIter = BB.begin(); InstIter != BB.end(); InstIter++){
    //       auto &Inst = *InstIter;
    //       auto Res = isAlgebracIdentity(Inst);
    //       auto IsAlgebrac = Res.first;
    //       Value * V = Res.second;
    //       if(IsAlgebrac){
    //         ReplaceInstWithValue(BB.getInstList(), InstIter, V);
    //         ChangeInstruction = Changed = true;
    //         break;
    //       }
    //     }
    //   }
    // }
    // for(auto &BB:F){
    //   BB.print(outs());
    // }
    // outs() << "here is also ok\n";
    return ChangeInstruction; 
  }
}; // class AlgebraicIdentity

char AlgebraicIdentity::ID = 0;
RegisterPass<AlgebraicIdentity> X("algebraic-identity",
                                  "CSCD70: Algebraic Identity");

} // anonymous namespace





