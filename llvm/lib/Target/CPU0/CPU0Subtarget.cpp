#include "CPU0Subtarget.h"

#include "CPU0MachineFunction.h"
#include "CPU0.h"
#include "CPU0RegisterInfo.h"

#include "CPU0TargetMachine.h"

#include "llvm/IR/Attributes.h"
#include "llvm/IR/Function.h"

#include "llvm/Support/CommandLine.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

#define DEBUG_TYPE "cpu0-subtarget"
#define GET_SUBTARGETINFO_TARGET_DESC
#define GET_SUBTARGETINFO_CTOR
#include "CPU0GenSubtargetInfo.inc"

extern bool FixGlobalBaseReg;

void CPU0Subtarget::anchor() {}

CPU0Subtarget::CPU0Subtarget(const Triple &TT, const StringRef &CPU,
                            const StringRef &FS, bool little,
                            const CPU0TargetMachine &TM_)
  : CPU0GenSubtargetInfo(TT, CPU, FS),
    IsLittle(little), TM(TM_), TargetTriple(TT), TSInfo(),
    InstrInfo(CPU0InstrInfo::create(initializeSubtargetDependencies(CPU, FS, TM))),
    FrameLowering(CPU0FrameLowering::create(*this)),
    TLInfo(CPU0TargetLowering::create(TM, *this)) { }

bool CPU0Subtarget::isPositionIndependent() const {
  return TM.isPositionIndependent();
}

CPU0Subtarget &
CPU0Subtarget::initializeSubtargetDependencies(StringRef CPU, StringRef FS,
                                               const TargetMachine &TM) {
  if (TargetTriple.getArch() == Triple::cpu0 || TargetTriple.getArch() == Triple::cpu0el) {
    if (CPU.empty() || CPU == "generic") {
      CPU = "cpu032II";
    }
    else if (CPU == "help") {
      CPU = "";
      return *this;
    }
    else if (CPU != "cpu032I" && CPU != "cpu032II") {
      CPU = "cpu032II";
    }
  }
  else {
    errs() << "Error, TargetTriple.getArch() = " << TargetTriple.getArch()
           << "CPU = " << CPU << "\n";
    exit(0);
  }

  if (CPU == "cpu032I")
    CPU0ArchVersion = CPU032I;
  else if (CPU == "cpu032II")
    CPU0ArchVersion = CPU032II;

  if (isCPU032I()) {
    HasCmp = true;
    HasSlt = false;
  }
  else if (isCPU032II()) {
    HasCmp = true;
    HasSlt = true;
  }
  else {
    errs() << "-mcpu must be empty(default:cpu032II), cpu032I or cpu032II" << "\n";
  }

  ParseSubtargetFeatures(CPU, FS);

  InstrItins = getInstrItineraryForCPU(CPU);
  return *this;
}

bool CPU0Subtarget::abiUsesSoftFloat() const {
  return true;
}

const CPU0ABIInfo &CPU0Subtarget::getABI() const { return TM.getABI(); }
      
