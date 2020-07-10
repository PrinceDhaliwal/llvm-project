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
  switch (Opcode) {
  default: break;
  }

  return false;
}

FunctionPass *llvm::createCPU0SEISelDag(CPU0TargetMachine &TM,
                                        CodeGenOpt::Level OptLevel) {
  return new CPU0SEDAGToDAGISel(TM, OptLevel);
}
