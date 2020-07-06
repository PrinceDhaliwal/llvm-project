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

  
};

}

#endif

