#include "CPU0SEFrameLowering.h"

#include "CPU0MachineFunction.h"
#include "CPU0SEInstrInfo.h"
#include "CPU0Subtarget.h"

#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/CodeGen/MachineOperand.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/RegisterScavenging.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/Function.h"
#include "llvm/MC/MachineLocation.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Target/TargetOptions.h"


using namespace llvm;

CPU0SEFrameLowering::CPU0SEFrameLowering(const CPU0Subtarget &STI)
  : CPU0FrameLowering(STI, STI.stackAlignment()) {}

void CPU0SEFrameLowering::emitPrologue(MachineFunction &MF,
                                       MachineBasicBlock &MBB) const {
  assert(&MF.front() == &MBB && "Shrink-wrapping not yet supported");
  MachineFrameInfo *MFI = &MF.getFrameInfo();
  CPU0FunctionInfo *CPU0FI = MF.getInfo<CPU0FunctionInfo>();

  const CPU0SEInstrInfo &TII =
    *static_cast<const CPU0SEInstrInfo *>(STI.getInstrInfo());
  const CPU0RegisterInfo &RegInfo =
    *static_cast<const CPU0RegisterInfo *>(STI.getRegisterInfo());

  MachineBasicBlock::iterator MBBI = MBB.begin();
  DebugLoc dl = MBBI != MBB.end() ? MBBI->getDebugLoc() : DebugLoc();
  CPU0ABIInfo ABI = STI.getABI();
  unsigned SP = CPU0::SP;

  const TargetRegisterClass *RC = &CPU0::GPROutRegClass;

  // First, compute final stack size
  uint64_t StackSize = MFI->getStackSize();

  // No need to allocate space on the stack if stack size is 0
  if (StackSize == 0 && !MFI->adjustsStack()) return;

  MachineModuleInfo &MMI = MF.getMMI();
  const MCRegisterInfo *MRI = MMI.getContext().getRegisterInfo();
  MachineLocation DstML, SrcML;

  // Adjust stack
  TII.adjustStackPtr(SP, -StackSize, MBB, MBBI);

  // emit ".cfi_def_cfa_offset StackSize"
  unsigned CFIIndex = MF.addFrameInst(MCCFIInstruction::cfiDefCfaOffset(nullptr, -StackSize));

  BuildMI(MBB, MBBI, dl, TII.get(TargetOpcode::CFI_INSTRUCTION))
    .addCFIIndex(CFIIndex);

  const std::vector<CalleeSavedInfo> &CSI = MFI->getCalleeSavedInfo();

  if (CSI.size()) {
    // Find the instruction past the last instruction that saves a callee-saved
    // register to the stack.
    for (unsigned i = 0; i < CSI.size(); ++i)
      ++MBBI;

    // Iterate over list of callee-saved registers and emit .cfi_offset
    // directives
    for (std::vector<CalleeSavedInfo>::const_iterator I = CSI.begin(),
	   E = CSI.end(); I != E; ++I) {
      int64_t Offset = MFI->getObjectOffset(I->getFrameIdx());
      unsigned Reg = I->getReg();
      {
        // Reg is in CPURegs
        unsigned CFIIndex = MF.addFrameInst(MCCFIInstruction::
                                             createOffset(nullptr,
                                                          MRI->getDwarfRegNum(Reg, 1),
                                                          Offset));

        BuildMI(MBB, MBBI, dl, TII.get(TargetOpcode::CFI_INSTRUCTION))
          .addCFIIndex(CFIIndex);
      }
    }
  }
}

void CPU0SEFrameLowering::emitEpilogue(MachineFunction &MF,
                                       MachineBasicBlock &MBB) const {
  MachineBasicBlock::iterator MBBI = MBB.getLastNonDebugInstr();
  MachineFrameInfo *MFI = &MF.getFrameInfo();
  CPU0FunctionInfo *CPU0FI = MF.getInfo<CPU0FunctionInfo>();

  const CPU0SEInstrInfo &TII =
    *static_cast<const CPU0SEInstrInfo *>(STI.getInstrInfo());
  const CPU0RegisterInfo &RegInfo =
    *static_cast<const CPU0RegisterInfo *>(STI.getRegisterInfo());

  DebugLoc dl = MBBI->getDebugLoc();
  CPU0ABIInfo ABI = STI.getABI();
  unsigned SP = CPU0::SP;

  // Get the number of bytes from FrameInfo
  uint64_t StackSize = MFI->getStackSize();

  if (!StackSize)
    return;

  // Adjust stack
  TII.adjustStackPtr(SP, StackSize, MBB, MBBI);
}

bool
CPU0SEFrameLowering::hasReservedCallFrame(const MachineFunction &MF) const {
  const MachineFrameInfo *MFI = &MF.getFrameInfo();

  // Reserve call frame if the size of the maximum call frame fits into 16-bit
  // immediate field and there are no variable sized objects on the stack.
  // Make sure the second regsiter scavenger spill slot can be accessed with
  // one instruction.
  return isInt<16>(MFI->getMaxCallFrameSize() + getStackAlignment()) &&
    !MFI->hasVarSizedObjects();
}

void CPU0SEFrameLowering::determineCalleeSaves(MachineFunction &MF,
                                               BitVector &SavedRegs,
                                               RegScavenger *RS) const {
  TargetFrameLowering::determineCalleeSaves(MF, SavedRegs, RS);
  CPU0FunctionInfo *CPU0FI = MF.getInfo<CPU0FunctionInfo>();
  MachineRegisterInfo &MRI = MF.getRegInfo();

  // // // if (MF.getFrameInfo()->hasCalls())
//    setAliasRegs(MF, SavedRegs, CPU0::LR);
}

const CPU0FrameLowering *
llvm::createCPU0SEFrameLowering(const CPU0Subtarget &ST) {
  return new CPU0SEFrameLowering(ST);
}
