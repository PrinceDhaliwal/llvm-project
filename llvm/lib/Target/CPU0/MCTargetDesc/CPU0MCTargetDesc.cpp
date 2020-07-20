#include "CPU0MCTargetDesc.h"

#include "CPU0InstPrinter.h"
#include "CPU0MCAsmInfo.h"

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

/// Select CPU0 architecture Feature for the given triple and cpu name.
/// The function will be called at command 'llvm-objdump -d' for CPU0 elf input.
static std::string selectCPU0ArchFeature(const Triple &TT, StringRef CPU) {
  std::string CPU0ArchFeature;

  if (CPU.empty() || CPU == "generic") {
    if (TT.getArch() == Triple::cpu0 || TT.getArch() == Triple::cpu0el) {
      if (CPU.empty() || CPU == "cpu032II") {
        CPU0ArchFeature = "+cpu032II";
      }
      else {
        if (CPU == "cpu032I") {
          CPU0ArchFeature = "+cpu032I";
        }
      }
    }
  }
  return CPU0ArchFeature;
}

static MCInstrInfo *createCPU0MCInstrInfo() {
  MCInstrInfo *X = new MCInstrInfo();
  InitCPU0MCInstrInfo(X); // defined in CPU0GenInstrInfo.inc
  return X;
}

static MCRegisterInfo *createCPU0MCRegisterInfo(const Triple &TT) {
  MCRegisterInfo *X = new MCRegisterInfo();
  InitCPU0MCRegisterInfo(X, CPU0::SW); //defined in CPU0GenRegisterInfo.inc
  return X;
}

static MCSubtargetInfo *createCPU0MCSubtargetInfo(const Triple &TT,
                                                  StringRef CPU,
                                                  StringRef FS) {
  std::string ArchFS = selectCPU0ArchFeature(TT, CPU);
  if (!FS.empty()) {
    if (!ArchFS.empty()) {
      ArchFS += "," + FS.str();
    } else {
      ArchFS = FS.str();
    }
  }

  // defined in CPU0GenSubtargetInfo.inc
  return createCPU0MCSubtargetInfoImpl(TT, CPU, ArchFS);
}

static MCAsmInfo *createCPU0AsmInfo(const MCRegisterInfo &MRI,
                                    const Triple &TT,
                                    const MCTargetOptions &Opts) {
  MCAsmInfo *MAI = new CPU0MCAsmInfo(TT);

  unsigned SP = MRI.getDwarfRegNum(CPU0::SP, true);
  MCCFIInstruction Inst = MCCFIInstruction::cfiDefCfa(nullptr, SP, 0);
  MAI->addInitialFrameState(Inst);
  return MAI;
}

static MCInstPrinter *createCPU0MCInstPrinter(const Triple &T,
                                              unsigned SyntaxVariant,
                                              const MCAsmInfo &MAI,
                                              const MCInstrInfo &MII,
                                              const MCRegisterInfo &MRI) {
  return new CPU0InstPrinter(MAI, MII, MRI);
}

static MCStreamer *createMCStreamer(const Triple &TT, MCContext &Context,
                                    std::unique_ptr<MCAsmBackend> &&MAB,
                                    std::unique_ptr<MCObjectWriter> &&OW,
                                    std::unique_ptr<MCCodeEmitter> &&Emitter, bool RelaxAll) {
  return createELFStreamer(Context, std::move(MAB), std::move(OW), std::move(Emitter), RelaxAll);
}

static MCTargetStreamer *createCPU0AsmTargetStreamer(MCStreamer &S,
                                                     formatted_raw_ostream &OS,
                                                     MCInstPrinter *InstPrint,
                                                     bool isVerboseAsm) {
  return new CPU0TargetAsmStreamer(S, OS);
}

namespace {
  class CPU0MCInstrAnalysis : public MCInstrAnalysis {
  public:
    CPU0MCInstrAnalysis(const MCInstrInfo *Info) : MCInstrAnalysis(Info) {}
  };
}


static MCInstrAnalysis *createCPU0MCInstrAnalysis(const MCInstrInfo *Info) {
  return new CPU0MCInstrAnalysis(Info);
}

// @2 {
extern "C" void LLVMInitializeCPU0TargetMC() {
  for (Target *T : {&TheCPU0Target, &TheCPU0elTarget}) {
    RegisterMCAsmInfoFn X(*T, createCPU0AsmInfo);

    TargetRegistry::RegisterMCInstrInfo(*T, createCPU0MCInstrInfo);

    // register info
    TargetRegistry::RegisterMCRegInfo(*T, createCPU0MCRegisterInfo);

    // subtarget info
    TargetRegistry::RegisterMCSubtargetInfo(*T, createCPU0MCSubtargetInfo);

    // instruction analyzer
    TargetRegistry::RegisterMCInstrAnalysis(*T, createCPU0MCInstrAnalysis);

    //  MCInstPrinter
    TargetRegistry::RegisterMCInstPrinter(*T, createCPU0MCInstPrinter);

    // elf streamer
    TargetRegistry::RegisterELFStreamer(*T, createMCStreamer);

    // asm target streamer
    TargetRegistry::RegisterAsmTargetStreamer(*T, createCPU0AsmTargetStreamer);

    // MC Code Emitter
    TargetRegistry::RegisterMCCodeEmitter(TheCPU0Target,
                                          createCPU0MCCodeEmitterEB);
    TargetRegistry::RegisterMCCodeEmitter(TheCPU0elTarget,
                                          createCPU0MCCodeEmitterEL);

    // asm backend
    TargetRegistry::RegisterMCAsmBackend(TheCPU0Target,
                                         createCPU0AsmBackendEB32);
    TargetRegistry::RegisterMCAsmBackend(TheCPU0elTarget,
                                         createCPU0AsmBackendEL32);
  }
}

// @2 }
