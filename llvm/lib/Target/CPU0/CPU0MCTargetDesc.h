#ifndef LLVM_LIB_TARGET_CPU0_MCTARGETDESC_CPU0MC_TARGETDESC_H_
#define LLVM_LIB_TARGET_CPU0_MCTARGETDESC_CPU0MC_TARGETDESC_H_

#include "CPU0Config.h"
#include "llvm/Support/DataTypes.h"

namespace llvm {
class Target;
class Triple;
extern Target TheCPU0Target;
extern Target TheCPU0elTarget;

}

#define GET_REGINFO_ENUM
#include "CPU0GenRegisterInfo.inc"

#define GET_INSTRINFO_ENUM
#include "CPU0GenInstrInfo.inc"

#define GET_SUBTARGETINFO_ENUM
#include "CPU0GenSubtargetInfo.inc"

#endif
