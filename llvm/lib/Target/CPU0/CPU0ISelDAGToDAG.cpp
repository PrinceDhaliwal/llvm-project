#include "CPU0ISelDAGToDAG.h"

#include "CPU0MachineFunction.h"
#include "CPU0RegisterInfo.h"
#include "CPU0SEISelDAGToDAG.h"
#include "CPU0TargetMachine.h"

#include "llvm/CodeGen/MachineConstantPool.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/SelectionDAGISel.h"
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

bool CPU0DAGToDAGISel::runOnMachineFunction(MachineFunction &MF) {
  bool Ret = SelectionDAGISel::runOnMachineFunction(MF);
  return Ret;
}

/// ComplexPattern used on CPU0InstrInfo
/// Used on CPU0 Load/Store instructions
bool CPU0DAGToDAGISel::SelectAddr(SDNode *Parent, SDValue Addr,
                                  SDValue &Base, SDValue &Offset) {
  EVT ValTy = Addr.getValueType();
  SDLoc DL(Addr);

  // if Parent is an unaligned f32 load or store, select a (base + index)
  // floating point load/store instruction (luxc1 or suxc1).
  const LSBaseSDNode *LS = 0;

  if (Parent && (LS = dyn_cast<LSBaseSDNode>(Parent))) {
    EVT VT = LS->getMemoryVT();

    if (VT.getSizeInBits() / 8 > LS->getAlignment()) {
      assert("Unalisnged loads/stores not supported for this type.");
      if (VT == MVT::f32)
        return false;
    }
  }

  // if Address is FI, get the TargetFrameIndex.
  if (FrameIndexSDNode *FIN = dyn_cast<FrameIndexSDNode>(Addr)) {
    Base = CurDAG->getTargetFrameIndex(FIN->getIndex(), ValTy);
    Offset = CurDAG->getTargetConstant(0, DL, ValTy);
    return true;
  }

  Base  = Addr;
  Offset = CurDAG->getTargetConstant(0, DL, ValTy);
  return true;
}

/// Select instructions not customized! Used for expanded,
/// promoted and normal instructions
void CPU0DAGToDAGISel::Select(SDNode *Node) {
  unsigned Opcode = Node->getOpcode();

//  DEBUG(errs() << "Selecting: "; Node->dump(CurDAG); errs() << "\n");

  // if we have a custom node, we already have selected
  if (Node->isMachineOpcode()) {
//    DEBUG(errs() << "== "; Node->dump(CurDAG); errs() << "\n");
    Node->setNodeId(-1);
    return;
  }

  // See if subclasses can handle this node.
  if (trySelect(Node))
    return;

  switch (Opcode) {
  default: break;
  }

  SelectCode(Node);
}
                                  
