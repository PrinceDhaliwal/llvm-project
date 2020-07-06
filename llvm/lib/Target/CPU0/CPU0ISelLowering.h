#ifndef LLVM_LIB_TARGET_CPU0_CPU0ISELLOWERING_H
#define LLVM_LIB_TARGET_CPU0_CPU0ISELLOWERING_H

#include "CPU0Config.h"

#include "MCTargetDesc/CPU0ABIInfo.h"
#include "CPU0.h"
#include "llvm/CodeGen/CallingConvLower.h"
#include "llvm/CodeGen/SelectionDAG.h"
#include "llvm/IR/Function.h"
#include "llvm/CodeGen/TargetLowering.h"
#include <deque>

namespace llvm {
  namespace CPU0ISD {
    enum NodeType {
      FIRST_NUMBER = ISD::BUILTIN_OP_END,
      JmpLink,

      // Tail call,
      TailCall,

      // get the higher 16 bits from a 32-bit immediate
      Hi,

      // get the lower 16 bits from a 32-bit immediate
      Lo,

      // Handle gp_rel (small data/bss sections) relocation
      GPRel,

      // Thread pointer
      ThreadPointer,

      // Return
      Ret,

      EH_RETURN,

      // DivRem(u)
      DivRem,
      DivRemU,

      Wrapper,
      DynAlloc,

      Sync
    };
  }

  // TargetLowering Implemnetation
  class CPU0FunctionInfo;
  class CPU0Subtarget;

  class CPU0TargetLowering : public TargetLowering {
  public:
    explicit CPU0TargetLowering(const CPU0TargetMachine &TM,
                                const CPU0Subtarget &STI);

    static const CPU0TargetLowering *create(const CPU0TargetMachine &TM,
                                            const CPU0Subtarget &STI);

    const char *getTargetNodeName(unsigned Opcode) const override;

  protected:
    struct ByValArgInfo {
      unsigned FirstIdx;// index of the first register used
      unsigned NumRegs; // number of registers used for this argument
      unsigned Address; // Offset of the stack area used to pass this argument

      ByValArgInfo()
        : FirstIdx(0), NumRegs(0), Address(0) {}
    };

    const CPU0Subtarget &Subtarget;

    const CPU0ABIInfo &ABI;

  private:
    // Lower Operand specifics
    SDValue lowerGlobalAddress(SDValue Op, SelectionDAG &DAG) const;

    SDValue LowerFormalArguments(SDValue Chain,
                                 CallingConv::ID CallConv, bool IsVarArg,
                                 const SmallVectorImpl<ISD::InputArg> &Ins,
                                 const SDLoc &dl, SelectionDAG &DAG,
                                 SmallVectorImpl<SDValue> &InVals) const override;

    SDValue LowerReturn(SDValue Chain,
                        CallingConv::ID CallCov, bool IsVarArg,
                        const SmallVectorImpl<ISD::OutputArg> &Outs,
                        const SmallVectorImpl<SDValue> &OutVals,
                        const SDLoc &dl, SelectionDAG &DAG) const override;

  };

  const CPU0TargetLowering *
    createCPU0SETargetLowering(const CPU0TargetMachine &TM, const CPU0Subtarget &STI);
}

#endif
