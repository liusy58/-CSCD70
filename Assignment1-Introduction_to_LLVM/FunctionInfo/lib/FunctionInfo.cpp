#include <llvm/IR/Instructions.h>
#include <llvm/IR/Module.h>
#include <llvm/Pass.h>
#include <llvm/Support/raw_ostream.h>

#include <iostream>
#include <map>
#include <string>

using namespace llvm;

namespace {

class FunctionInfo final : public ModulePass {
public:
  static char ID;

  class Info{
    public:
      int Args;
      int Calls;
      int Blocks;
      int Insts;

    Info(int Args=0, int Calls=0, int Blocks=0,int Insts=0): Args(Args), Calls(Calls), Blocks(Blocks), Insts(Insts){}

    friend llvm::raw_ostream& operator<< (llvm::raw_ostream & Stream, const Info& Info){
      Stream << " Number of Arguments: "<< Info.Args << "\n";
      Stream << " Number of Calls: "<< Info.Calls << "\n";
      Stream << " Number OF BBs: "<< Info.Blocks << "\n";
      Stream << " Number of Instructions: "<< Info.Insts << "\n";
      return Stream;
    }
  };

  std::map<std::string, Info> Infos;

  FunctionInfo() : ModulePass(ID) {}

  // We don't modify the program, so we preserve all analysis.
  virtual void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.setPreservesAll();
  }

  virtual bool runOnModule(Module &M) override {
    outs() << "CSCD70 Function Information Pass"
           << "\n";



    for(auto &Func:M){
      auto Name = Func.getName().str();
      Infos[Name] = Info();
    }

    for(auto &Func:M){
      iterateFunction(Func);
    }


    show();

    /**
     * @todo(cscd70) Please complete this method.
     */

    return false;
  }

  void iterateFunction(Function&Func){
    auto Name = Func.getName().str();
    Infos[Name].Args = Func.arg_size();
    Infos[Name].Blocks = Func.getBasicBlockList().size();
    for(auto&BB:Func){
      Infos[Name].Insts += iterateBlock(BB);
    }
  }


//return instruction_num;
  int iterateBlock(BasicBlock&Block){
    for(auto&Inst:Block){
      if(CallInst* Call = dyn_cast<CallInst>(&Inst)){
        auto Name = Call->getCalledFunction()->getName().str();
        Infos[Name].Calls+=1;
      }
    }
    return Block.size();
  }

  void show(){
    // outs() << "args" << " " << "calls" << " " << "blocks" << " " << "insts" << "\n";
    for(auto &[Name,Info]:Infos){
      
      outs() << " Function Name: " << Name << "\n";
      outs() << Info << "\n";
    }
  }
}; // class FunctionInfo

char FunctionInfo::ID = 0;
RegisterPass<FunctionInfo> X("function-info", "CSCD70: Function Information");

} // anonymous namespace
