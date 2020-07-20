#ifndef LLVM_LIB_TARGET_CPU0_CPU0MCINSTLOWER_H_
#define LLVM_LIB_TARGET_CPU0_CPU0MCINSTLOWER_H_

#include "MCTargetDesc/CPU0MCExpr.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/CodeGen/MachineOperand.h"
#include "llvm/Support/Compiler.h"

namespace llvm {
  class MCContext;
  class MCInst;
  class MCOperand;
  class MachineInstr;
  class MachineFunction;
  class CPU0AsmPrinter;

  // Lower an MachineInstr into an MCInst
  class LLVM_LIBRARY_VISIBILITY CPU0MCInstLower {
    typedef MachineOperand::MachineOperandType MachineOperandType;
    MCContext *Ctx;
    CPU0AsmPrinter &AsmPrinter;
  public:
    CPU0MCInstLower(CPU0AsmPrinter &AP);
    void Initialize(MCContext *C);
    void Lower(const MachineInstr *MI, MCInst &OutMI) const;
    MCOperand LowerOperand(const MachineOperand &MO, unsigned Offset = 0) const;
  };
}

#endif
