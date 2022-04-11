#include <llvm/IR/Module.h>
#include <llvm/IR/Instructions.h>
#include <llvm/Pass.h>
#include <llvm/Support/raw_ostream.h>

#include <string>
#include <map>
using namespace llvm;

namespace {

class FunctionInfo final : public ModulePass {
public:
  static char ID;

  class Info{
    public:
      int args;
      int calls;
      int blocks;
      int insts;
  };

  std::map<std::string, Info> infos;

  FunctionInfo() : ModulePass(ID) {}

  // We don't modify the program, so we preserve all analysis.
  virtual void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.setPreservesAll();
  }

  virtual bool runOnModule(Module &M) override {
    outs() << "CSCD70 Function Information Pass"
           << "\n";

    for(auto &func:M){
      auto name = func.getName().str();
      infos[name] = Info();

    }

    /**
     * @todo(cscd70) Please complete this method.
     */

    return false;
  }

  void iterate_function(Function&func){
    auto name = func.getName().str();
    infos[name].args = func.arg_size();
    infos[name].blocks = func.getBasicBlockList().size();
    for(auto&bb:func){
      infos[name].insts += iterate_block(bb);
    }
  }


//return instruction_num;
  int iterate_block(BasicBlock&block){
    int inst_num = 0;
    for(auto&inst:block){
      if(CallInst* call_inst = dyn_cast<CallInst>(&inst)){
        auto name = call_inst->getCalledFunction()->getName().str();
        infos[name].calls+=1;
        inst_num++;
      }
    }
    return inst_num;
  }
}; // class FunctionInfo

char FunctionInfo::ID = 0;
RegisterPass<FunctionInfo> X("function-info", "CSCD70: Function Information");

} // anonymous namespace
