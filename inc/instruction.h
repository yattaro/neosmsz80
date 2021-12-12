#include "z80.h"
#ifndef Z80_INSTR
#define Z80_INSTR

enum pri_instructions {
    nop,    // Implemented
    ldbcXX,
    ldbcMEMa,
    incbc,
    incb,
    decb,
    ldbX,
    rlca,
    exafafSHADOW,
    addhlbc,
    ldabcMEM,
    decbc,
    incc,
    decc,
    ldcX,
    rrca,
    djnzX,
    lddeXX,
    lddeMEMa,
    incde,
    incd,
    decd,
    lddX,
    rla,
    jrX,
    addhlde,
    ldadeMEM,
    decde,
    ince,
    dece,
    ldeX,
    rra,
    jrnzX,
    ldhlXX,
    ldXXMEMhl,
    inchl,
    inch,
    dech,
    ldhX,
    daa,
    jrzX,
    addhlhl,
    ldhlXXMEM,
    dechl,
    incl,
    decl,
    ldlX,
    cpl,
    jrncX,
    ldspXX,
    ldXXMEMa,
    incsp,
    inchlMEM,
    dechlMEM,
    ldhlMEMX,
    scf,
    jrcX,
    addhlsp,
    ldaXXMEM,
    decsp,
    inca,
    deca,
    ldaX,
    ccf,
    ldbb,
    ldbc,
    ldbd,
    ldbe,
    ldbh,
    ldbl,
    ldbhlMEM,
    ldba,
    ldcb,
    ldcc,
    ldcd,
    ldce,
    ldch,
    ldcl,
    ldchlMEM,
    ldca,
    lddb,
    lddc,
    lddd,
    ldde,
    lddh,
    lddl,
    lddhlMEM,
    ldda,
    ldeb,
    ldec,
    lded,
    ldee,
    ldeh,
    ldel,
    ldehlMEM,
    ldea,
    ldhb,
    ldhc,
    ldhd,
    ldhe,
    ldhh,
    ldhl,
    ldhhlMEM,
    ldha,
    ldlb,
    ldlc,
    ldld,
    ldle,
    ldlh,
    ldll,
    ldlhlMEM,
    ldla,
    ldhlMEMb,
    ldhlMEMc,
    ldhlMEMd,
    ldhlMEMe,
    ldhlMEMh,
    ldhlMEMl,
    halt,
    ldhlMEMa,
    ldab,
    ldac,
    ldad,
    ldae,
    ldah,
    ldal,
    ldahlMEM,
    ldaa,
    addab,
    addac,
    addad,
    addae,
    addah,
    addal,
    addahlMEM,
    addaa,
    adcab,
    adcac,
    adcad,
    adcae,
    adcah,
    adcal,
    adcahlMEM,
    adcaa,
    subb,
    subc,
    subd,
    sube,
    subh,
    subl,
    subhlMEM,
    suba,
    sbcab,
    sbcac,
    sbcad,
    sbcae,
    sbcah,
    sbcal,
    sbcahlMEM,
    sbcaa,
    andb,
    andc,
    andd,
    ande,
    andh,
    andl,
    andhlMEM,
    anda,
    xorb,
    xorc,
    xord,
    xore,
    xorh,
    xorl,
    xorhlMEM,
    xora,
    orb,
    orc,
    ord,
    ore,
    orh,
    orl,
    orhlMEM,
    ora,
    cp_b,
    cp_c,
    cp_d,
    cp_e,
    cp_h,
    cp_l,
    cp_hlMEM,
    cp_a,
    retnz,
    popbc,
    jpnzXX,
    jpXX,
    callnzXX,
    pushbc,
    addaX,
    rst00h,
    retz,
    ret,
    jpzXX,
    BITS,
    callzXX,
    callXX,
    adcaX,
    rst08h,
    retnc,
    popde,
    jpncXX,
    outXMEMa,
    callncXX,
    pushde,
    subX,
    rst10h,
    retc,
    exx,
    jpcXX,
    inaXMEM,
    callcXX,
    IX,
    sbcaX,
    rst18h,
    retpo,
    pophl,
    jppoXX,
    exspMEMhl,
    callpoXX,
    pushhl,
    andX,
    rst20h,
    retpe,
    jphlMEM,
    jppeXX,
    exdehl,
    callpeXX,
    EXTD,
    xorX,
    rst28h,
    retp,
    popaf,
    jppXX,
    di,
    callpXX,
    pushaf,
    orX,
    rst30h,
    retm,
    ldsphl,
    jpmXX,
    ei,
    callmXX,
    IY,
    cp_X,
    rst38h
};

enum ed_instructions;
enum cb_instructions;

#endif