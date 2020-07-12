#include "CPU0InstrInfo.h"

#include "CPU0TargetMachine.h"
#include "CPU0MachineFunction.h"

#include "llvm/ADT/STLExtras.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

#define GET_INSTRINFO_CTOR_DTOR
#include "CPU0GenInstrInfo.inc"

// Pin the vtable to this file.
void CPU0InstrInfo::anchor() {}

CPU0InstrInfo::CPU0InstrInfo(const CPU0Subtarget &STI)
  : Subtarget(STI) {}

const CPU0InstrInfo *CPU0InstrInfo::create(CPU0Subtarget &STI) {
  return llvm::createCPU0SEInstrInfo(STI);
}

unsigned CPU0InstrInfo::getInstSizeInBytes(const MachineInstr &MI) const {
  return MI.getDesc().getSize();
}

MachineMemOperand *
CPU0InstrInfo::GetMemOperand(MachineBasicBlock &MBB, int FI,
                             MachineMemOperand::Flags Flags) const {
  MachineFunction &MF = *MBB.getParent();
  MachineFrameInfo &MFI = MF.getFrameInfo();
  Align align = MFI.getObjectAlign(FI);

  return MF.getMachineMemOperand(MachinePointerInfo::getFixedStack(MF, FI),
                                 Flags, MFI.getObjectSize(FI), align);
}
