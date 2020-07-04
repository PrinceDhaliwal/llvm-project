#include "CPU0.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

Target llvm::TheCPU0Target, llvm::TheCPU0elTarget;
extern "C" void LLVMInitializeCPU0TargetInfo() {
  RegisterTarget<Triple::cpu0, true> X(TheCPU0Target, "cpu0", "CPU0",  "CPU0");
  RegisterTarget<Triple::cpu0el, true> Y(TheCPU0elTarget, "cpu0el", "CPU0 Co-processor", "CPU0el");
}
