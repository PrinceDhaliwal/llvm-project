#ifndef LLVM_LIB_TARGET_CPU0_CPU0SEFRAMELOWERING_H_
#define LLVM_LIB_TARGET_CPU0_CPU0SEFRAMELOWERING_H_

#include "CPU0Config.h"
#include "CPU0FrameLowering.h"

namespace llvm {
  class CPU0SEFrameLowering : public CPU0FrameLowering {
  public:
    explicit CPU0SEFrameLowering(const CPU0Subtarget &STI);

    void emitPrologue(MachineFunction &MF, MachineBasicBlock &MBB) const override;
    void emitEpilogue(MachineFunction &MF, MachineBasicBlock &MBB) const override;
    bool hasReservedCallFrame(const MachineFunction &MF) const override;
    void determineCalleeSaves(MachineFunction &MF, BitVector &SavedRegs,
                              RegScavenger *RS) const override;
  };
} // end llvm namespace

#endif
