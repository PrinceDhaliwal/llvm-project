#ifndef LLVM_LIB_TARGET_CPU0_MCTARGETDESC_CPU0FIXUPKINDS_H
#define LLVM_LIB_TARGET_CPU0_MCTARGETDESC_CPU0FIXUPKINDS_H

#include "llvm/MC/MCFixup.h"

namespace llvm {
namespace CPU0 {
  // Although most of the current fixup types reflect a unique relocation
  // one can have multiple fixup types for a given relocation and thus need
  // to be uniquely named.
  //
  // This table *must* be in the save order of
  // MCFixupKindInfo Infos[CPU0::NumTargetFixupKinds]
  // in CPU0AsmBackend.cpp.
  enum Fixups {
    //@ Pure upper 32 bit fixup resulting in - R_CPU0_32.
    fixup_CPU0_32 = FirstTargetFixupKind,

    // Pure upper 16 bit fixup resulting in - R_CPU0_HI16.
    fixup_CPU0_HI16,

    // Pure lower 16 bit fixup resulting in - R_CPU0_LO16.
    fixup_CPU0_LO16,

    // 16 bit fixup for GP offest resulting in - R_CPU0_GPREL16.
    fixup_CPU0_GPREL16,

    // Symbol fixup resulting in - R_CPU0_GOT16.
    fixup_CPU0_GOT,

    

    // resulting in - R_CPU0_GOT_HI16
    fixup_CPU0_GOT_HI16,

    // resulting in - R_CPU0_GOT_LO16
    fixup_CPU0_GOT_LO16,

    // Marker
    LastTargetFixupKind,
    NumTargetFixupKinds = LastTargetFixupKind - FirstTargetFixupKind
  };

} // namespace CPU0
} // namespace llvm

#endif
