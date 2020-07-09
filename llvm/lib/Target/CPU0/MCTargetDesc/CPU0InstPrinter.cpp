#include "CPU0InstPrinter.h"

#include "CPU0InstrInfo.h"
#include "llvm/ADT/StringExtras.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

#define DEBUG_TYPE "asm-printer"

#define PRINT_ALIAS_INSTR
#include "CPU0GenAsmWriter.inc"

void CPU0InstPrinter::printRegName(raw_ostream &OS, unsigned RegNo) const {
  OS << "$" << StringRef(getRegisterName(RegNo)).lower();
}

void CPU0InstPrinter::printInst(const MCInst *MI, uint64_t Address,
                                StringRef Annot, const MCSubtargetInfo &STI, raw_ostream &O) {
  if (!printAliasInstr(MI, Address, O))
    printInstruction(MI, Address, O);
  printAnnotation(O, Annot);
}

void CPU0InstPrinter::printOperand(const MCInst *MI, unsigned OpNo,
                                   raw_ostream &O) {
  const MCOperand &Op = MI->getOperand(OpNo);
  if (Op.isReg()) {
    printRegName(O, Op.getReg());
    return;
  }

  if (Op.isImm()) {
    O << Op.getImm();
    return;
  }

  assert(Op.isExpr() && "unknown operand kind in printOperand");
  Op.getExpr()->print(O, &MAI, true);
}

void CPU0InstPrinter::printUnsignedImm(const MCInst *MI, int OpNum,
                                      raw_ostream &O) {
  const MCOperand &MO = MI->getOperand(OpNum);
  if (MO.isImm()) {
    O << (unsigned short int)MO.getImm();
  } else {
    printOperand(MI, OpNum, O);
  }
}

void CPU0InstPrinter::
printMemOperand(const MCInst *MI, int OpNum, raw_ostream &O) {
  printOperand(MI, OpNum+1, O);
  O << "(";
  printOperand(MI, OpNum, O);
  O << ")";
}

void CPU0InstPrinter::
printMemOperandEA(const MCInst *MI, int opNum, raw_ostream &O) {
  // when using stack locations for not load/store instructions
  // print the same way as all normal 3 operand instructions.
  printOperand(MI, opNum, O);
  O << ", ";
  printOperand(MI, opNum+1, O);
  return;
}

