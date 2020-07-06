// This describes calling conventions for CPU0 architecture.

// CCIfSubtarget - Match if the current subtarget has a feature F.
class CCIfSubtarget<string F, CCAction A>:
      CCIf<!strconcat("State.getTarget().getSubtarget<CPU0Subtarget>().", F), A>;

def CSR_O32 : CalleeSavedRegs<(add LR, FP, (sequence "S%u", 1, 0))>;
