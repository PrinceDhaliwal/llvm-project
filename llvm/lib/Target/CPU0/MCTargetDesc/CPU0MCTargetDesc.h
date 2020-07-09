
#ifndef LLVM_LIB_TARGET_CPU0_MCTARGETDESC_CPU0MCTARGETDESC_H
#define LLVM_LIB_TARGET_CPU0_MCTARGETDESC_CPU0MCTARGETDESC_H

#include "llvm/Support/DataTypes.h"

namespace llvm {
  class Target;
  class Triple;
  class MCAsmBackend;
  class MCContext;
  class MCCodeEmitter;
  class MCInstrInfo;
  class MCObjectWriter;
  class MCRegisterInfo;
  class MCSubtargetInfo;
  class StringRef;
  class raw_ostream;
  
  extern Target TheCPU0Target;
  extern Target TheCPU0elTarget;

} // End llvm namespace

// Defines symbolic names for CPU0 registers.  This defines a mapping from
// register name to register number.
#define GET_REGINFO_ENUM
#include "CPU0GenRegisterInfo.inc"

// Defines symbolic names for the CPU0 instructions.
#define GET_INSTRINFO_ENUM
#include "CPU0GenInstrInfo.inc"

#define GET_SUBTARGETINFO_ENUM
#include "CPU0GenSubtargetInfo.inc"

#endif
