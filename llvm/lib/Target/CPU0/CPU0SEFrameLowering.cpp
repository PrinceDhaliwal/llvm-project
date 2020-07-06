#include "CPU0SEFrameLowering.h"

#include "CPU0MachineFunction.h"
#include "CPU0SEInstrInfo.h"
#include "CPU0Subtarget.h"

#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/RegisterScavenging.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Target/TargetOptions.h"


using namespace llvm;

CPU0SEFrameLowering::CPU0SEFrameLowering(const CPU0Subtarget &STI)
  : CPU0FrameLowering(STI, STI.stackAlignment()) {}

void CPU0SEFrameLowering::emitPrologue(MachineFunction &MF,
                                       MachineBasicBlock &MBB) const {
}
void CPU0SEFrameLowering::emitEpilogue(MachineFunction &MF,
                                       MachineBasicBlock &MBB) const {
}

const CPU0FrameLowering *
llvm::createCPU0SEFrameLowering(const CPU0Subtarget &ST) {
  return new CPU0SEFrameLowering(ST);
}
