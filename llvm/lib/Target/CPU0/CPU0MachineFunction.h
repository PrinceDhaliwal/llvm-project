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
      : MF(MF), VarArgsFrameIndex(0), MaxCallFrameSize(0), EmitNOAT(false),
	SRetReturnReg(0), CallsEhReturn(false), CallsEhDwarf(false)
    {}

    ~CPU0FunctionInfo();

    int getVarArgsFrameIndex() const { return VarArgsFrameIndex; }
    void setVarArgsFrameIndex(int Index) { VarArgsFrameIndex = Index; }

    bool getEmitNOAT() const { return EmitNOAT; }
    void setEmitNOAT() { EmitNOAT = true; }

    unsigned getSRetReturnReg() const { return SRetReturnReg; }
    void setSRetReturnReg(unsigned Reg) { SRetReturnReg = Reg; }

    bool hasByvalArg() const { return HasByvalArg; }
    void setFormalArgInfo(unsigned Size, bool HasByval) {
      IncomingArgSize = Size;
      HasByvalArg = HasByval;
    }

    unsigned getIncomingArgSize() const { return IncomingArgSize; }

    bool callsEhReturn() const { return CallsEhReturn; }
    void setCallsEhReturn() { CallsEhReturn = true; }

    bool callsEhDwarf() const { return CallsEhDwarf; }
    void setCallsEhDwarf() { CallsEhDwarf = true; }

    void createEhDataRegsFI();
    int getEhDataRegFI(unsigned Reg) const { return EhDataRegFI[Reg]; }

    unsigned getMaxCallFrameSize()  const { return MaxCallFrameSize; }
    void setMaxCallFrameSize(unsigned S) { MaxCallFrameSize = S; }
  private:
    virtual void anchor();

    MachineFunction &MF;
    int VarArgsFrameIndex;
    unsigned MaxCallFrameSize;
    bool EmitNOAT;

    /// SRetReturnReg - Some subtargets require that sret lowering includes
    /// returnign the vlaue of the returned struct in a register. This field
    /// holds the virtual register into which the sret argument is passed.
    unsigned SRetReturnReg;

    /// True if function has a byval argument.
    bool HasByvalArg;

    /// Size of incoming argument area.
    unsigned IncomingArgSize;

    /// CallsEhReturn - Whether the function calls llvm.eh.return.
    bool CallsEhReturn;

    /// CallsEhDwarf - Whether the function calls llvm.eh.dwarf.
    bool CallsEhDwarf;

    /// Frame objects for spilling eh data registers.
    int EhDataRegFI[2];
  };

}
#endif
