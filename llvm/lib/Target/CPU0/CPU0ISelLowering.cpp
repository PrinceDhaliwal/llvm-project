#include "CPU0ISelLowering.h"

#include "CPU0MachineFunction.h"
#include "CPU0TargetMachine.h"
#include "CPU0TargetObjectFile.h"
#include "CPU0Subtarget.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/CodeGen/CallingConvLower.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/SelectionDAG.h"
#include "llvm/CodeGen/ValueTypes.h"
#include "llvm/IR/CallingConv.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

#define DEBUG_TYPE "cpu0-lower"

const char *CPU0TargetLowering::getTargetNodeName(unsigned Opcode) const {
  switch (Opcode) {
  case CPU0ISD::JmpLink: return "CPU0ISD::JmpLink";
  case CPU0ISD::TailCall: return "CPU0ISD::TailCall";
  case CPU0ISD::Hi: return "CPU0ISD::Hi";
  case CPU0ISD::Lo: return "CPU0ISD::Lo";
  case CPU0ISD::GPRel: return "CPU0ISD::GPRel";
  case CPU0ISD::Ret: return "CPU0ISD::Ret";
  case CPU0ISD::EH_RETURN: return "CPU0ISD::EH_RETURN";
  case CPU0ISD::DivRem: return "CPU0ISD::DivRem";
  case CPU0ISD::DivRemU: return "CPU0ISD::DivRemU";
  case CPU0ISD::Wrapper: return "CPU0ISD::Wrapper";
  default: return NULL;
  }
}

CPU0TargetLowering::CPU0TargetLowering(const CPU0TargetMachine &TM,
                                       const CPU0Subtarget &STI)
  : TargetLowering(TM), Subtarget(STI), ABI(TM.getABI()) {
  // set .align 2
  setMinFunctionAlignment(Align(2));
}

const CPU0TargetLowering *CPU0TargetLowering::create(const CPU0TargetMachine &TM,
                                                     const CPU0Subtarget &STI) {
  return llvm::createCPU0SETargetLowering(TM, STI);
}

/// Lower helper functions
#include "CPU0GenCallingConv.inc"

// Formal Argumnets Calling convention implementation
SDValue
CPU0TargetLowering::LowerFormalArguments(SDValue Chain,
                                         CallingConv::ID CallConv,
                                         bool IsVarArg,
                                         const SmallVectorImpl<ISD::InputArg> &Ins,
                                         const SDLoc &DL, SelectionDAG &DAG,
                                         SmallVectorImpl<SDValue> &InVals) const {
  return Chain;
}

/// Return Value Calling Convention Implementation
SDValue
CPU0TargetLowering::LowerReturn(SDValue Chain,
                                CallingConv::ID CallConv, bool IsVarArg,
                                const SmallVectorImpl<ISD::OutputArg> &Outs,
                                const SmallVectorImpl<SDValue> &OutVals,
                                const SDLoc &DL, SelectionDAG &DAG) const {
  return DAG.getNode(CPU0ISD::Ret, DL, MVT::Other,
                     Chain, DAG.getRegister(CPU0::LR, MVT::i32));
}

