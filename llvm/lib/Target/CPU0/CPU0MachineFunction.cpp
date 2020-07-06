#include "CPU0MachineFunction.h"
#include "CPU0InstrInfo.h"
#include "CPU0Subtarget.h"
#include "llvm/IR/Function.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"

using namespace llvm;

bool FixGlobalBaseReg;

CPU0FunctionInfo::~CPU0FunctionInfo() {}

void CPU0FunctionInfo::anchor() {}
