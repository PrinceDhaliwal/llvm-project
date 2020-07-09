
#include "CPU0AsmPrinter.h"
#include "MCTargetDesc/CPU0InstPrinter.h"

#include "MCTargetDesc/CPU0BaseInfo.h"
#include "CPU0.h"
#include "CPU0InstrInfo.h"
#include "llvm/ADT/SmallString.h"
#include "llvm/ADT/StringExtras.h"
#include "llvm/ADT/Twine.h"
#include "llvm/CodeGen/MachineConstantPool.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/CodeGen/MachineMemOperand.h"
#include "llvm/CodeGen/AsmPrinter.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Mangler.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Target/TargetLoweringObjectFile.h"
#include "llvm/Target/TargetOptions.h"


using namespace llvm;

#define DEBUG_TYPE "cpu0-asm-printer"

bool CPU0AsmPrinter::runOnMachineFunction(MachineFunction &MF) {
  CPU0FI = MF.getInfo<CPU0FunctionInfo>();
  AsmPrinter::runOnMachineFunction(MF);
  return true;
}


void CPU0AsmPrinter::emitInstruction(const MachineInstr *MI) {
  if (MI->isDebugValue()) {
    SmallString<128> Str;
    raw_svector_ostream OS(Str);

    PrintDebugValueComment(MI, OS);
    return;
  }

  auto I = MI->getIterator();
  auto E = MI->getParent()->instr_end();

  do {
    if (I->isPseudo())
      llvm_unreachable("Pseudo opcode found in emitInstruction()");

    MCInst TmpInst0;
    MCInstLowering.Lower(&*I, TmpInst0);
    OutStreamer->emitInstruction(TmpInst0, getSubtargetInfo());
  } while ((++I != E) && I->isInsideBundle());
}

//===---------------------------------------------------------------------===//
// CPU0 Asm Directives
//
//
// -- Frame Directive describes the stack frame
//    StackPointer, StackSize, RARegister
//
// -- Mask Directives "(f)mask bitmask, offset"
// Tells the assembler which registers are saved and where
// bitmask - contain a little endian bitset indicating which registers are
//           saved on function prologue
// offset  - the position before stack pointer subtraction indicating where
//           the first saved register on prologue is located
//
//
// Consider the following function prologue
//
//    .frame $fp, 48, $ra
//    .mask  0xc0000000, -8
//      addiu $sp, $sp, -58
//      st    $ra, 40($sp)
//      st    $fp, 36($sp)
//
//    With a 0xc0000000 mask, the assembler knows the register 31 (RA) and 30(FP)
//    are saved at prologue. As the save order on prologue is from left to right,
//    RA is saved first. A -8 offset means that after the stack pointer subtraction
//    the first register in the mask (RA) will be saved at address 48-8 = 40.
//===---------------------------------------------------------------------===//

// Create a bitmask with all callee saved registers for CPU or Floating Point
// registers. For CPU registers consider LR, GP and GP for saving if necessary.
void CPU0AsmPrinter::printSavedRegsBitMask(raw_ostream &O) {
  unsigned CPUBitmask = 0;
  int CPUTopSavedRegOff;

  const MachineFrameInfo *MFI = &MF->getFrameInfo();
  const TargetRegisterInfo *TRI = MF->getSubtarget().getRegisterInfo();
  const std::vector<CalleeSavedInfo> &CSI = MFI->getCalleeSavedInfo();

  // size of stack area to which FP callee-saved regs are saved.
  unsigned CPURegSize = CPU0::CPURegsRegClass.getNumRegs();
  unsigned i = 0, e = CSI.size();

  for (; i != e; ++i) {
    unsigned Reg = CSI[i].getReg();
    unsigned RegNum = TRI->getEncodingValue(Reg);
    CPUBitmask |= 1 << RegNum;
  }

  CPUTopSavedRegOff = CPUBitmask ? -CPURegSize : 0;
  O << "\t.mask \t"; printHex32(CPUBitmask, O);
  O << "," << Twine(CPUTopSavedRegOff) << "\n";
}

