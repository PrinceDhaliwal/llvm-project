
include "CPU0Schedule.td"
include "CPU0InstrInfo.td"

def CPU0InstrInfo : InstrInfo;

// Will generate CPU0GenAsmWrite.inc included by CPU0InstPrinter.cpp, contents 
//  as follows,
// void CPU0InstPrinter::printInstruction(const MCInst *MI, raw_ostream &O) {...}
// const char *CPU0InstPrinter::getRegisterName(unsigned RegNo) {...}
def CPU0 : Target {
// def CPU0InstrInfo : InstrInfo as before.
  let InstructionSet = CPU0InstrInfo;
}

include "Target.td"
