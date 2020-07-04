// This is top level entry for the CPU0 target.

// Target-independent interfaces
include "llvm/Target/Target.td"

// Target-dependent interfaces

include "CPU0RegisterInfo.td"
include "CPU0RegisterInfoGPROutForOther.td"
include "CPU0.td"
