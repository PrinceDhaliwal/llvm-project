#ifndef LLVM_LIB_TARGET_CPU0_CPU0SEISELDAGTODAG_H_
#define LLVM_LIB_TARGET_CPU0_CPU0SEISELDAGTODAG_H_

#include "CPU0ISelDAGToDAG.h"

namespace llvm {
  class CPU0SEDAGToDAGISel : public CPU0DAGToDAGISel {
  public:
    explicit CPU0SEDAGToDAGISel(CPU0TargetMachine &TM, CodeGenOpt::Level OL)
      : CPU0DAGToDAGISel(TM, OL) {}

  private:
    bool runOnMachineFunction(MachineFunction &MF) override;

    bool trySelect(SDNode *Node) override;

    std::pair<SDNode *, SDNode *> selectMULT(SDNode *N, unsigned Opc,
                                             const SDLoc &DL, EVT Ty,
                                             bool HasLo, bool HasHi);

    void processFunctionAfterISel(MachineFunction &MF);
  };

  FunctionPass *createCPU0SEISelDag(CPU0TargetMachine &TM,
                                    CodeGenOpt::Level OptLevel);
}
#endif
