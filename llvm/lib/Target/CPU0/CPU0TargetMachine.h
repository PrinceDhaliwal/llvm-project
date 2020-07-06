#ifndef LLVM_LIB_TARGET_CPU0_CPU0TARGETMACHINE_H_
#define LLVM_LIB_TARGET_CPU0_CPU0TARGETMACHINE_H_
#include "CPU0Config.h"


#include "MCTargetDesc/CPU0ABIInfo.h"
#include "CPU0Subtarget.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/CodeGen/SelectionDAGISel.h"
#include "llvm/CodeGen/TargetFrameLowering.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {
  class formatted_raw_ostream;
  class CPU0RegisterInfo;

  class CPU0TargetMachine : public LLVMTargetMachine {
    bool isLittle;
    std::unique_ptr<TargetLoweringObjectFile> TLOF;

    CPU0ABIInfo ABI;
    CPU0Subtarget DefaultSubtarget;

    mutable StringMap<std::unique_ptr<CPU0Subtarget>> SubtargetMap;

  public:
    CPU0TargetMachine(const Target &T, const Triple &TT, StringRef CPU,
                      StringRef FS, const TargetOptions &Options,
                      Optional<Reloc::Model> RM, CodeModel::Model CM,
                      CodeGenOpt::Level OL, bool isLittle);

    ~CPU0TargetMachine() override;

    const CPU0Subtarget *getSubtargetImpl() const {
      return &DefaultSubtarget;
    }

    const TargetSubtargetInfo *getSubtargetImpl(const Function &F) const override;

    TargetPassConfig *createPassConfig(PassManagerBase &PM) override;

    TargetLoweringObjectFile *getObjFileLowering() const override {
      return TLOF.get();
    }

    bool isLittleEndian() const { return isLittle; }
    const CPU0ABIInfo &getABI() const { return ABI; }
  };

  /// CPU0EBTargetMachine - CPU032 big endian target machine
  class CPU0EBTargetMachine : public CPU0TargetMachine {
    virtual void anchor();
  public:
    CPU0EBTargetMachine(const Target &T, const Triple &TT, StringRef CPU,
                        StringRef FS, const TargetOptions &Options,
                        Optional<Reloc::Model> RM, Optional<CodeModel::Model> CM,
                        CodeGenOpt::Level OL, bool JIT);
  };

  /// CPU0ELTargetMachine - CPU032 little endian target machine.
  class CPU0ELTargetMachine : public CPU0TargetMachine {
    virtual void anchor();
  public:
    CPU0ELTargetMachine(const Target &T, const Triple &TT, StringRef CPU,
                        StringRef FS, const TargetOptions &Options,
                        Optional<Reloc::Model> RM, Optional<CodeModel::Model> CM,
                        CodeGenOpt::Level OL, bool JIT);
  };
} // end llvm namespace
#endif
