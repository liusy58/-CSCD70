
/**
 * @file Liveness Dataflow Analysis
 */
#include <unordered_map>


#include "Variable.h"

#include <llvm/IR/Instructions.h>
#include <llvm/IR/Module.h>
#include <llvm/Pass.h>
#include <llvm/Support/raw_ostream.h>


#include <dfa/Framework.h>
#include <dfa/MeetOp.h>


using namespace dfa;
using namespace llvm;
using namespace std;
using LivenessFrameworkBase =
    Framework<Variable, bool, Direction::kBackward, Union>;

namespace {

/**
 * @todo(cscd70) Implement @c Liveness using the @c dfa::Framework interface.
 */
class Liveness final : public LivenessFrameworkBase, public FunctionPass {
public:
  static char ID;

  Liveness() : LivenessFrameworkBase(),FunctionPass(ID) {}

  unordered_map<const Value*,int>Variable2index;

  virtual void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.setPreservesAll();
  }


  virtual void initializeDomainFromInst(const Instruction &Inst) override {
    if(isa<PHINode>(&Inst)){
      const Value *V = &Inst;
      Domain.push_back(Variable(V));
      Variable2index.emplace(V,Domain.size()-1);
    }
    for(const auto * Iter = Inst.op_begin(); Iter != Inst.op_end(); ++Iter){
      Value *V = *Iter;
      if(isa<Instruction>(V) || isa<Argument>(V)){
        if(std::find(Domain.begin(), Domain.end(), Variable(V)) == Domain.end()){

          errs() << "insert " << *V << " from  " << Inst << "\n";

          Domain.push_back(Variable(V));
          Variable2index.emplace(V,Domain.size()-1);
        }
      }
    }

    // errs() << Inst << "     " << Domain.size() << "\n";
  }


  virtual bool transferFunc(const Instruction &Inst, const DomainVal_t &IBV,
                            DomainVal_t &OBV) override {
    /**
     * @todo(cscd70) Please complete the definition of the transfer function.
     */
    // for(auto It:Variable2index){
    //   errs() << "---" << *It.first << " index = " << It.second << "\n";
    // }


    errs() << Inst << "   " <<"\n";
    errs() << "In: " ;
    for(auto V: IBV){errs() << V; }
    errs() << "  Out: " ;
    for(auto V: OBV){errs() << V; }  
    errs() << "\n" ;


    
    DomainVal_t Out = IBV;
    for(const auto * Iter = Inst.op_begin(); Iter != Inst.op_end(); ++Iter){
      Value *V = *Iter;
      errs() << "Value = " << *V <<"\n";
      if(isa<Instruction>(V) || isa<Argument>(V)){
        auto Index = Variable2index[V];
        Out[Index] = true;
      }
    }
    if(Variable2index.find(&Inst) != Variable2index.end()){
      auto Index = Variable2index[&Inst];
      Out[Index] = false;
    }

    auto Changed = diff(Out,OBV);
    InstDomainValMap[&Inst] = Out;

    errs() << "In: " ;
    for(auto V: IBV){errs() << V; }
    errs() << "  Out: " ;
    for(auto V: OBV){errs() << V; }  
    errs() << "\n" ;
    
    return Changed;
  }

  virtual bool runOnFunction(Function &F) override {
    return LivenessFrameworkBase::runOnFunction(F);
  }
};

char Liveness::ID = 0;
RegisterPass<Liveness> X("liveness", "Liveness");

} // anonymous namespace
