#include "CPU0.h"

#include "CPU0MCExpr.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCAssembler.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCObjectStreamer.h"
#include "llvm/MC/MCSymbolELF.h"

using namespace llvm;

#define DEBUG_TYPE "cpu0mcexpr"

const CPU0MCExpr *CPU0MCExpr::create(CPU0MCExpr::CPU0ExprKind Kind,
                                     const MCExpr *Expr, MCContext &Ctx) {
  return new (Ctx) CPU0MCExpr(Kind, Expr);
}

const CPU0MCExpr *CPU0MCExpr::create(const MCSymbol *Symbol, CPU0MCExpr::CPU0ExprKind Kind,
                         MCContext &Ctx) {
  const MCSymbolRefExpr *MCSym =
      MCSymbolRefExpr::create(Symbol, MCSymbolRefExpr::VK_None, Ctx);
  return new (Ctx) CPU0MCExpr(Kind, MCSym);
}

const CPU0MCExpr *CPU0MCExpr::createGpOff(CPU0MCExpr::CPU0ExprKind Kind,
                                          const MCExpr *Expr, MCContext &Ctx) {
  return create(Kind, create(CEK_None, create(CEK_GPREL, Expr, Ctx), Ctx), Ctx);
}

void CPU0MCExpr::printImpl(raw_ostream &OS, const MCAsmInfo *MAI) const {
  int64_t AbsVal;

  switch (Kind) {
  case CEK_None:
  case CEK_Special:
    llvm_unreachable("CEK_None and CEK_Special are invalid");
    break;
  case CEK_CALL_HI16:
    OS << "%call_hi";
    break;
  case CEK_CALL_LO16:
    OS << "%call_lo";
    break;
  case CEK_DTP_HI:
    OS << "%dtp_hi";
    break;
  case CEK_DTP_LO:
    OS << "%dtp_lo";
    break;
  case CEK_GOT:
    OS << "%got";
    break;
  case CEK_GOTTPREL:
    OS << "%gottprel";
    break;
  case CEK_GOT_CALL:
    OS << "%call16";
    break;
  case CEK_GOT_DISP:
    OS << "%got_disp";
    break;
  case CEK_GOT_HI16:
    OS << "%got_hi";
    break;
  case CEK_GOT_LO16:
    OS << "%got_lo";
    break;
  case CEK_GPREL:
    OS << "%gp_rel";
    break;
  case CEK_ABS_HI:
    OS << "%hi";
    break;
  case CEK_ABS_LO:
    OS << "%lo";
    break;
  case CEK_TLSGD:
    OS << "%tlsgd";
    break;
  case CEK_TLSLDM:
    OS << "%tlsldm";
    break;
  case CEK_TP_HI:
    OS << "%tp_hi";
    break;
  case CEK_TP_LO:
    OS << "%tp_lo";
    break;
  }

  OS << '(';
  if (Expr->evaluateAsAbsolute(AbsVal))
    OS << AbsVal;
  else
    Expr->print(OS, MAI, true);
  OS << ')';
}

bool
CPU0MCExpr::evaluateAsRelocatableImpl(MCValue &Res,
                                      const MCAsmLayout *Layout,
                                      const MCFixup *Fixup) const {
  return getSubExpr()->evaluateAsRelocatable(Res, Layout, Fixup);
}

void CPU0MCExpr::visitUsedExpr(MCStreamer &Streamer) const {
  Streamer.visitUsedExpr(*getSubExpr());
}

void CPU0MCExpr::fixELFSymbolsInTLSFixups(MCAssembler &Asm) const {
  switch (getKind()) {
  case CEK_None:
  case CEK_Special:
    llvm_unreachable("CEK_None and CEK_Special are invalid");
    break;
  case CEK_CALL_HI16:
  case CEK_CALL_LO16:
    break;
  }
}

bool CPU0MCExpr::isGpOff(CPU0ExprKind &Kind) const {
  if (const CPU0MCExpr *S1 = dyn_cast<const CPU0MCExpr>(getSubExpr())) {
    if (const CPU0MCExpr *S2 = dyn_cast<const CPU0MCExpr>(S1->getSubExpr())) {
      if (S1->getKind() == CEK_None && S2->getKind() == CEK_GPREL) {
        Kind = getKind();
        return true;
      }
    }
  }
  return false;
}
