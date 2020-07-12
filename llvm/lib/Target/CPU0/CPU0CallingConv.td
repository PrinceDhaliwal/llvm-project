// This describes calling conventions for CPU0 architecture.

// CCIfSubtarget - Match if the current subtarget has a feature F.
class CCIfSubtarget<string F, CCAction A>:
      CCIf<!strconcat("State.getTarget().getSubtarget<CPU0Subtarget>().", F), A>;

def CSR_O32 : CalleeSavedRegs<(add LR, FP, V0, (sequence "S%u", 1, 0))>;


def RetCC_CPU0EABI : CallingConv<[
      // i32 are returned in registers V0, V1, A0, A1
      CCIfType<[i32], CCAssignToReg<[V0, V1, A0, A1]>>
]>;

def RetCC_CPU0 : CallingConv<[
      CCDelegateTo<RetCC_CPU0EABI>
]>;
