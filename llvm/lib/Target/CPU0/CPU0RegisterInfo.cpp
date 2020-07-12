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
  MachineInstr &MI = *II;
  MachineFunction &MF = *MI.getParent()->getParent();
  MachineFrameInfo &MFI = MF.getFrameInfo();
  CPU0FunctionInfo *CPU0FI = MF.getInfo<CPU0FunctionInfo>();

  unsigned I = 0;
  while (!MI.getOperand(I).isFI()) {
    ++I;
    assert(I < MI.getNumOperands() &&
           "Instr doesn't have FrameIndex operand!");
  }

  // DEBUG(errs() << "\nFunction: " << MF.getFunction().getName() << "\n"
  //       << "<----------->\n" << MI);

  int FrameIndex = MI.getOperand(I).getIndex();

  uint64_t stackSize = MF.getFrameInfo().getStackSize();
  int64_t spOffset = MF.getFrameInfo().getObjectOffset(FrameIndex);

  // DEBUG(errs() << "FrameIndex : " << FrameIndex << "\n"
  //       << "spOffset : " << spOffset << "\n"
  //       << "stackSize : " << stackSize << "\n");

  const std::vector<CalleeSavedInfo> &CSI = MFI.getCalleeSavedInfo();
  int MinCSFI = 0;
  int MaxCSFI = -1;

  if (CSI.size()) {
    MinCSFI = CSI[0].getFrameIdx();
    MaxCSFI = CSI[CSI.size() - 1].getFrameIdx();
  }

  // The following stack frame objects are always reference relative to $sp:
  //   1. Outgoing arguments.
  //   2. Pointer to dynamically allocated stack space.
  //   3. Locations for callee-saved registers.
  // Everything else is referenced relative to whatever register
  // getFrameRegister() returns.
  unsigned FrameReg;

  FrameReg = CPU0::SP;


  // Calculate final offset.
  // - There is no need to change the offset if the frame object is one of the
  //   following: an outgoing argument, pointer to a dynamically allocated
  //   stack space or a $gp restore location,
  // - If the frame object is any of the following, its offset must be adjusted
  //   by adding the size of the stack:
  //   incoming argument, callee-saved register location or local variable.
  int64_t Offset;
  Offset = spOffset + (int64_t)stackSize;

  Offset += MI.getOperand(I+1).getImm();

//  DEBUG(errs() << "Offset : " << Offset << "\n" << "<---------->\n");

  // If MI is not a debug value, make sure Offset fits in the 16-bit
  // immediate field.
  if (!MI.isDebugValue() && !isInt<16>(Offset)) {
    assert("!MI.isDebugValue() && !isInt<16>(Offset)");
  }

  MI.getOperand(I).ChangeToRegister(FrameReg, false);
  MI.getOperand(I+1).ChangeToImmediate(Offset);
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

