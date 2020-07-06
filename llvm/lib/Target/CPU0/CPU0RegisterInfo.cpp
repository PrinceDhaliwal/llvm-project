#define DEBUG_TYPE "cpu0-reg-info"

#include "CPU0RegisterInfo.h"
#include "CPU0.h"
#include "CPU0Subtarget.h"
#include "CPU0MachineFunction.h"

#include "llvm/IR/Function.h"
#include "llvm/IR/Type.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"

#define GET_REGINFO_TARGET_DESC
#include "CPU0GenRegisterInfo.inc"

using namespace llvm;

CPU0RegisterInfo::CPU0RegisterInfo(const CPU0Subtarget &ST)
  : CPU0GenRegisterInfo(CPU0::LR), Subtarget(ST) {}

// Callee Saved Registers methods
const MCPhysReg *
CPU0RegisterInfo::getCalleeSavedRegs(const MachineFunction *MF) const {
  return CSR_O32_SaveList;
}

const uint32_t *CPU0RegisterInfo::getCallPreservedMask(const MachineFunction &MF,
                                                       CallingConv::ID) const {
  return CSR_O32_RegMask;
}

BitVector CPU0RegisterInfo::getReservedRegs(const MachineFunction &MF) const {
  static const uint16_t ReservedCPURegs[] = {
                                             CPU0::ZERO,
                                             CPU0::AT,
                                             CPU0::SP,
                                             CPU0::LR,
                                             CPU0::PC
  };

  BitVector Reserved(getNumRegs());

  for (unsigned I = 0; I < array_lengthof(ReservedCPURegs); ++I)
    Reserved.set(ReservedCPURegs[I]);

  return Reserved;
}

void CPU0RegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator II,
                                           int SPAdj, unsigned FIOperandNum,
                                           RegScavenger *RS) const {
}

bool CPU0RegisterInfo::requiresRegisterScavenging(const MachineFunction &MF) const {
  return true;
}

bool CPU0RegisterInfo::trackLivenessAfterRegAlloc(const MachineFunction &MF) const {
  return true;
}

Register CPU0RegisterInfo::getFrameRegister(const MachineFunction &MF) const {
  const TargetFrameLowering *TFI = MF.getSubtarget().getFrameLowering();
  return TFI->hasFP(MF) ? (CPU0::FP) : (CPU0::SP);
}

