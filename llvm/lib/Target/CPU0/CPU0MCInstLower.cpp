#include "CPU0MCInstLower.h"

#include "CPU0AsmPrinter.h"
#include "CPU0InstrInfo.h"
#include "MCTargetDesc/CPU0BaseInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/CodeGen/MachineOperand.h"
#include "llvm/IR/Mangler.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"

using namespace llvm;

CPU0MCInstLower::CPU0MCInstLower(CPU0AsmPrinter &AP)
  : AsmPrinter(AP) {}

void CPU0MCInstLower::Initialize(MCContext *C) { Ctx = C; }

static void CreateMCInst(MCInst &Inst, unsigned Opc, const MCOperand &Op0,
                         const MCOperand &Op1,
                         const MCOperand &Op2 = MCOperand()) {
  Inst.setOpcode(Opc);
  Inst.addOperand(Op0);
  Inst.addOperand(Op1);
  if (Op2.isValid())
    Inst.addOperand(Op2);
}

MCOperand CPU0MCInstLower::LowerOperand(const MachineOperand &MO,
                                        unsigned Offset) const {
  MachineOperandType MOT = MO.getType();

  switch (MOT) {
  default: llvm_unreachable("unknown operand type");
  case MachineOperand::MO_Register:
    if (MO.isImplicit()) break;
    return MCOperand::createReg(MO.getReg());
  case MachineOperand::MO_Immediate:
    return MCOperand::createImm(MO.getImm() + Offset);
  case MachineOperand::MO_RegisterMask:
    break;
  }

  return MCOperand();
}

void CPU0MCInstLower::Lower(const MachineInstr *MI, MCInst &OutMI) const {
  OutMI.setOpcode(MI->getOpcode());

  for (unsigned i = 0, e = MI->getNumOperands(); i != e; ++i) {
    const MachineOperand &MO = MI->getOperand(i);
    MCOperand MCO = LowerOperand(MO);
    if (MCO.isValid())
      OutMI.addOperand(MCO);
  }
}
