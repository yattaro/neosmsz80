#include "z80.h"
#include "mem.h"
#include "instruction.h"
#include <errno.h>
#include <string.h>

struct z80_regs registers = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

/*
 * Reads the next instruction to be executed.
 */
int next_instruction()
{
    int opcode_cycles = 0;
    BYTE instr = read_mem(registers.PC);
    registers.PC++;
    opcode_cycles = exec(instr);
    return opcode_cycles;
}

/*
 * Increments the Z80's refresh register
 */
void inc_rr()
{
    if(registers.RR == 127) registers.RR = 0;
    else registers.RR++;
}


/*
 * Decodes and executes the given opcode and returns the number of cycles
 * requred.
 */
int exec(BYTE opcode)
{
    inc_rr();
    switch (opcode)
    {
    case nop: 
        return 4;
    case ldbcXX:
    case ldbcMEMa:
    case incbc:
    case incb:
    case decb:
    case ldbX:
    case rlca:
    case exafafSHADOW:
    case addhlbc:
    case ldabcMEM:
    case decbc:
    case incc:
    case decc:
    case ldcX:
    case rrca:
    case djnzX:
    case lddeXX:
    case lddeMEMa:
    case incde:
    case incd:
    case decd:
    case lddX:
    case rla:
    case jrX:
    case addhlde:
    case ldadeMEM:
    case decde:
    case ince:
    case dece:
    case ldeX:
    case rra:
    case jrnzX:
    case ldhlXX:
    case ldXXMEMhl:
    case inchl:
    case inch:
    case dech:
    case ldhX:
    case daa:
    case jrzX:
    case addhlhl:
    case ldhlXXMEM:
    case dechl:
    case incl:
    case decl:
    case ldlX:
    case cpl:
    case jrncX:
    case ldspXX:
    case ldXXMEMa:
    case incsp:
    case inchlMEM:
    case dechlMEM:
    case ldhlMEMX:
    case scf:
    case jrcX:
    case addhlsp:
    case ldaXXMEM:
    case decsp:
    case inca:
    case deca:
    case ldaX:
    case ccf:
    case ldbb:
    case ldbc:
    case ldbd:
    case ldbe:
    case ldbh:
    case ldbl:
    case ldbhlMEM:
    case ldba:
    case ldcb:
    case ldcc:
    case ldcd:
    case ldce:
    case ldch:
    case ldcl:
    case ldchlMEM:
    case ldca:
    case lddb:
    case lddc:
    case lddd:
    case ldde:
    case lddh:
    case lddl:
    case lddhlMEM:
    case ldda:
    case ldeb:
    case ldec:
    case lded:
    case ldee:
    case ldeh:
    case ldel:
    case ldehlMEM:
    case ldea:
    case ldhb:
    case ldhc:
    case ldhd:
    case ldhe:
    case ldhh:
    case ldhl:
    case ldhhlMEM:
    case ldha:
    case ldlb:
    case ldlc:
    case ldld:
    case ldle:
    case ldlh:
    case ldll:
    case ldlhlMEM:
    case ldla:
    case ldhlMEMb:
    case ldhlMEMc:
    case ldhlMEMd:
    case ldhlMEMe:
    case ldhlMEMh:
    case ldhlMEMl:
    case halt:
    case ldhlMEMa:
    case ldab:
    case ldac:
    case ldad:
    case ldae:
    case ldah:
    case ldal:
    case ldahlMEM:
    case ldaa:
    case addab:
    case addac:
    case addad:
    case addae:
    case addah:
    case addal:
    case addahlMEM:
    case addaa:
    case adcab:
    case adcac:
    case adcad:
    case adcae:
    case adcah:
    case adcal:
    case adcahlMEM:
    case adcaa:
    case subb:
    case subc:
    case subd:
    case sube:
    case subh:
    case subl:
    case subhlMEM:
    case suba:
    case sbcab:
    case sbcac:
    case sbcad:
    case sbcae:
    case sbcah:
    case sbcal:
    case sbcahlMEM:
    case sbcaa:
    case andb:
    case andc:
    case andd:
    case ande:
    case andh:
    case andl:
    case andhlMEM:
    case anda:
    case xorb:
    case xorc:
    case xord:
    case xore:
    case xorh:
    case xorl:
    case xorhlMEM:
    case xora:
    case orb:
    case orc:
    case ord:
    case ore:
    case orh:
    case orl:
    case orhlMEM:
    case ora:
    case cp_b:
    case cp_c:
    case cp_d:
    case cp_e:
    case cp_h:
    case cp_l:
    case cp_hlMEM:
    case cp_a:
    case retnz:
    case popbc:
    case jpnzXX:
    case jpXX:
    case callnzXX:
    case pushbc:
    case addaX:
    case rst00h:
    case retz:
    case ret:
    case jpzXX:
    case BITS:
    case callzXX:
    case callXX:
    case adcaX:
    case rst08h:
    case retnc:
    case popde:
    case jpncXX:
    case outXMEMa:
    case callncXX:
    case pushde:
    case subX:
    case rst10h:
    case retc:
    case exx:
    case jpcXX:
    case inaXMEM:
    case callcXX:
    case IX:
    case sbcaX:
    case rst18h:
    case retpo:
    case pophl:
    case jppoXX:
    case exspMEMhl:
    case callpoXX:
    case pushhl:
    case andX:
    case rst20h:
    case retpe:
    case jphlMEM:
    case jppeXX:
    case exdehl:
    case callpeXX:
    case EXTD:
    case xorX:
    case rst28h:
    case retp:
    case popaf:
    case jppXX:
    case di:
    case callpXX:
    case pushaf:
    case orX:
    case rst30h:
    case retm:
    case ldsphl:
    case jpmXX:
    case ei:
    case callmXX:
    case IY:
    case cp_X:
    case rst38h:
    
    default:
        errno = ENOSYS;
        fprintf(stderr, "%s: unimplemented instruction 0x%02x\n",
            strerror(errno), opcode);
        break;
    }
}