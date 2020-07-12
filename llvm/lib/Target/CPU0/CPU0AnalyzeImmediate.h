#ifndef CPU0_ANALYZE_IMMEDIATE_H_
#define CPU0_ANALYZE_IMMEDIATE_H_

#include "llvm/ADT/SmallVector.h"
#include "llvm/Support/DataTypes.h"

namespace llvm {
  class CPU0AnalyzeImmediate {
  public:
    struct Inst {
      unsigned Opc, ImmOpnd;
      Inst(unsigned Opc, unsigned ImmOpnd)
        : Opc(Opc), ImmOpnd(ImmOpnd) {}
    };

    typedef SmallVector<Inst, 7> InstSeq;

    /// Analyze - Get an instruction sequence to load immediate Imm. The last
    // instruction in the sequence must be an ADDiu if LastInstrIsAddiu is true.
    const InstSeq &Analyze(uint64_t Imm, unsigned Size, bool LastInstrIsAddiu);
  private:
    typedef SmallVector<InstSeq, 5> InstSeqLs;

    /// AddInstr - Add I to all instruction sequences in SeqLs.
    void AddInstr(InstSeqLs &SeqLs, const Inst &I);

    /// GetInstSeqLsADDiu - Get instruction sequences which end with an ADDiu to
    /// load immediate Imm
    void GetInstSeqLsADDiu(uint64_t Imm, unsigned RemSize, InstSeqLs &SeqLs);

    /// Get instruction sequcnes which end with an Ori to load immediate Imm
    void GetInstSeqLsORi(uint64_t Imm, unsigned RemSize, InstSeqLs &SeqLs);

    /// get instruction sequences which end with a SHL to load immediate Imm
    void GetInstSeqLsSHL(uint64_t Imm, unsigned RemSize, InstSeqLs &SeqLs);

    /// get instruction sequences to load Imm
    void GetInstSeqLs(uint64_t Imm, unsigned RemSize, InstSeqLs &SeqLs);

    /// replace an ADDiu & SHL pair with a LUi.
    void ReplaceADDiuSHLWithLUi(InstSeq &Seq);

    /// get shortest seq - find the shortest instruction sequence in SeqLs and
    /// return it in Insts
    void GetShortestSeq(InstSeqLs &SeqLs, InstSeq &Insts);

    unsigned Size;
    unsigned ADDiu, ORi, SHL, LUi;
    InstSeq Insts;
  };
} // namespace llvm
#endif
