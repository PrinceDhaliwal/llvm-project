#ifndef LLVM_LIB_TARGET_CPU0_CPU0ISELDAGTODAG_H_
#define LLVM_LIB_TARGET_CPU0_CPU0ISELDAGTODAG_H_

#include "CPU0.h"
#include "CPU0Subtarget.h"
#include "CPU0TargetMachine.h"

#include "llvm/CodeGen/SelectionDAGISel.h"
#include "llvm/IR/Type.h"
#include "llvm/Support/Debug.h"

namespace llvm {

  class CPU0DAGToDAGISel : public SelectionDAGISel {
  public:
    explicit CPU0DAGToDAGISel(CPU0TargetMachine &TM, CodeGenOpt::Level OL)
      : SelectionDAGISel(TM, OL), Subtarget(nullptr) {}

    StringRef getPassName() const override {
      return "CPU0 DAG->DAG Pattern Instruction Selection";
    }

    bool runOnMachineFunction(MachineFunction &MF) override;

  protected:
    /// Keep a pointer to the CPU0Subtarget around so that we can make the
    /// right decision when generating code for different targets
    const CPU0Subtarget *Subtarget;

  private:

    /// Include the pieces autogenerated from the target description
#include "CPU0GenDAGISel.inc"

    /// getTargetmachine - Return a reference to the Targetmachine, casted
    /// to the target-specific type.
    const CPU0TargetMachine &getTargetMachine() {
      return static_cast<const CPU0TargetMachine &>(TM);
    }

    void Select(SDNode *N) override;

    virtual bool trySelect(SDNode *Node) = 0;

    /// Complex Pattern
    bool SelectAddr(SDNode *Parent, SDValue N, SDValue &Base, SDValue &Offset);


    inline SDValue getImm(const SDNode *Node, unsigned Imm) {
      return CurDAG->getTargetConstant(Imm, SDLoc(Node), Node->getValueType(0));
    }
  };
  
}

#endif
