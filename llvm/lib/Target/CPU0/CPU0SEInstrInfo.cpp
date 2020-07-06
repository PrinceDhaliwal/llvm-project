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

const CPU0InstrInfo *llvm::createCPU0SEInstrInfo(const CPU0Subtarget &STI) {
  return new CPU0SEInstrInfo(STI);
}

