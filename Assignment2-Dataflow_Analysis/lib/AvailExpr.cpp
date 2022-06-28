/**
 * @file Available Expression Dataflow Analysis
 */
#include <unordered_map>

#include <llvm/IR/Function.h>
#include <llvm/IR/Instruction.h>
#include <llvm/IR/Instructions.h>
#include <llvm/Pass.h>

#include <dfa/Framework.h>
#include <dfa/MeetOp.h>

#include "Expression.h"

using namespace dfa;
using namespace llvm;
using namespace std;
namespace {

using AvailExprFrameworkBase =
    Framework<Expression, bool, Direction::kForward, Intersect>;

class AvailExpr final : public AvailExprFrameworkBase, public FunctionPass {
private:
  unordered_map<const Instruction*,int>Instruction2index;
  virtual void initializeDomainFromInst(const Instruction &Inst) override {
    if (const BinaryOperator *const BinaryOp =
            dyn_cast<BinaryOperator>(&Inst)) {
      /**
       * @todo(cscd70) Please complete the construction of domain.
       */
      Domain.push_back(Expression(*BinaryOp));
      Instruction2index.emplace(&Inst, Domain.size()-1);
    }
  }
  virtual bool transferFunc(const Instruction &Inst, const DomainVal_t &IBV,
                            DomainVal_t &OBV) override {
    /**
     * @todo(cscd70) Please complete the definition of the transfer function.
     */
    DomainVal_t Out = IBV;
    if(const BinaryOperator *const BinaryOp =
            dyn_cast<BinaryOperator>(&Inst)){
        auto Index = Instruction2index[&Inst];
        Out[Index] = true;
    }
    auto Changed = diff(Out,OBV);
    InstDomainValMap[&Inst] = Out;
    return Changed;
  }

public:
  static char ID;

  AvailExpr() : AvailExprFrameworkBase(), FunctionPass(ID) {}

  virtual void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.setPreservesAll();
  }
  bool runOnFunction(Function &F) override {
    return AvailExprFrameworkBase::runOnFunction(F);
  }
};

char AvailExpr::ID = 0;
RegisterPass<AvailExpr> X("avail-expr", "Available Expression");

} // anonymous namespace
