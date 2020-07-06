#ifndef LLVM_LIB_TARGET_CPU0_CPU0MACHINEFUNCTION_H
#define LLVM_LIB_TARGET_CPU0_CPU0MACHINEFUNCTION_H

#include "CPU0Config.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineMemOperand.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/PseudoSourceValue.h"
#include "llvm/Target/TargetMachine.h"

#include <map>

namespace llvm {

  class CPU0FunctionInfo : public MachineFunctionInfo {
  public:
    CPU0FunctionInfo(MachineFunction &MF)
      : MF(MF), VarArgsFrameIndex(0), MaxCallFrameSize(0) {}

    ~CPU0FunctionInfo();

    int getVarArgsFrameIndex() const { return VarArgsFrameIndex; }
    void setVarArgsFrameIndex(int Index) { VarArgsFrameIndex = Index; }

  private:
    virtual void anchor();

    MachineFunction &MF;
    int VarArgsFrameIndex;
    unsigned MaxCallFrameSize;
  };

}
#endif
