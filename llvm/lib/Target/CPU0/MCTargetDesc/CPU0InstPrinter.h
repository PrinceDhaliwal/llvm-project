#ifndef LLVM_LIB_TARGET_CPU0_INSTPRINTER_CPU0INSTPRINTER_H
#define LLVM_LIB_TARGET_CPU0_INSTPRINTER_CPU0INSTPRINTER_H

#include "llvm/MC/MCInstPrinter.h"

namespace llvm {
  class TargetMachine;

  class CPU0InstPrinter : public MCInstPrinter {
  public:
    CPU0InstPrinter(const MCAsmInfo &MAI, const MCInstrInfo &MII,
                    const MCRegisterInfo &MRI)
      : MCInstPrinter(MAI, MII, MRI) {}

    ~CPU0InstPrinter() override {}
    // these are autogenerated by tablegen
    void printInstruction(const MCInst *MI, uint64_t Address, raw_ostream &O);
    static const char *getRegisterName(unsigned RegNo);

    void printRegName(raw_ostream &OS, unsigned RegNo) const override;
    void printInst(const MCInst *MI, uint64_t Address, StringRef Annot,
                   const MCSubtargetInfo &STI, raw_ostream &O) override;

    bool printAliasInstr(const MCInst *MI, uint64_t Address, raw_ostream &OS);
    void printCustomAliasOperand(const MCInst *MI, uint64_t Address, unsigned OpIdx,
                                 unsigned PrintMethodIdx, raw_ostream &O);

  private:
    void printOperand(const MCInst *MI, unsigned OpNo, raw_ostream &O);
    void printUnsignedImm(const MCInst *MI, int OpNum, raw_ostream &O);
    void printMemOperand(const MCInst *MI, int OpNum, raw_ostream &O);
    void printMemOperandEA(const MCInst *MI, int opNum, raw_ostream &O);
  };
}

#endif
