#include "CPU0FrameLowering.h"

#include "CPU0InstrInfo.h"
#include "CPU0MachineFunction.h"
#include "CPU0Subtarget.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Target/TargetOptions.h"

using namespace llvm;

//- emitPrologue and emitEpiLogue must exist for main().

//===------------------------------------------------------===//
// Stack Frame Processing methods
// The stack is allocated decrementing the stack pointer on the first
// instruction of a function prologue. Once decremented, all stack
// references are done through a positive offset from the stack/frame
// pointer, so the stack is considering to grow up!

// The stack frame required by the ABI (after call):
// Offset
//
// 0               -----------
// 4               Args to pass
// .               saved $GP (used in PIC)
// .               Alloca allocations
// .               Local Area
// .               CPU *Callee Saved* Registers
// .               saved FP
// .               saved RA
// .               FPU "Callee Saved" Registers
// StackSize       ------------

const CPU0FrameLowering *CPU0FrameLowering::create(const CPU0Subtarget &ST) {
  return llvm::createCPU0SEFrameLowering(ST);
}

bool CPU0FrameLowering::hasFP(const MachineFunction &MF) const {
  const MachineFrameInfo &MFI = MF.getFrameInfo();
  const TargetRegisterInfo *TRI = STI.getRegisterInfo();

  return MF.getTarget().Options.DisableFramePointerElim(MF) ||
    MFI.hasVarSizedObjects() || MFI.isFrameAddressTaken() ||
    TRI->needsStackRealignment(MF);
}


