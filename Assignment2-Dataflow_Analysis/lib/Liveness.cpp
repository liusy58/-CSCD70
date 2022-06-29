
/**
 * @file Liveness Dataflow Analysis
 */
#include <unordered_map>


#include "Variable.h"
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

    for(const auto * Iter = Inst.op_begin(); Iter != Inst.op_end(); ++Iter){
      Value *V = *Iter;
      if(isa<Instruction>(V) || isa<Argument>(V)){
        if(std::find(Domain.begin(), Domain.end(), Variable(V)) == Domain.end()){
          Domain.push_back(Variable(V));
          Variable2index.emplace(V,Domain.size()-1);
        }
      }
    }

    errs() << Inst << "     " << Domain.size() << "\n";
  }


  virtual bool transferFunc(const Instruction &Inst, const DomainVal_t &IBV,
                            DomainVal_t &OBV) override {
    /**
     * @todo(cscd70) Please complete the definition of the transfer function.
     */
    errs() << Inst << "---";
    DomainVal_t Out = IBV;
    for(const auto * Iter = Inst.op_begin(); Iter != Inst.op_end(); ++Iter){
      Value *V = *Iter;
      if(isa<Instruction>(V) || isa<Argument>(V)){
        auto Index = Variable2index[&Inst];
        Out[Index] = true;
      }
    }
    auto Changed = diff(Out,OBV);
    InstDomainValMap[&Inst] = Out;
    return Changed;
  }

  virtual bool runOnFunction(Function &F) override {
    // clang-format off
    // errs() << "**************************************************" << "\n"
    //        << "* Instruction-Domain Value Mapping===" << "\n"
    //        << "**************************************************" << "\n";
    // // clang-format on
    // for(auto &BB:F){
    //   for(auto &Inst:BB){
    //     for(auto Iter = Inst.op_begin(); Iter != Inst.op_end(); ++Iter){
    //       Value *V = *Iter;
    //       if(isa<Instruction>(V) || isa<Argument>(V)){
            
    //       } 
    //     }
    //   }
    // }
    

    // return false;
    return LivenessFrameworkBase::runOnFunction(F);
  }
};

char Liveness::ID = 0;
RegisterPass<Liveness> X("liveness", "Liveness");

} // anonymous namespace
