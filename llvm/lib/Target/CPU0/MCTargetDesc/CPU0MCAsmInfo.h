#ifndef LLVM_LIB_TARGET_CPU0_MCTARGETDESC_CPU0MCASMINFO_H
#define LLVM_LIB_TARGET_CPU0_MCTARGETDESC_CPU0MCASMINFO_H

#include "CPU0TargetStreamer.h"
#include "llvm/MC/MCAsmInfoELF.h"

namespace llvm {
  class Triple;

  class CPU0MCAsmInfo : public MCAsmInfoELF {
    void anchor() override;
  public:
    explicit CPU0MCAsmInfo(const Triple &TheTriple);
  };
}

#endif
