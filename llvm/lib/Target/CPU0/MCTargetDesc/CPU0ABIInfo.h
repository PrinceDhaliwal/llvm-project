#ifndef LLVM_LIB_TARGET_CPU0_MCTARGETDESC_CPU0ABIINFO_H
#define LLVM_LIB_TARGET_CPU0_MCTARGETDESC_CPU0ABIINFO_H

#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/Triple.h"
#include "llvm/IR/CallingConv.h"
#include "llvm/MC/MCRegisterInfo.h"

namespace llvm {

  class MCTargetOptions;
  class StringRef;
  class TargetRegisterClass;

  class CPU0ABIInfo {
  public:
    enum class ABI { Unknown, O32, S32 };

  protected:
    ABI ThisABI;

  public:
    CPU0ABIInfo(ABI ThisABI)
      : ThisABI(ThisABI) {}

    static CPU0ABIInfo Unknown() { return CPU0ABIInfo(ABI::Unknown); }
    static CPU0ABIInfo O32() { return CPU0ABIInfo(ABI::O32); }
    static CPU0ABIInfo S32() { return CPU0ABIInfo(ABI::S32); }

    static CPU0ABIInfo computeTargetABI();

    bool IsKnown() const { return ThisABI != ABI::Unknown; }
    bool IsO32() const { return ThisABI == ABI::O32; }
    bool IsS32() const { return ThisABI == ABI::S32; }

    ABI GetEnumValue() const { return ThisABI; }

    // the registers to use for byval arguments
    const ArrayRef<MCPhysReg> GetByValArgRegs() const;

    // the registers to use for the variable argument list
    const ArrayRef<MCPhysReg> GetVarArgRegs() const;

    // Obtain the size of the area allocatecd by the callee for arguments.
    // CallingConv::FastCall affects the value for O32
    unsigned GetCalleeAllocArgSizeInBytes(CallingConv::ID CC) const;

    // Ordering of ABI's
    // CPU0GenSubtargetInfo.inc will use this to resolve conflicts when given
    // multiple ABI options.
    bool operator<(const CPU0ABIInfo Other) const {
      return ThisABI < Other.GetEnumValue();
    }

    unsigned GetStackPtr() const;
    unsigned GetFramePtr() const;
    unsigned GetNullPtr() const;

    unsigned GetEhDataReg(unsigned I) const;
    int EhDataRegSize() const;
  };
}
#endif
