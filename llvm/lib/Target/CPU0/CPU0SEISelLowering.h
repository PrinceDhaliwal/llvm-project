#ifndef LLVM_LIB_TARGET_CPU0_CPU0SEISELLOWERING_H
#define LLVM_LIB_TARGET_CPU0_CPU0SEISELLOWERING_H

#include "CPU0Config.h"
#include "CPU0ISelLowering.h"
#include "CPU0RegisterInfo.h"

namespace llvm {
  class CPU0SETargetLowering : public CPU0TargetLowering {
  public:
    explicit CPU0SETargetLowering(const CPU0TargetMachine &TM,
                                  const CPU0Subtarget &STI);

    SDValue LowerOperation(SDValue Op, SelectionDAG &DAG) const override;
  };
}

#endif
