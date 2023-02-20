
/**
 * @file Liveness Dataflow Analysis
 */
#include <unordered_map>
#include <set>

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

  set<const Instruction*> Defs; // variable defined(φexcluded)
  unordered_map<const Instruction*, set<Instruction*>> Uses; // variable used(φexcluded)
  unordered_map<const Instruction*, set<Instruction*>>UpwardExposed; // variables used in B without any preceding definition in B
  unordered_map<const Instruction*, set<Instruction*>> PhiDefs;
  unordered_map<const Instruction*, set<Instruction*>> PhiUses;


  virtual void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.setPreservesAll();
  }


  virtual void initializeDomainFromInst(const Instruction &Inst) override {
    Domain.push_back(&Inst);
    Variable2index.insert({&Inst, Domain.size()-1});
    for(const auto * Iter = Inst.op_begin(); Iter != Inst.op_end(); ++Iter){
          Value *V = *Iter;
          if(isa<Instruction>(V) || isa<Argument>(V)){
              Domain.push_back(V);
              Variable2index.insert({V, Domain.size()-1});
          }
    }
  }


  virtual bool transferFunc(const Instruction &Inst, const DomainVal_t &IBV,
                            DomainVal_t &OBV) override {
    /**
     * @todo(cscd70) Please complete the definition of the transfer function.
     */
    errs() << Inst << "   " <<"\n";
    errs() << "In: " ;
    for(auto V: IBV){errs() << V; }
    errs() << "  Out: " ;
    for(auto V: OBV){errs() << V; }  
    errs() << "\n" ;
    DomainVal_t Out = IBV;
    if(isa<PHINode>(&Inst)){
        errs() << "PhisNode\n";
        const PHINode* Phi = dyn_cast<PHINode>(&Inst);
        for(unsigned int I =0 ; I < Phi->getNumOperands(); ++I){
            auto *Value = Phi->getIncomingValue(I);
            auto Index = Variable2index[Value];
            Out[Index] = false;
        }
    } else {
        errs() << "NONEPhisNode\n";
        for(const auto * Iter = Inst.op_begin(); Iter != Inst.op_end(); ++Iter){
            Value *V = *Iter;
            if(isa<Instruction>(V) || isa<Argument>(V)){
                auto Index = Variable2index[V];
                Out[Index] = true;
            }
        }
    }
    Out[Variable2index[&Inst]] = false;
    auto Changed = diff(Out,OBV);
    InstDomainValMap[&Inst] = Out;
//      errs() << "New In: " ;
//      for(auto V: IBV){errs() << V; }
      errs() << "New  Out: " ;
      for(auto V: Out){errs() << V; }
      errs() << "\n" ;


    return Changed;


    
//    DomainVal_t Out = IBV;
//    for(const auto * Iter = Inst.op_begin(); Iter != Inst.op_end(); ++Iter){
//      Value *V = *Iter;
//      errs() << "Value = " << *V <<"\n";
//      if(isa<Instruction>(V) || isa<Argument>(V)){
//        auto Index = Variable2index[V];
//        Out[Index] = true;
//      }
//    }
//    if(Variable2index.find(&Inst) != Variable2index.end()){
//      auto Index = Variable2index[&Inst];
//      Out[Index] = false;
//    }
//
//    auto Changed = diff(Out,OBV);
//    InstDomainValMap[&Inst] = Out;
//
//    errs() << "In: " ;
//    for(auto V: IBV){errs() << V; }
//    errs() << "  Out: " ;
//    for(auto V: OBV){errs() << V; }
//    errs() << "\n" ;
//
//    return Changed;
  }

  virtual bool runOnFunction(Function &F) override {

    return LivenessFrameworkBase::runOnFunction(F);

  }
};

char Liveness::ID = 0;
RegisterPass<Liveness> X("liveness", "Liveness");

} // anonymous namespace
