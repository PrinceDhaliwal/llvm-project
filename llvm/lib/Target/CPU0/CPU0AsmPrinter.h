#ifndef LLVM_LIB_TARGET_CPU0_CPU0ASMPRINTER_H_
#define LLVM_LIB_TARGET_CPU0_CPU0ASMPRINTER_H_

#include "CPU0MachineFunction.h"
#include "CPU0MCInstLower.h"
#include "CPU0Subtarget.h"
#include "CPU0TargetMachine.h"
#include "llvm/CodeGen/AsmPrinter.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/Support/Compiler.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {
  class MCStreamer;
  class MachineInstr;
  class MachineBasicBlock;
  class Module;
  class raw_ostream;

  class LLVM_LIBRARY_VISIBILITY CPU0AsmPrinter : public AsmPrinter {
    void EmitInstrWithMacroNoAT(const MachineInstr *MI);
  private:
    /// Convert a MachineOperand into the equivalent MCOperand
    bool lowerOperand(const MachineOperand &MO, MCOperand &MCOp);

  public:
    const CPU0Subtarget *Subtarget;
    const CPU0FunctionInfo *CPU0FI;
    CPU0MCInstLower MCInstLowering;

    explicit CPU0AsmPrinter(TargetMachine &TM,
                            std::unique_ptr<MCStreamer> Streamer)
      : AsmPrinter(TM, std::move(Streamer)),
      MCInstLowering(*this) {
      Subtarget = static_cast<CPU0TargetMachine &>(TM).getSubtargetImpl();
    }

    virtual StringRef getPassName() const override {
      return "CPU0 Assembly Printer";
    }

    virtual bool runOnMachineFunction(MachineFunction &MF) override;


    void emitInstruction(const MachineInstr *MI) override;
    void printSavedRegsBitMask(raw_ostream &O);
    void printHex32(unsigned int Value, raw_ostream &O);
    void emitFrameDirective();
    const char *getCurrentABIString() const;
    void emitFunctionEntryLabel() override;
    void emitFunctionBodyStart() override;
    void emitFunctionBodyEnd() override;
    void emitStartOfAsmFile(Module &M) override;
    void PrintDebugValueComment(const MachineInstr *MI, raw_ostream &OS);
  };
}

#endif
