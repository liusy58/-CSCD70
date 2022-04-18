#include <llvm/IR/Constants.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Module.h>
#include <llvm/Pass.h>
#include <llvm/Transforms/Utils/BasicBlockUtils.h>

#include <map>
#include <queue>

using namespace llvm;

namespace {

class StrengthReduction final : public FunctionPass {
public:
  static char ID;

  StrengthReduction() : FunctionPass(ID) {}

  /**
   * @todo(cscd70) Please complete the methods below.
   */
  virtual void getAnalysisUsage(AnalysisUsage &AU) const override {

  }

  std::pair<int, Value*> canStrengthReduction(Instruction &Inst){
    Value* V= nullptr;
    int LogBase2 = -1;
    if(Inst.getOpcode() == Instruction::Mul){
      for (auto *Iter = Inst.op_begin(); Iter != Inst.op_end(); ++Iter) {
        Value *Operand = *Iter;
        if (ConstantInt *C = dyn_cast<ConstantInt>(Operand)){
          if(C->getValue().isPowerOf2()){
            LogBase2 = C->getValue().exactLogBase2();
          }
        }else{
          V = Operand;
        }
      }
    }

    return {LogBase2 ,V};
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
          auto CanStrengthReduction = canStrengthReduction(Inst);
          if(CanStrengthReduction.first != -1){
            Value* V = CanStrengthReduction.second;
            auto &Context = V->getContext();
            Value * V2 = ConstantInt::get(IntegerType::get(Context,32), (uint64_t)CanStrengthReduction.first/*value*/, false);
            ReplaceInstWithInst(Inst.getParent()->getInstList(), InstIter,
                    BinaryOperator::Create(llvm::Instruction::BinaryOps::Shl, V, V2));
            Changed = true;
            break;
          }
        }
      }
    }
    for(auto &BB:F){ 
      BB.print(outs());
    }
    // outs() << "here is also ok\n";
    return ChangeInstruction; 
  }
}; // class StrengthReduction

char StrengthReduction::ID = 0;
RegisterPass<StrengthReduction> X("strength-reduction",
                                  "CSCD70: Strength Reduction");

} // anonymous namespace
