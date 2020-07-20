#ifndef LLVM_LIB_TARGET_CPU0_MCTARGETDESC_CPU0ASMBACKEND_H_
#define LLVM_LIB_TARGET_CPU0_MCTARGETDESC_CPU0ASMBACKEND_H_

#include "CPU0FixupKinds.h"
#include "llvm/ADT/Triple.h"
#include "llvm/MC/MCAsmBackend.h"
#include "llvm/Support/Endian.h"

namespace llvm {

  class MCAssembler;
  struct MCFixupKindInfo;
  class Target;
  class MCObjectTargetWriter;

  class CPU0AsmBackend : public MCAsmBackend {
    Triple::OSType OSType;

    bool IsLittle;

  public:
    CPU0AsmBackend(const Target &T, Triple::OSType OS, bool IsLittle_)
      : MCAsmBackend(IsLittle_
                     ? support::endianness::little
                     : support::endianness::big), OSType(OS), IsLittle(IsLittle_) {}

    std::unique_ptr<MCObjectTargetWriter> createObjectTargetWriter() const override;

    void applyFixup(const MCAssembler &Asm,
                    const MCFixup &Fixup,
                    const MCValue &Target,
                    MutableArrayRef<char> Data,
                    uint64_t Value, bool IsResolved,
                    const MCSubtargetInfo *STI) const override;

    const MCFixupKindInfo &getFixupKindInfo(MCFixupKind Kind) const override;

    unsigned getNumFixupKinds() const override {
      return CPU0::NumTargetFixupKinds;
    }

    /// Check whether the given instruction may need relaxation
    bool mayNeedRelaxation(const MCInst &Inst, const MCSubtargetInfo &STI) const override {
      return false;
    }

    /// Target specific predicate for whether a given fixup requires
    /// the associated instruction to be relaxed.
    bool fixupNeedsRelaxation(const MCFixup &Fixup, uint64_t Value,
                              const MCRelaxableFragment *DF,
                              const MCAsmLayout &Layout) const override {
      llvm_unreachable("unimplemented");
      return false;
    }

    bool writeNopData(raw_ostream &OS, uint64_t Count) const override;
  };
}

#endif
