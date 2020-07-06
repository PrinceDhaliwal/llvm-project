#ifndef LLVM_LIB_TARGET_CPU0_CPU0SUBTARGET_H_
#define LLVM_LIB_TARGET_CPU0_CPU0SUBTARGET_H_

#include "CPU0FrameLowering.h"
#include "CPU0ISelLowering.h"
#include "CPU0InstrInfo.h"
#include "llvm/CodeGen/SelectionDAGTargetInfo.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/MC/MCInstrItineraries.h"
#include "llvm/CodeGen/TargetSubtargetInfo.h"
#include "llvm/Support/Alignment.h"

#include <string>

#define GET_SUBTARGETINFO_HEADER
#include "CPU0GenSubtargetInfo.inc"

namespace llvm {
  class StringRef;
  class CPU0TargetMachine;

  class CPU0Subtarget : public CPU0GenSubtargetInfo {
    virtual void anchor();
  protected:
    enum CPU0ArchEnum {
                       CPU032I,
                       CPU032II
    };


    // CPU0 archtecture version
    CPU0ArchEnum CPU0ArchVersion;

    bool IsLittle;
    bool EnableOverflow;
    bool DisableOverflow;
    bool HasCmp;
    bool HasSlt;
    bool SupportFloat;

    InstrItineraryData InstrItins;
    const CPU0TargetMachine &TM;

    Triple TargetTriple;

    const SelectionDAGTargetInfo TSInfo;

    std::unique_ptr<const CPU0InstrInfo> InstrInfo;
    std::unique_ptr<const CPU0RegisterInfo> RegisterInfo;
    std::unique_ptr<const CPU0FrameLowering> FrameLowering;
    std::unique_ptr<const CPU0TargetLowering> TLInfo;


  public:
    bool isPositionIndependent() const;
    const CPU0ABIInfo &getABI() const;

    CPU0Subtarget(const Triple &TT, const StringRef &CPU, const StringRef &FS,
                  bool Little, const CPU0TargetMachine &TM);

    void ParseSubtargetFeatures(StringRef CPU, StringRef FS);

    bool isLittle() const { return IsLittle; }
    bool hasCPU032I() const { return CPU0ArchVersion >= CPU032I; }
    bool isCPU032I() const { return CPU0ArchVersion == CPU032I; }

    bool hasCPU032II() const { return CPU0ArchVersion >= CPU032II; }
    bool isCPU032II() const { return CPU0ArchVersion >= CPU032II; }

    bool enableOverflow() const { return EnableOverflow; }
    bool disableOverflow() const { return DisableOverflow; }

    bool hasCmp() const { return HasCmp; }
    bool hasSlt() const { return HasSlt; }
    bool supportFloat() const { return SupportFloat; }

    bool abiUsesSoftFloat() const;

    bool enableLongBranchPass() const {
      return hasCPU032II();
    }

    llvm::Align stackAlignment() const { return llvm::Align(8); }

    CPU0Subtarget &initializeSubtargetDependencies(StringRef CPU, StringRef FS,
                                                   const TargetMachine &TM);

    const SelectionDAGTargetInfo *getSelectionDAGInfo() const override {
      return &TSInfo;
    }

    const CPU0InstrInfo *getInstrInfo() const override { return InstrInfo.get(); }
    const TargetFrameLowering *getFrameLowering() const override {
      return FrameLowering.get();
    }

    const CPU0RegisterInfo *getRegisterInfo() const override {
      return &InstrInfo->getRegisterInfo();
    }

    const CPU0TargetLowering *getTargetLowering() const override {
      return TLInfo.get();
    }

    const InstrItineraryData *getInstrItineraryData() const override {
      return &InstrItins;
    }
  };
} // end llvm namespace


#endif
