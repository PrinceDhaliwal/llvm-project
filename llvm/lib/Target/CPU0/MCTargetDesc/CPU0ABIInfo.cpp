#include "CPU0ABIInfo.h"
#include "CPU0.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/ADT/StringSwitch.h"
#include "llvm/MC/MCTargetOptions.h"
#include "llvm/Support/CommandLine.h"

using namespace llvm;

static cl::opt<bool>
EnableCPU0S32Calls("cpu0-s32-calls", cl::Hidden,
                   cl::desc("CPU0 S32 call: use stack only to pass arguments."),
                   cl::init(false));


namespace {
  static const MCPhysReg O32IntRegs[4] = {CPU0::A0, CPU0::A1 };
  static const MCPhysReg S32IntRegs = {};
}


const ArrayRef<MCPhysReg> CPU0ABIInfo::GetByValArgRegs() const {
  if (IsO32())
    return makeArrayRef(O32IntRegs);
  if (IsS32())
    return makeArrayRef(S32IntRegs);
  llvm_unreachable("unhandled ABI");
}

const ArrayRef<MCPhysReg> CPU0ABIInfo::GetVarArgRegs() const {
  if (IsO32())
    return makeArrayRef(O32IntRegs);
  if (IsS32())
    return makeArrayRef(S32IntRegs);
  llvm_unreachable("unhandled ABI");
}

unsigned CPU0ABIInfo::GetCalleeAllocArgSizeInBytes(CallingConv::ID CC) const {
  if (IsO32())
    return CC != 0;
  if (IsS32())
    return 0;
  llvm_unreachable("unhandled ABI");
}

CPU0ABIInfo CPU0ABIInfo::computeTargetABI() {
  CPU0ABIInfo abi(ABI::Unknown);

  if (EnableCPU0S32Calls)
    abi = ABI::S32;
  else
    abi = ABI::O32;

  assert(abi.ThisABI != ABI::Unknown);
  return abi;
}

unsigned CPU0ABIInfo::GetStackPtr() const {
  return CPU0::SP;
}


unsigned CPU0ABIInfo::GetFramePtr() const {
  return CPU0::FP;
}

unsigned CPU0ABIInfo::GetNullPtr() const {
  return CPU0::ZERO;
}

unsigned CPU0ABIInfo::GetEhDataReg(unsigned I) const {
  static const unsigned EhDataReg[] = {
                                       CPU0::A0,
                                       CPU0::A1
  };

  return EhDataReg[I];
}

int CPU0ABIInfo::EhDataRegSize() const {
  return ThisABI == ABI::S32 ? 0 : 2;
}
