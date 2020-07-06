#ifndef LLVM_LIB_TARGET_CPU0_CPU0SEREGISTERINFO_H
#define LLVM_LIB_TARGET_CPU0_CPU0SEREGISTERINFO_H

#include "CPU0RegisterInfo.h"

namespace llvm {
  class CPU0SEInstrInfo;

  class CPU0SERegisterInfo : public CPU0RegisterInfo {
  public:
    CPU0SERegisterInfo(const CPU0Subtarget &Subtarget);

    const TargetRegisterClass *intRegClass(unsigned Size) const override;
  };
} // end namespace llvm

#endif
