//===-- CPU0SEInstrInfo.cpp - CPU032/64 Instruction Information -----------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the CPU032/64 implementation of the TargetInstrInfo class.
//
//===----------------------------------------------------------------------===//

#include "CPU0SEInstrInfo.h"

#include "CPU0MachineFunction.h"
#include "CPU0TargetMachine.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

CPU0SEInstrInfo::CPU0SEInstrInfo(const CPU0Subtarget &STI)
    : CPU0InstrInfo(STI),
      RI(STI) {}

const CPU0RegisterInfo &CPU0SEInstrInfo::getRegisterInfo() const {
  return RI;
}

// Expand Pseuo instructions into real backend instructions
bool CPU0SEInstrInfo::expandPostRAPseudo(MachineInstr &MI) const {
  MachineBasicBlock &MBB = *MI.getParent();

  switch (MI.getDesc().getOpcode()) {
  default:
    return false;

  case CPU0::RetLR:
    expandRetLR(MBB, MI);
    break;
  }

  MBB.erase(MI);
  return true;
}

void CPU0SEInstrInfo::expandRetLR(MachineBasicBlock &MBB,
				  MachineBasicBlock::iterator I) const {
  BuildMI(MBB, I, I->getDebugLoc(), get(CPU0::RET)).addReg(CPU0::LR);
}

void CPU0SEInstrInfo::adjustStackPtr(unsigned SP, int64_t Amount,
				     MachineBasicBlock &MBB,
				     MachineBasicBlock::iterator I) const {
  DebugLoc DL = I != MBB.end() ? I->getDebugLoc() : DebugLoc();
  unsigned Addu = CPU0::ADDu;
  unsigned ADDiu = CPU0::ADDiu;

  if (isInt<16>(Amount)) {
    // addiu sp, sp, amount
    BuildMI(MBB, I, DL, get(ADDiu), SP).addReg(SP).addImm(Amount);
  } else { // Expand immediate that doesn't fit in 16-bit
    unsigned Reg = loadImmediate(Amount, MBB, I, DL, nullptr);
    BuildMI(MBB, I, DL, get(Addu), SP).addReg(SP).addReg(Reg, RegState::Kill);
  }
}

unsigned
CPU0SEInstrInfo::loadImmediate(int64_t Imm, MachineBasicBlock &MBB,
			       MachineBasicBlock::iterator II,
			       const DebugLoc &DL,
			       unsigned *NewImm) const {
  CPU0AnalyzeImmediate AnalyzeImm;
  unsigned Size = 32;
  unsigned LUi = CPU0::LUi;
  unsigned ZEROReg = CPU0::ZERO;
  unsigned ATReg = CPU0::AT;
  bool LastInstrIsADDiu = NewImm;

  const CPU0AnalyzeImmediate::InstSeq &Seq =
    AnalyzeImm.Analyze(Imm, Size, LastInstrIsADDiu);

  CPU0AnalyzeImmediate::InstSeq::const_iterator Int = Seq.begin();

  assert(Seq.size() && (!LastInstrIsADDiu || (Seq.size() > 1)));

  // the first instruction can be a LUi, which is different from other
  // instructions (ADDiu, ORI and SLL) in that it does not have a register
  // operand.
  if (Inst->Opc == LUi)
    BuildMI(MBB, II, DL, get(LUi), ATReg).addImm(SignExtend64<16>(Inst->ImmOpnd));
  else
    BuildMI(MBB, II, DL, get(Inst->Opc), ATReg).addReg(ZEROReg)
      .addImm(SignExtend64<16>(Inst->ImmOpnd));

  // Build the remaining instructions in Seq.
  for (++Inst; Inst != Seq.end() - LastInstrIsADDiu; ++Inst)
    BuildMI(MBB, II, DL, get(Inst->Opc), ATReg).addReg(ATReg)
      .addImm(SignExtend64<16>(Inst->ImmOpnd));

  if (LastInstrIsADDiu)
    *NewImm = Inst->ImmOpnd;

  return ATReg;
}

const CPU0InstrInfo *llvm::createCPU0SEInstrInfo(const CPU0Subtarget &STI) {
  return new CPU0SEInstrInfo(STI);
}


