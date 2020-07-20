#ifndef LLVM_LIB_TARGET_CPU0_MCTARGETDESC_CPU0MCTARGETDESC_H
#define LLVM_LIB_TARGET_CPU0_MCTARGETDESC_CPU0MCTARGETDESC_H

#include "llvm/Support/DataTypes.h"
#include <memory>

namespace llvm {
  class Target;
  class Triple;
  class MCAsmBackend;
  class MCContext;
  class MCCodeEmitter;
  class MCInstrInfo;
  class MCObjectTargetWriter;
  class MCRegisterInfo;
  class MCTargetOptions;
  class MCSubtargetInfo;
  class StringRef;
  class raw_ostream;
  class raw_pwrite_stream;
  
  extern Target TheCPU0Target;
  extern Target TheCPU0elTarget;

MCCodeEmitter *createCPU0MCCodeEmitterEB(const MCInstrInfo &MCII,
                                         const MCRegisterInfo &MRI,
                                         MCContext &Ctx);
MCCodeEmitter *createCPU0MCCodeEmitterEL(const MCInstrInfo &MCII,
                                         const MCRegisterInfo &MRI,
                                         MCContext &Ctx);

MCAsmBackend *createCPU0AsmBackendEB32(const Target &T,
                                       const MCSubtargetInfo &STI,
                                       const MCRegisterInfo &MRI,
                                       const MCTargetOptions &Opt);

MCAsmBackend *createCPU0AsmBackendEL32(const Target &T,
                                       const MCSubtargetInfo &STI,
                                       const MCRegisterInfo &MRI,
                                       const MCTargetOptions &Opt);

std::unique_ptr<MCObjectTargetWriter> createCPU0ELFObjectWriter(uint8_t OSABI);

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
