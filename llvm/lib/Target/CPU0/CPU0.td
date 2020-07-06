
include "CPU0CallingConv.td"
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

def FeatureFloat : SubtargetFeature<"float", "SupportFloat", "true", "Enable floating point operations">;
def FeatureCmp   : SubtargetFeature<"cmp", "HasCmp", "true", "Enable 'cmp' instructions">;
def FeatureSlt   : SubtargetFeature<"slt", "HasSlt", "true", "Enable 'slt' instructions">;

def : Processor<"cpu032I", CPU0GenericItineraries, [ FeatureFloat, FeatureCmp ]>;
def : Processor<"cpu032II", CPU0GenericItineraries, [ FeatureFloat, FeatureSlt ]>;


