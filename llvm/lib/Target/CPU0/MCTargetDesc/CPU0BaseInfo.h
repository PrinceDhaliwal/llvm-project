#ifndef LLVM_LIB_TARGET_CPU0_MCTARGETDESC_CPU0BASEINFO_H
#define LLVM_LIB_TARGET_CPU0_MCTARGETDESC_CPU0BASEINFO_H

#include "CPU0MCTargetDesc.h"
#include "CPU0FixupKinds.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/Support/DataTypes.h"
#include "llvm/Support/ErrorHandling.h"

namespace llvm {
  namespace CPU0II {
    /// Target Operand Flag Enum
    enum TOF {
      MO_NO_FLAG,

      /// Represents the offset into the global offset table at
      /// which the address of a call site relocation entry symbol
      /// resides during execution. This flag can only be present
      /// in call instructions
      MO_GOT_CALL,

      /// Represents the offset from the current gp value to be used
      /// for the relocatable object file being produced
      MO_GPREL,


      /// Represents the hi or low part of an absolute symbol address
      MO_ABS_HI,
      MO_ABS_LO,

      /// Relocations used for large GOTs
      MO_GOT_HI16,
      MO_GOT_LO16
    };

    enum {
      // Instruction encodings
      Pseudo = 0,

      /// FrmR - format R instructions
      FrmR = 1,
      /// FrmI - format I instructions
      FrmI = 2,
      /// FrmJ - format J instructions
      FrmJ = 3,

      /// Instructions having no specific format
      FrmOther = 4,

      FormMask = 15
    };
  }
}
      

#endif
