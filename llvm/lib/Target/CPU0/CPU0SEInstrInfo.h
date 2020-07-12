//===-- CPU0SEInstrInfo.h - CPU032/64 Instruction Information ---*- C++ -*-===//
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

#ifndef LLVM_LIB_TARGET_CPU0_CPU0SEINSTRINFO_H
#define LLVM_LIB_TARGET_CPU0_CPU0SEINSTRINFO_H

#include "CPU0Config.h"

#include "CPU0InstrInfo.h"
#include "CPU0SERegisterInfo.h"
#include "CPU0MachineFunction.h"

namespace llvm {

class CPU0SEInstrInfo : public CPU0InstrInfo {
  const CPU0SERegisterInfo RI;

public:
  explicit CPU0SEInstrInfo(const CPU0Subtarget &STI);

  const CPU0RegisterInfo &getRegisterInfo() const override;

  bool expandPostRAPseudo(MachineInstr &MI) const override;

  // Adjust SP by amount
  void adjustStackPtr(unsigned SP, int64_t Amount, MachineBasicBlock &MBB,
		      MachineBasicBlock::iterator I) const;

  // Emit a series of instructions to load an immediate. If NewImm is a
  // non-NULL parameter, the last instruction is not emitted, but instead its
  // immediate operand is returned in NewImm.
  unsigned loadImmediate(int64_t Imm, MachineBasicBlock &MBB,
                         MachineBasicBlock::iterator II, const DebugLoc &DL,
                         unsigned *NewImm) const;

  void storeRegToStack(MachineBasicBlock &MBB,
                       MachineBasicBlock::iterator MI,
                       Register SrcReg, bool IsKill, int FrameIndex,
                       const TargetRegisterClass *RC,
                       const TargetRegisterInfo *TRI,
                       int64_t Offset) const override;

  void loadRegFromStack(MachineBasicBlock &MBB,
                        MachineBasicBlock::iterator MI,
                        Register DestReg, int FrameIndex,
                        const TargetRegisterClass *RC,
                        const TargetRegisterInfo *TRI,
                        int64_t Offset) const override;
private:
  void expandRetLR(MachineBasicBlock &MBB, MachineBasicBlock::iterator I) const;
};

}

#endif

