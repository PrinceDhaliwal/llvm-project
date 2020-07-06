#ifndef LLVM_LIB_TARGET_CPU0_CPU0TARGETOBJECTFILE_H 
#define LLVM_LIB_TARGET_CPU0_CPU0TARGETOBJECTFILE_H

#include "CPU0Config.h"
#include "CPU0TargetMachine.h"

#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"

namespace llvm {
    class CPU0TargetMachine;

    class CPU0TargetObjectFile : public TargetLoweringObjectFileELF {
        MCSection *SmallDataSection;
        MCSection *SmallBSSSection;

        const CPU0TargetMachine *TM;

    public:
        void Initialize(MCContext &Ctx, const TargetMachine &TM) override;
    };
}

#endif