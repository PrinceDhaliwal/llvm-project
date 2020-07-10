set(LLVM_TARGET_DEFINITIONS CPU0Other.td)

tablegen(LLVM CPU0GenRegisterInfo.inc -gen-register-info)
tablegen(LLVM CPU0GenInstrInfo.inc -gen-instr-info)
tablegen(LLVM CPU0GenSubtargetInfo.inc -gen-subtarget)
tablegen(LLVM CPU0GenMCPseudoLowering.inc -gen-pseudo-lowering)
tablegen(LLVM CPU0GenDAGISel.inc -gen-dag-isel)
tablegen(LLVM CPU0GenCallingConv.inc -gen-callingconv)
tablegen(LLVM CPU0GenCodeEmitter.inc -gen-emitter)
tablegen(LLVM CPU0GenMCCodeEmitter.inc -gen-emitter)
tablegen(LLVM CPU0GenAsmWriter.inc -gen-asm-writer)

add_public_tablegen_target(CPU0CommonTableGen)

add_llvm_target(CPU0CodeGen
  CPU0FrameLowering.cpp
  CPU0InstrInfo.cpp
  CPU0ISelLowering.cpp
  CPU0MachineFunction.cpp
  CPU0RegisterInfo.cpp
  CPU0SEFrameLowering.cpp
  CPU0SEInstrInfo.cpp
  CPU0SEISelLowering.cpp
  CPU0SERegisterInfo.cpp
  CPU0Subtarget.cpp
  CPU0TargetObjectFile.cpp
  CPU0MCInstLower.cpp
  CPU0AsmPrinter.cpp
  CPU0SEISelDAGToDAG.cpp
  CPU0ISelDAGToDAG.cpp
  CPU0MCInstLower.cpp
  CPU0TargetMachine.cpp)

add_subdirectory(TargetInfo)
add_subdirectory(MCTargetDesc)
