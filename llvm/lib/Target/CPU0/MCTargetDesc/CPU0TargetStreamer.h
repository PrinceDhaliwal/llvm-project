#ifndef LLVM_LIB_TARGET_CPU0_CPU0TARGETSTREAMER_H_
#define LLVM_LIB_TARGET_CPU0_CPU0TARGETSTREAMER_H_

#include "llvm/MC/MCStreamer.h"

namespace llvm {

    class CPU0TargetStreamer : public MCTargetStreamer {
    public:
      CPU0TargetStreamer(MCStreamer &S)
        : MCTargetStreamer(S) {}
    };

    // This part is for ascii assembly output
    class CPU0TargetAsmStreamer : public CPU0TargetStreamer {
      formatted_raw_ostream &OS;

    public:
      CPU0TargetAsmStreamer(MCStreamer &S, formatted_raw_ostream &OS)
        : CPU0TargetStreamer(S), OS(OS) {}
    };

}

#endif
