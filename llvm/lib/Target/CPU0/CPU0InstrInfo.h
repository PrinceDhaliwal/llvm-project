#ifndef LLVM_LIB_TARGET_CPU0_CPU0INSTRINFO_H
#define LLVM_LIB_TARGET_CPU0_CPU0INSTRINFO_H

#include "CPU0Config.h"

#include "CPU0.h"
#include "CPU0RegisterInfo.h"

#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/TargetInstrInfo.h"

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
  };

  const CPU0InstrInfo *createCPU0SEInstrInfo(const CPU0Subtarget &STI);
}
    

#endif
