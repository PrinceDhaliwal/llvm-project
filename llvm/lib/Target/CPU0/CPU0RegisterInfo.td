// Declarations that describe the CPU0 register file
class CPU0Reg<bits<16> Enc, string n> : Register<n> {
  let HWEncoding = Enc;
  let Namespace = "CPU0";
}

// CPU0 CPU Registers
class CPU0GPRReg<bits<16> Enc, string n> : CPU0Reg<Enc, n>;

// Co-processor 0 Registers
class CPU0C0Reg<bits<16> Enc, string n> : CPU0Reg<Enc, n>;

// @Registers
let Namespace = "CPU0" in {
  // GPR
  def ZERO  : CPU0GPRReg<0, "zero">, DwarfRegNum<[0]>;
  def AT    : CPU0GPRReg<1, "1">,    DwarfRegNum<[1]>;
  def V0    : CPU0GPRReg<2, "2">,    DwarfRegNum<[2]>;
  def V1    : CPU0GPRReg<3, "3">,    DwarfRegNum<[3]>;
  def A0    : CPU0GPRReg<4, "4">,    DwarfRegNum<[4]>;
  def A1    : CPU0GPRReg<5, "5">,    DwarfRegNum<[5]>;
  def T9    : CPU0GPRReg<6, "t9">,    DwarfRegNum<[6]>;
  def T0    : CPU0GPRReg<7, "7">,    DwarfRegNum<[7]>;
  def T1    : CPU0GPRReg<8, "8">,    DwarfRegNum<[8]>;
  def S0    : CPU0GPRReg<9, "9">,    DwarfRegNum<[0]>;
  def S1    : CPU0GPRReg<10, "10">,  DwarfRegNum<[10]>;
  def GP    : CPU0GPRReg<11, "gp">,  DwarfRegNum<[11]>;
  def FP    : CPU0GPRReg<12, "fp">,  DwarfRegNum<[12]>;
  def SP    : CPU0GPRReg<13, "sp">,  DwarfRegNum<[13]>;
  def LR    : CPU0GPRReg<14, "lr">,  DwarfRegNum<[14]>;
  def SW    : CPU0GPRReg<15, "sw">,  DwarfRegNum<[15]>;

  def PC    : CPU0C0Reg<0, "pc">,    DwarfRegNum<[20]>;
  def EPC   : CPU0C0Reg<1, "epc">,   DwarfRegNum<[21]>;
}


// @Register classes
def CPURegs : RegisterClass<"CPU0", [i32], 32, (add
  ZERO, AT,
  // Return Values and Arguments
  V0, V1, A0, A1,
  // Not preserved across procedure calls
  T9, T0, T1,
  // Callee save
  S0, S1,
  // Reserved
  GP, FP,
  SP, LR, SW)>;

// Status Registers class
def SR    : RegisterClass<"CPU0", [i32], 32, (add SW)>;

// Co-processor 0 registers class
def C0Regs : RegisterClass<"CPU0", [i32],32, (add PC, EPC)>;
