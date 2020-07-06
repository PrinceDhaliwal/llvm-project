#ifndef LLVM_LIB_TARGET_CPU0_CPU0REGISTERINFO_H
#define LLVM_LIB_TARGET_CPU0_CPU0REGISTERINFO_H

#include "CPU0.h"
#include "llvm/CodeGen/TargetRegisterInfo.h"

#define GET_REGINFO_HEADER
#include "CPU0GenRegisterInfo.inc"

namespace llvm {
  class CPU0Subtarget;
  class TargetInstrInfo;
  class Type;

  class CPU0RegisterInfo : public CPU0GenRegisterInfo {
  protected:
    const CPU0Subtarget &Subtarget;

  public:
    CPU0RegisterInfo(const CPU0Subtarget &Subtarget);

    const MCPhysReg *getCalleeSavedRegs(const MachineFunction *MF) const override;

    const uint32_t *getCallPreservedMask(const MachineFunction &MF,
                                         CallingConv::ID) const override;

    BitVector getReservedRegs(const MachineFunction &MF) const override;

    bool requiresRegisterScavenging(const MachineFunction &MF) const override;
    bool trackLivenessAfterRegAlloc(const MachineFunction &MF) const override;

    void eliminateFrameIndex(MachineBasicBlock::iterator II,
                             int SPAdj, unsigned FIOperandNum,
                             RegScavenger *RS = nullptr) const override;

    Register getFrameRegister(const MachineFunction &MF) const override;

    virtual const TargetRegisterClass *intRegClass(unsigned Size) const = 0;
  };
} // end namespace llvm

#endif
