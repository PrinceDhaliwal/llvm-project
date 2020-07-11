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

  // CCValAssign - represent the assignment of
  // the return value to a location
  SmallVector<CCValAssign, 16> RVLocs;
  MachineFunction &MF = DAG.getMachineFunction();

  // CCState - Info about the registers and stack slot.
  CCState CCInfo(CallConv, IsVarArg, MF, RVLocs, *DAG.getContext());

  CPU0CC CPU0CCInfo(CallConv, ABI.IsO32(), CCInfo);

  // Analyze return values
  CPU0CCInfo.analyzeReturn(Outs, Subtarget.abiUsesSoftFloat(),
			   MF.getFunction().getReturnType());

  SDValue Flag;
  SmallVector<SDValue, 4> RetOps(1, Chain);

  // Copy the result values into the output registers.
  for (unsigned i = 0; i != RVLocs.size(); ++i) {
    SDValue Val = OutVals[i];
    CCValAssign &VA = RVLocs[i];
    assert(VA.isRegLoc() && "Can only return in registers!");

    if (RVLocs[i].getValVT() != RVLocs[i].getLocVT())
      Val = DAG.getNode(ISD::BITCAST, DL, RVLocs[i].getLocVT(), Val);

    Chain = DAG.getCopyToReg(Chain, DL, VA.getLocReg(), Val, Flag);

    // Guarantee that all emitted copies are stuck together with flags.
    Flag = Chain.getValue(1);
    RetOps.push_back(DAG.getRegister(VA.getLocReg(), VA.getLocVT()));
  }
  
  // Ordinary struct type: 2
  // The CPU0 ABIs for returning structs by value requires that we copy
  // the sret argument into $v0 for the return. We saved the argument into
  // a virtual register in the entry block, so now we copy the value out
  // and into $v0
  if (MF.getFunction().hasStructRetAttr()) {
    CPU0FunctionInfo *CPU0FI = MF.getInfo<CPU0FunctionInfo>();
    unsigned Reg = CPU0FI->getSRetReturnReg();

    if (!Reg)
      llvm_unreachable("sret virtual register not created in the entry block");
    SDValue Val =
      DAG.getCopyFromReg(Chain, DL, Reg, getPointerTy(DAG.getDataLayout()));
    unsigned V0 = CPU0::V0;
    Chain = DAG.getCopyToReg(Chain, DL, V0, Val, Flag);
    Flag = Chain.getValue(1);
    RetOps.push_back(DAG.getRegister(V0, getPointerTy(DAG.getDataLayout())));
  }

  RetOps[0] = Chain;
  // Add the flag if we have it.
  if (Flag.getNode())
    RetOps.push_back(Flag);

  // Return on CPU0 is always a "ret $lr"
  return DAG.getNode(CPU0ISD::Ret, DL, MVT::Other, RetOps);
}

void CPU0TargetLowering::CPU0CC::initialize() {
  CCInfo.AllocateStack(reservedArgArea(), 1);
}

template <typename Ty>
void CPU0TargetLowering::CPU0CC::
analyzeReturn(const SmallVectorImpl<Ty> &RetVals, bool IsSoftFloat,
              const SDNode *CallNode, const Type *RetTy) const {
  CCAssignFn *Fn;
  Fn = RetCC_CPU0;

  for (unsigned I = 0, E = RetVals.size(); I < E; ++I) {
    MVT VT = RetVals[I].VT;
    ISD::ArgFlagsTy Flags = RetVals[I].Flags;
    MVT RegVT = this->getRegVT(VT, RetTy, CallNode, IsSoftFloat);

    if (Fn(I, VT, RegVT, CCValAssign::Full, Flags, this->CCInfo)) {
#ifndef NDEBUG
      dbgs() << "Call result #" << I << " has unhandled type "
             << EVT(VT).getEVTString() << "\n";
#endif
      llvm_unreachable(nullptr);
    }
  }
}

void CPU0TargetLowering::CPU0CC::
analyzeCallResult(const SmallVectorImpl<ISD::InputArg> &Ins, bool IsSoftFloat,
		  const SDNode *CallNode, const Type *RetTy) const {
  analyzeReturn(Ins, IsSoftFloat, CallNode, RetTy);
}

void CPU0TargetLowering::CPU0CC::
analyzeReturn(const SmallVectorImpl<ISD::OutputArg> &Outs, bool IsSoftFloat,
	      const Type *RetTy) const {
  analyzeReturn(Outs, IsSoftFloat, nullptr, RetTy);
}

unsigned CPU0TargetLowering::CPU0CC::reservedArgArea() const {
  return (IsO32 && (CallConv != CallingConv::Fast)) ? 8 : 0;
}

MVT CPU0TargetLowering::CPU0CC::getRegVT(MVT VT, const Type *OrigTy,
					 const SDNode *CallNode,
					 bool IsSoftFloat) const {
  if (IsSoftFloat || IsO32)
    return VT;

  return VT;
}
