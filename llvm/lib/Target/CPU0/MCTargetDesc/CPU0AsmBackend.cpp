#include "CPU0FixupKinds.h"
#include "CPU0AsmBackend.h"

#include "CPU0MCTargetDesc.h"
#include "llvm/MC/MCAsmBackend.h"
#include "llvm/MC/MCAssembler.h"
#include "llvm/MC/MCDirectives.h"
#include "llvm/MC/MCELFObjectWriter.h"
#include "llvm/MC/MCFixupKindInfo.h"
#include "llvm/MC/MCObjectWriter.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

static unsigned adjustFixupValue(const MCFixup &Fixup, uint64_t Value,
                                 MCContext *Ctx = nullptr) {
  unsigned Kind = Fixup.getKind();

  switch (Kind) {
  default:
    return 0;

  case FK_GPRel_4:
  case FK_Data_4:
  case CPU0::fixup_CPU0_LO16:
    break;
  case CPU0::fixup_CPU0_HI16:
  case CPU0::fixup_CPU0_GOT:
    // get the higher 16-bits. Also add 1 if bit 15 is 1
    Value = ((Value + 0x8000) >> 16) & 0xffff;
    break;
  }

  return Value;
}

std::unique_ptr<MCObjectTargetWriter>
CPU0AsmBackend::createObjectTargetWriter() const {
  return createCPU0ELFObjectWriter(MCELFObjectTargetWriter::getOSABI(OSType));

}

/// Apply the Value for given Fixup into the provided data fragment, at the
/// offset specifid by the fixup and following the fixup kind as appropriate.
void CPU0AsmBackend::applyFixup(const MCAssembler &Asm,
                                const MCFixup &Fixup, const MCValue &Target,
                                MutableArrayRef<char> Data,
                                uint64_t Value,
                                bool IsResolved, const MCSubtargetInfo *STI) const {
  MCFixupKind Kind = Fixup.getKind();
  Value = adjustFixupValue(Fixup, Value);

  if (!Value)
    return;

  unsigned Offset = Fixup.getOffset();
  unsigned NumBytes = (getFixupKindInfo(Kind).TargetSize + 7) / 8;
  unsigned FullSize;

  switch ((unsigned)Kind) {
  default:
    FullSize = 4;
    break;
  }

  
  // Grab current value, if any, from bits.
  uint64_t CurVal = 0;

  for (unsigned i = 0; i != NumBytes; ++i) {
    unsigned Idx = Endian == support::endianness::little ? i : (FullSize - 1 - i);
    CurVal |= (uint64_t)((uint8_t)Data[Offset + Idx]) << (i*8);
  }

  uint64_t Mask = ((uint64_t)(-1) >>
                    (64 - getFixupKindInfo(Kind).TargetSize));
  CurVal |= Value & Mask;

  // Write out the fixed up bytes back to the code/data bits.
  for (unsigned i = 0; i != NumBytes; ++i) {
    unsigned Idx = Endian == support::endianness::little ? i : (FullSize - 1 - i);
    Data[Offset + Idx] = (uint8_t)((CurVal >> (i*8)) & 0xff);
  }
}

const MCFixupKindInfo &CPU0AsmBackend::
getFixupKindInfo(MCFixupKind Kind) const {
  const static MCFixupKindInfo Infos[CPU0::NumTargetFixupKinds] = {
    // This table *must* be in same the order of fixup_* kinds in
    // CPU0FixupKinds.h.
    //
    // name                        offset  bits  flags
    { "fixup_CPU0_32",             0,     32,   0 },
    { "fixup_CPU0_HI16",           0,     16,   0 },
    { "fixup_CPU0_LO16",           0,     16,   0 },
    { "fixup_CPU0_GPREL16",        0,     16,   0 },
    { "fixup_CPU0_GOT",            0,     16,   0 },
    { "fixup_CPU0_GOT_HI16",       0,     16,   0 },
    { "fixup_CPU0_GOT_LO16",       0,     16,   0 }
  };

  if (Kind < FirstTargetFixupKind)
    return MCAsmBackend::getFixupKindInfo(Kind);

  assert(unsigned(Kind - FirstTargetFixupKind) < getNumFixupKinds() &&
         "Invalid kind!");
  return Infos[Kind - FirstTargetFixupKind];
}

/// WriteNopData - Write an (optimal) nop sequence of Count bytes
/// to the given output. If the target cannot generate such a sequence,
/// it should return an error.
///
/// \return - True on success.
bool CPU0AsmBackend::writeNopData(raw_ostream &OS, uint64_t Count) const {
  return true;
}

// MCAsmBackend
MCAsmBackend *llvm::createCPU0AsmBackendEL32(const Target &T,
                                             const MCSubtargetInfo &STI,
                                             const MCRegisterInfo &MRI,
                                             const MCTargetOptions &Opt) {
  return new CPU0AsmBackend(T, Triple::OSType::Linux, /*IsLittle*/true);
}

MCAsmBackend *llvm::createCPU0AsmBackendEB32(const Target &T,
                                             const MCSubtargetInfo &STI,
                                             const MCRegisterInfo &MRI,
                                             const MCTargetOptions &Opt) {
  return new CPU0AsmBackend(T, Triple::OSType::Linux, /*IsLittle*/false);
}