// Print a 32 bit hex number with all numbers.
void CPU0AsmPrinter::printHex32(unsigned Value, raw_ostream &O) {
  O << "0x";
  for (int i = 7; i >= 0; i--)
    O.write_hex((Value & (0xF << (i*4))) >> (i*4));
}

//===---------------------------------------------------------------------===//
// Frame and Set directives
//===---------------------------------------------------------------------===//
void CPU0AsmPrinter::emitFrameDirective() {
  const TargetRegisterInfo &RI = *MF->getSubtarget().getRegisterInfo();

  unsigned stackReg = RI.getFrameRegister(*MF);
  unsigned returnReg = RI.getRARegister();
  unsigned stackSize = MF->getFrameInfo().getStackSize();

  if (OutStreamer->hasRawTextSupport())
    OutStreamer->emitRawText("\t.frame\t$" +
                             StringRef(CPU0InstPrinter::getRegisterName(stackReg)).lower() +
                             "," + Twine(stackSize) + ",$" +
                             StringRef(CPU0InstPrinter::getRegisterName(returnReg)).lower());
}

/// emit Set directives
const char *CPU0AsmPrinter::getCurrentABIString() const {
  switch (static_cast<CPU0TargetMachine &>(TM).getABI().GetEnumValue()) {
  case CPU0ABIInfo::ABI::O32: return "abiO32";
  case CPU0ABIInfo::ABI::S32: return "abiS32";
  default: llvm_unreachable("Unknown CPU0 ABI");
  }
}

void CPU0AsmPrinter::emitFunctionEntryLabel()  {
  if (OutStreamer->hasRawTextSupport())
    OutStreamer->emitRawText("\t.ent\t" + Twine(CurrentFnSym->getName()));
  OutStreamer->emitLabel(CurrentFnSym);
}

void CPU0AsmPrinter::emitFunctionBodyStart() {
  MCInstLowering.Initialize(&MF->getContext());

  emitFrameDirective();
  if (OutStreamer->hasRawTextSupport()) {
    SmallString<128> Str;
    raw_svector_ostream OS(Str);
    printSavedRegsBitMask(OS);
    OutStreamer->emitRawText(OS.str());
    OutStreamer->emitRawText(StringRef("\t.set\tnoreorder"));
    OutStreamer->emitRawText(StringRef("\t.set\tnomacro"));
    if (CPU0FI->getEmitNOAT())
      OutStreamer->emitRawText(StringRef("\t.set\tnoat"));
  }
}

void CPU0AsmPrinter::emitFunctionBodyEnd() {
  if (OutStreamer->hasRawTextSupport()) {
    if (CPU0FI->getEmitNOAT()) {
      OutStreamer->emitRawText(StringRef("\t.set\t\at"));
    }

    OutStreamer->emitRawText(StringRef("\t.set\tmacro"));
    OutStreamer->emitRawText(StringRef("\t.set\treorder"));
    OutStreamer->emitRawText("\t.end\t" + Twine(CurrentFnSym->getName()));
  }
}


// .section .mdebug.abi32
// .previous
void CPU0AsmPrinter::emitStartOfAsmFile(Module &M) {
  // Tell the assembler which ABI we are using
  if (OutStreamer->hasRawTextSupport())
    OutStreamer->emitRawText("\t.section .mdebug." +
                            Twine(getCurrentABIString()));

  // return to previous section
  if (OutStreamer->hasRawTextSupport())
    OutStreamer->emitRawText(StringRef("\t.previous"));
}

void CPU0AsmPrinter::PrintDebugValueComment(const MachineInstr *MI,
                                            raw_ostream &OS) {
  // TODO
  OS << "PrintDebugValueComment()";
}

extern "C" void LLVMInitializeCPU0AsmPrinter() {
  RegisterAsmPrinter<CPU0AsmPrinter> X(TheCPU0Target);
  RegisterAsmPrinter<CPU0AsmPrinter> Y(TheCPU0elTarget);
}
