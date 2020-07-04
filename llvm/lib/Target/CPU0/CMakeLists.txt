set(LLVM_TARGET_DEFINITIONS CPU0Other.td)

tablegen(LLVM CPU0GenRegisterInfo.inc -gen-register-info)
tablegen(LLVM CPU0GenInstrInfo.inc -gen-instr-info)
tablegen(LLVM CPU0GenSubtargetInfo.inc -gen-subtarget)
tablegen(LLVM CPU0GenMCPseudoLowering.inc -gen-pseudo-lowering)

add_public_tablegen_target(CPU0CommonTableGen)

add_llvm_target(CPU0CodeGen
  CPU0TargetMachine.cpp)

add_subdirectory(TargetInfo)
add_subdirectory(MCTargetDesc)
