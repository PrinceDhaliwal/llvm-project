#include "CPU0SEISelDAGToDAG.h"

#include "MCTargetDesc/CPU0BaseInfo.h"
#include "CPU0.h"
#include "CPU0MachineFunction.h"
#include "CPU0RegisterInfo.h"
#include "llvm/CodeGen/MachineConstantPool.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/SelectionDAGNodes.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/GlobalValue.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/IR/Type.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetMachine.h"

using namespace llvm;

#define DEBUG_TYPE "cpu0-isel"

bool CPU0SEDAGToDAGISel::runOnMachineFunction(MachineFunction &MF) {
  Subtarget = &static_cast<const CPU0Subtarget &>(MF.getSubtarget());
  return CPU0DAGToDAGISel::runOnMachineFunction(MF);
}

void CPU0SEDAGToDAGISel::processFunctionAfterISel(MachineFunction &MF) {}

bool CPU0SEDAGToDAGISel::trySelect(SDNode *Node) {
  unsigned Opcode = Node->getOpcode();
  SDLoc DL(Node);

  EVT NodeTy = Node->getValueType(0);
  unsigned MultOpc;

  // Instruction selection not handled by the auto-generated table
  // selection, should be handled here.
  switch (Opcode) {
  default: break;

  case ISD::MULHS:
  case ISD::MULHU: {
    MultOpc = (Opcode = ISD::MULHU ? CPU0::MULTu : CPU0::MULT);
    auto LoHi = selectMULT(Node, MultOpc, DL, NodeTy, false, true);
    ReplaceNode(Node, LoHi.second);
    return true;
  }
  case ISD::Constant: {
    const ConstantSDNode *CN = dyn_cast<ConstantSDNode>(Node);
    unsigned Size = CN->getValueSizeInBits(0);
    if (Size == 32)
      break;
    return true;
  }
    
  }

  return false;
}

std::pair<SDNode *, SDNode *>
CPU0SEDAGToDAGISel::selectMULT(SDNode *N, unsigned Opc, const SDLoc &DL,
                               EVT Ty, bool HasLo, bool HasHi) {
  SDNode *Lo = 0, *Hi = 0;
  SDNode *Mul = CurDAG->getMachineNode(Opc, DL, MVT::Glue, N->getOperand(0),
                                       N->getOperand(1));

  SDValue InFlag = SDValue(Mul, 0);

  if (HasLo) {
    Lo = CurDAG->getMachineNode(CPU0::MFLO, DL,
                                Ty, MVT::Glue, InFlag);
    InFlag = SDValue(Lo, 1);
  }

  if (HasHi) {
    Hi = CurDAG->getMachineNode(CPU0::MFHI, DL,
                                Ty, InFlag);
  }

  return std::make_pair(Lo, Hi);
}

FunctionPass *llvm::createCPU0SEISelDag(CPU0TargetMachine &TM,
                                        CodeGenOpt::Level OptLevel) {
  return new CPU0SEDAGToDAGISel(TM, OptLevel);
}
