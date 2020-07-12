#ifndef LLVM_LIB_TARGET_CPU0_CPU0INSTRINFO_H
#define LLVM_LIB_TARGET_CPU0_CPU0INSTRINFO_H

#include "CPU0Config.h"

#include "CPU0.h"
#include "CPU0RegisterInfo.h"
#include "CPU0AnalyzeImmediate.h"

#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/TargetInstrInfo.h"
#include "llvm/CodeGen/MachineBasicBlock.h"

#define GET_INSTRINFO_HEADER
#include "CPU0GenInstrInfo.inc"

namespace llvm {
  class CPU0InstrInfo : public CPU0GenInstrInfo {
    virtual void anchor();
  protected:
    const CPU0Subtarget &Subtarget;
  public:
    explicit CPU0InstrInfo(const CPU0Subtarget &STI);

    static const CPU0InstrInfo *create(CPU0Subtarget &STI);

    virtual const CPU0RegisterInfo &getRegisterInfo() const = 0;

    unsigned getInstSizeInBytes(const MachineInstr &MI) const;

    void storeRegToStackSlot(MachineBasicBlock &MBB,
                             MachineBasicBlock::iterator MBBI,
                             Register SrcReg, bool IsKill, int FrameIndex,
                             const TargetRegisterClass *RC,
                             const TargetRegisterInfo *TRI) const override {
      storeRegToStack(MBB, MBBI, SrcReg, IsKill, FrameIndex, RC, TRI, 0);
    }

    void loadRegFromStackSlot(MachineBasicBlock &MBB,
                              MachineBasicBlock::iterator MBBI,
                              Register DestReg, int FrameIndex,
                              const TargetRegisterClass *RC,
                              const TargetRegisterInfo *TRI) const override {
      loadRegFromStack(MBB, MBBI, DestReg, FrameIndex, RC, TRI, 0);
    }

    virtual void storeRegToStack(MachineBasicBlock &MBB,
                                 MachineBasicBlock::iterator MI,
                                 Register SrcReg, bool isKill, int FrameIndex,
                                 const TargetRegisterClass *RC,
                                 const TargetRegisterInfo *TRI,
                                 int64_t Offset) const = 0;

    virtual void loadRegFromStack(MachineBasicBlock &MBB,
                                  MachineBasicBlock::iterator MI,
                                  Register DestReg, int FrameIndex,
                                  const TargetRegisterClass *RC,
                                  const TargetRegisterInfo *TRI,
                                  int64_t Offset) const = 0;

    MachineMemOperand *GetMemOperand(MachineBasicBlock &MBB,
                                     int FI, MachineMemOperand::Flags Flags) const;
  };

  const CPU0InstrInfo *createCPU0SEInstrInfo(const CPU0Subtarget &STI);
}
    

#endif
