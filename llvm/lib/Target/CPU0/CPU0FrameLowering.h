#ifndef LLVM_LIB_TARGET_CPU0_CPU0FRAMELOWERING_H_
#define LLVM_LIB_TARGET_CPU0_CPU0FRAMELOWERING_H_

#include "CPU0Config.h"
#include "CPU0.h"

#include "llvm/CodeGen/TargetFrameLowering.h"
#include "llvm/Support/Alignment.h"

namespace llvm {
  class CPU0Subtarget;

  class CPU0FrameLowering : public TargetFrameLowering {
  protected:
    const CPU0Subtarget &STI;

  public:
    explicit CPU0FrameLowering(const CPU0Subtarget &STI, llvm::Align Alignment)
      : TargetFrameLowering(StackGrowsDown, Alignment, 0, Alignment),
      STI(STI) {}

    static const CPU0FrameLowering *create(const CPU0Subtarget &ST);

    bool hasFP(const MachineFunction &MF) const override;
  };

  /// Create CPU0FrameLowering objects
  const CPU0FrameLowering *createCPU0SEFrameLowering(const CPU0Subtarget &ST);
} // End llvm namespace

#endif
