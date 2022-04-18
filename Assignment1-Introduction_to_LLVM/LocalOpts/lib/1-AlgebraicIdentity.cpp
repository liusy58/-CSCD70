
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

    return {IsAlgebracIdentity, V};
  }
  virtual bool runOnFunction(Function &F) override {
    bool ChangeInstruction = false;

    // std::queue<std::tuple<Instruction *, BasicBlock&, Value*>>Worklist;
    bool Changed = true;
    while(Changed){
      Changed = false;
      for(auto BBIter = F.begin(); BBIter != F.end(); BBIter++){
        auto &BB = *BBIter;
        for(auto InstIter = BB.begin(); InstIter != BB.end(); InstIter++){
          auto &Inst = *InstIter;
          auto Res = isAlgebracIdentity(Inst);
          auto IsAlgebrac = Res.first;
          Value * V = Res.second;
          if(IsAlgebrac){
            ReplaceInstWithValue(BB.getInstList(), InstIter, V);
            BB.print(outs());
            Changed = true;
            break;
          }
        }
      }
    }
    // outs() << "here is also ok\n";
    return ChangeInstruction; 
  }
}; // class AlgebraicIdentity

char AlgebraicIdentity::ID = 0;
RegisterPass<AlgebraicIdentity> X("algebraic-identity",
                                  "CSCD70: Algebraic Identity");

} // anonymous namespace





