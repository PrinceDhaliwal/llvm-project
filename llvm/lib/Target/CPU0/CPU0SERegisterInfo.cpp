#include "CPU0SERegisterInfo.h"

using namespace llvm;

#define DEBUG_TYPE "cpu0-reg-info"


CPU0SERegisterInfo::CPU0SERegisterInfo(const CPU0Subtarget &ST)
  : CPU0RegisterInfo(ST) {}

const TargetRegisterClass *
CPU0SERegisterInfo::intRegClass(unsigned Size) const {
  return &CPU0::CPURegsRegClass;
}
