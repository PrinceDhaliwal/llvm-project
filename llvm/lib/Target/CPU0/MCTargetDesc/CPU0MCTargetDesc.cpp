#include "CPU0MCTargetDesc.h"
#include "llvm/MC/MachineLocation.h"
#include "llvm/MC/MCELFStreamer.h"
#include "llvm/MC/MCInstrAnalysis.h"
#include "llvm/MC/MCInstPrinter.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/FormattedStream.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

#define GET_INSTRINFO_MC_DESC
#include "CPU0GenInstrInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "CPU0GenSubtargetInfo.inc"

#define GET_REGINFO_MC_DESC
#include "CPU0GenRegisterInfo.inc"


// @2 {
extern "C" void LLVMInitializeCPU0TargetMC() {
}

// @2 }
