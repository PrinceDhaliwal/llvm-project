#include "CPU0MachineFunction.h"
#include "CPU0SEISelLowering.h"
#include "CPU0RegisterInfo.h"
#include "CPU0TargetMachine.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/CodeGen/TargetInstrInfo.h"

using namespace llvm;

#define DEBUG_TYPE "cpu0-isel"

static cl::opt<bool>
EnableCPU0TailCalls("enable-cpu0-tail-calls", cl::Hidden,
                    cl::desc("CPU0: Enable tail calls."), cl::init(false));

CPU0SETargetLowering::CPU0SETargetLowering(const CPU0TargetMachine &TM,
                                           const CPU0Subtarget &STI)
  : CPU0TargetLowering(TM, STI) {
  // set up register classes
  addRegisterClass(MVT::i32, &CPU0::CPURegsRegClass);

  // must, computeRegisterProperties - Once all of the register classes are
  // added, this allows us to compute derived properties we expose.
  computeRegisterProperties(Subtarget.getRegisterInfo());
}

SDValue CPU0SETargetLowering::LowerOperation(SDValue Op,
                                             SelectionDAG &DAG) const {
  return CPU0TargetLowering::LowerOperation(Op, DAG);
}

const CPU0TargetLowering *
llvm::createCPU0SETargetLowering(const CPU0TargetMachine &TM,
                                 const CPU0Subtarget &STI) {
  return new CPU0SETargetLowering(TM, STI);
}
