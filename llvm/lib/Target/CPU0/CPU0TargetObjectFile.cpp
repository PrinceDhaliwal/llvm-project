#include "CPU0TargetObjectFile.h"

#include "CPU0Subtarget.h"
#include "CPU0TargetMachine.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCSectionELF.h"

#include "llvm/Support/CommandLine.h"
#include "llvm/Target/TargetMachine.h"

using namespace llvm;

static cl::opt<unsigned>
SSThreshold("cpu0-section-threshold", cl::Hidden,
            cl::desc("Small data and bss section threshold size (default=8)"),
            cl::init(8));

void CPU0TargetObjectFile::Initialize(MCContext &Ctx, const TargetMachine &TM) {
  TargetLoweringObjectFileELF::Initialize(Ctx, TM);
  InitializeELF(TM.Options.UseInitArray);

  SmallDataSection = getContext().getELFSection(".sdata",
                                                ELF::SHT_PROGBITS,
                                                ELF::SHF_WRITE | ELF::SHF_ALLOC);

  SmallBSSSection = getContext().getELFSection(".sbss", ELF::SHT_NOBITS,
                                               ELF::SHF_WRITE | ELF::SHF_ALLOC);

  this->TM = &static_cast<const CPU0TargetMachine &>(TM);
}
