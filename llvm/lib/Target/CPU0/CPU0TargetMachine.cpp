#include "CPU0TargetMachine.h"
#include "CPU0TargetObjectFile.h"
#include "CPU0.h"
#include "CPU0SEISelDAGToDAG.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/Support/CodeGen.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Target/TargetLoweringObjectFile.h"

using namespace llvm;

#define DEBUG_TYPE "cpu0"

extern "C" void LLVMInitializeCPU0Target() {
  // Register the target
  //- Big endian Target Machine
  RegisterTargetMachine<CPU0EBTargetMachine> X(TheCPU0Target);
  RegisterTargetMachine<CPU0ELTargetMachine> Y(TheCPU0elTarget);
}

static std::string computeDataLayout(const Triple &TT, StringRef CPU,
                                     const TargetOptions &Options,
                                     bool isLittle) {
  std::string Ret = "";

  if (isLittle)
    Ret += "e";
  else
    Ret += "E";

  Ret += "-m:m";

  // Pointers are 32-bit on some ABIs.
  Ret += "-p:32:32";

  // 8 and 16 bit integers only need to have natual alignment, but try to
  // align them to 32-bits. 64 bit integers have natural alignment
  Ret += "-i8:8:32-i16:16:32-i64:64";

  // 32-bit register are always available and the stack is at least 64-bit
  // aligned
  Ret += "-n32-s64";

  return Ret;
}


static Reloc::Model getEffectiveRelocModel(Optional<CodeModel::Model> CM,
                                           Optional<Reloc::Model> RM) {
  if (!RM.hasValue() || (CM.hasValue() && CM == CodeModel::Small))
    return Reloc::Static;
  return *RM;
}

static CodeModel::Model getCodeModel1(Optional<CodeModel::Model> CM) {
  if (!CM.hasValue()) {
    return CodeModel::Small;
  }
  return *CM;
}

CPU0TargetMachine::CPU0TargetMachine(const Target &T, const Triple &TT,
                                     StringRef CPU, StringRef FS,
                                     const TargetOptions &Options,
                                     Optional<Reloc::Model> RM,
                                     CodeModel::Model CM, CodeGenOpt::Level OL,
                                     bool isLittle)
  : LLVMTargetMachine(T, computeDataLayout(TT, CPU, Options, isLittle), TT,
                      CPU, FS, Options, getEffectiveRelocModel(CM, RM), CM,
                      OL),
    isLittle(isLittle), TLOF(std::make_unique<CPU0TargetObjectFile>()),
    ABI(CPU0ABIInfo::computeTargetABI()),
    DefaultSubtarget(TT, std::string(CPU), FS, isLittle, *this) {
  initAsmInfo();
}

CPU0TargetMachine::~CPU0TargetMachine() {}

void CPU0EBTargetMachine::anchor() {}

CPU0EBTargetMachine::CPU0EBTargetMachine(const Target &T, const Triple &TT,
                                         StringRef CPU, StringRef FS,
                                         const TargetOptions &Options,
                                         Optional<Reloc::Model> RM,
                                         Optional<CodeModel::Model> CM, CodeGenOpt::Level OL, bool JIT)
  : CPU0TargetMachine(T, TT, CPU, FS, Options, RM, getCodeModel1(CM), OL, true) {}

void CPU0ELTargetMachine::anchor() {}

CPU0ELTargetMachine::CPU0ELTargetMachine(const Target &T, const Triple &TT,
                                         StringRef CPU, StringRef FS,
                                         const TargetOptions &Options,
                                         Optional<Reloc::Model> RM,
                                         Optional<CodeModel::Model> CM, CodeGenOpt::Level OL, bool JIT)
  : CPU0TargetMachine(T, TT, CPU, FS, Options, RM, getCodeModel1(CM), OL, true) {}

const TargetSubtargetInfo *
CPU0TargetMachine::getSubtargetImpl(const Function &F) const {
  Attribute CPUAttr = F.getFnAttribute("target-cpu");
  Attribute FSAttr = F.getFnAttribute("target-features");
  std::string CPU = !CPUAttr.hasAttribute(Attribute::None) ?
    CPUAttr.getValueAsString().str()
    : TargetCPU;

  std::string FS = !FSAttr.hasAttribute(Attribute::None) ?
    FSAttr.getValueAsString().str()
    : TargetFS;

  auto &I = SubtargetMap[CPU + FS];
  if (!I) {
    resetTargetOptions(F);
    I = std::make_unique<CPU0Subtarget>(TargetTriple, CPU, FS, isLittle, *this);
  }
  return I.get();
}


namespace {
  class CPU0PassConfig : public TargetPassConfig {
  public:
    CPU0PassConfig(CPU0TargetMachine *TM, PassManagerBase &PM)
      : TargetPassConfig(*TM, PM) {}

    bool addInstSelector() override;

    CPU0TargetMachine &getCPU0TargetMachine() const {
      return getTM<CPU0TargetMachine>();
    }

    const CPU0Subtarget &getCPU0Subtarget() const {
      return *getCPU0TargetMachine().getSubtargetImpl();
    }
  };
} // namespace

bool CPU0PassConfig::addInstSelector() {
  addPass(createCPU0SEISelDag(getCPU0TargetMachine(), getOptLevel()));
  return false;
}

TargetPassConfig *CPU0TargetMachine::createPassConfig(PassManagerBase &PM) {
  return new CPU0PassConfig(this, PM);
}
