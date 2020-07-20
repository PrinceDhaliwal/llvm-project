#include "CPU0BaseInfo.h"
#include "CPU0FixupKinds.h"
#include "CPU0MCTargetDesc.h"
#include "llvm/MC/MCObjectWriter.h"
#include "llvm/MC/MCELFObjectWriter.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCSection.h"
#include "llvm/MC/MCValue.h"
#include "llvm/Support/ErrorHandling.h"

using namespace llvm;

namespace {
  class CPU0ELFObjectWriter : public MCELFObjectTargetWriter {
  public:
    CPU0ELFObjectWriter(uint8_t OSABI);

    ~CPU0ELFObjectWriter() override;

    unsigned getRelocType(MCContext &Ctx, const MCValue &Target,
                          const MCFixup &Fixup, bool IsPCRel) const override;

    bool needsRelocateWithSymbol(const MCSymbol &Sym,
                                 unsigned Type) const override;
  };
}

CPU0ELFObjectWriter::CPU0ELFObjectWriter(uint8_t OSABI)
  : MCELFObjectTargetWriter(/* is 64 bit */ false,
                            OSABI,
                            ELF::EM_CPU0,
                            /* has relocation added */ false) {}

CPU0ELFObjectWriter::~CPU0ELFObjectWriter() {}


unsigned CPU0ELFObjectWriter::getRelocType(MCContext &Ctx,
                                           const MCValue &Target,
                                           const MCFixup &Fixup,
                                           bool IsPCRel) const {
  unsigned Type = (unsigned) ELF::R_CPU0_NONE;
  unsigned Kind = (unsigned) Fixup.getKind();

  switch (Kind) {
  default:
    llvm_unreachable("invalid fixup kind!");

  case FK_Data_4:
    Type = ELF::R_CPU0_32;
    break;

  case CPU0::fixup_CPU0_32:
    Type = ELF::R_CPU0_32;
    break;

  case CPU0::fixup_CPU0_GPREL16:
    Type = ELF::R_CPU0_GPREL16;
    break;
  case CPU0::fixup_CPU0_GOT:
    Type = ELF::R_CPU0_GOT16;
    break;
  case CPU0::fixup_CPU0_HI16:
    Type = ELF::R_CPU0_HI16;
    break;
  case CPU0::fixup_CPU0_LO16:
    Type = ELF::R_CPU0_LO16;
    break;
  case CPU0::fixup_CPU0_GOT_HI16:
    Type = ELF::R_CPU0_GOT_HI16;
    break;
  case CPU0::fixup_CPU0_GOT_LO16:
    Type = ELF::R_CPU0_GOT_LO16;
    break;
  }

  return Type;
}

bool
CPU0ELFObjectWriter::needsRelocateWithSymbol(const MCSymbol &Sym,
                                             unsigned Type) const {
  switch (Type) {
  default:
    return true;

  case ELF::R_CPU0_GOT16:
    return true;

  case ELF::R_CPU0_HI16:
  case ELF::R_CPU0_LO16:
  case ELF::R_CPU0_32:
    return true;

  case ELF::R_CPU0_GPREL16:
    return false;
  }
}


namespace llvm {
std::unique_ptr<MCObjectTargetWriter> createCPU0ELFObjectWriter(uint8_t OSABI) {
  return std::make_unique<CPU0ELFObjectWriter>(OSABI);
}
}
