// copied
#ifndef LLVM_LIB_TARGET_CPU0_MCTARGETDESC_CPU0MCEXPR_H
#define LLVM_LIB_TARGET_CPU0_MCTARGETDESC_CPU0MCEXPR_H

#include "llvm/MC/MCAsmLayout.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCValue.h"

namespace llvm {

class CPU0MCExpr : public MCTargetExpr {
public:
  enum CPU0ExprKind {
    CEK_None,
    CEK_ABS_HI,
    CEK_ABS_LO,
    CEK_CALL_HI16,
    CEK_CALL_LO16,
    CEK_DTP_HI,
    CEK_DTP_LO,
    CEK_GOT,
    CEK_GOTTPREL,
    CEK_GOT_CALL,
    CEK_GOT_DISP,
    CEK_GOT_HI16,
    CEK_GOT_LO16,
    CEK_GPREL,
    CEK_TLSGD,
    CEK_TLSLDM,
    CEK_TP_HI,
    CEK_TP_LO,
    CEK_Special,
  };

private:
  const CPU0ExprKind Kind;
  const MCExpr *Expr;

  explicit CPU0MCExpr(CPU0ExprKind Kind, const MCExpr *Expr)
    : Kind(Kind), Expr(Expr) {}

public:
  static const CPU0MCExpr *create(CPU0ExprKind Kind, const MCExpr *Expr,
                                  MCContext &Ctx);
  static const CPU0MCExpr *create(const MCSymbol *Symbol, 
                                  CPU0MCExpr::CPU0ExprKind Kind, MCContext &Ctx);
  static const CPU0MCExpr *createGpOff(CPU0ExprKind Kind, const MCExpr *Expr,
                                       MCContext &Ctx);

  /// Get the kind of this expression.
  CPU0ExprKind getKind() const { return Kind; }

  /// Get the child of this expression.
  const MCExpr *getSubExpr() const { return Expr; }

  void printImpl(raw_ostream &OS, const MCAsmInfo *MAI) const override;
  bool evaluateAsRelocatableImpl(MCValue &Res, const MCAsmLayout *Layout,
                                 const MCFixup *Fixup) const override;
  void visitUsedExpr(MCStreamer &Streamer) const override;
  MCFragment *findAssociatedFragment() const override {
    return getSubExpr()->findAssociatedFragment();
  }

  void fixELFSymbolsInTLSFixups(MCAssembler &Asm) const override;

  static bool classof(const MCExpr *E) {
    return E->getKind() == MCExpr::Target;
  }

  bool isGpOff(CPU0ExprKind &Kind) const;
  bool isGpOff() const {
    CPU0ExprKind Kind;
    return isGpOff(Kind);
  }
};
} // end namespace llvm

#endif
