/* codegen.c - machine code generation */

#include "armasm.h"
#include "elf/elf.h"

struct codegen_map_st codegen_opcode_map[] = CODEGEN_OPCODE_MAP;
struct codegen_map_st codegen_bcc_map[] = CODEGEN_BCC_MAP;

#define COND_BIT 28

void codegen_error(char *err) {
    printf("codegen_error: %s\n", err);
    exit(-1);
}

void codegen_table_init(struct codegen_table_st *ct, struct parse_node_st *tree) {
    ct->len = 0;
    ct->next = 0;
    ct->tree = tree;
}

void codegen_add_inst(struct codegen_table_st *ct, uint32_t inst) {
    ct->table[ct->len] = inst;
    ct->len += 1;
}

uint32_t codegen_lookup(char *name, struct codegen_map_st *map, int map_len) {
    int i;
    for (i = 0; i < map_len; i++) {
        if (strncmp(name, map[i].name, SCAN_TOKEN_LEN) == 0) {
            return map[i].bits;
        }
    }

    codegen_error(name);
    return (uint32_t) -1;
}

uint32_t codegen_lookup_opcode(char *name) {
    int len = sizeof(codegen_opcode_map) / sizeof(codegen_opcode_map[0]);
    return codegen_lookup(name, codegen_opcode_map, len);
}

uint32_t codegen_lookup_bcc(char *name) {
    int len = sizeof(codegen_bcc_map) / sizeof(codegen_bcc_map[0]);
    return codegen_lookup(name, codegen_bcc_map, len);
}

int codegen_get_index(struct parse_node_st *np, char *label, int level) {
    int l1 = -1;
    int l2 = -1;

    if (np->type == INST) {
        if (strncmp(label, np->stmt.inst.label, SCAN_TOKEN_LEN) == 0) {
            l1 = level;
        }
    } else if (np->type == SEQ) { 
        /* check the left node to see if it's a dir, if so skip it by going on to the next node and don't update the level */
        if (np->stmt.seq.left->type == DIR) {   
            l2 = codegen_get_index(np->stmt.seq.right, label, level + 0);
        }   else {
            l1 = codegen_get_index(np->stmt.seq.left, label, level);
            l2 = codegen_get_index(np->stmt.seq.right, label, level + 1);
        } 
    } if (l1 == -1) {
        l1 = l2;
    }

    return l1;
}

void codegen_dp_and_cmp_common(struct codegen_table_st *ct, uint32_t imm, uint32_t op,
        uint32_t cond, uint32_t rn, uint32_t rd, uint32_t op2) {

    const uint32_t DP_IMM_BIT = 25;
    const uint32_t DP_OP_BIT  = 21;
    const uint32_t DP_COND_BIT  = 20;
    const uint32_t DP_RN_BIT  = 16;
    const uint32_t DP_RD_BIT  = 12;
    uint32_t inst = 0;

    inst = (COND_AL << COND_BIT)
        | (imm << DP_IMM_BIT)
        | (op  << DP_OP_BIT)
        | (cond << DP_COND_BIT)
        | (rn  << DP_RN_BIT)
        | (rd  << DP_RD_BIT)
        | op2;
    codegen_add_inst(ct, inst);
}

void codegen_dp3(struct codegen_table_st *ct, struct parse_node_st *np) {
    codegen_dp_and_cmp_common(
            ct,
            0,      /*imm*/
            codegen_lookup_opcode(np->stmt.inst.name),
            0,
            np->stmt.inst.dp3.rn,
            np->stmt.inst.dp3.rd,
            np->stmt.inst.dp3.rm);
}

void codegen_dpimm(struct codegen_table_st *ct, struct parse_node_st *np) {
    if ((np->stmt.inst.dpimm.imm) < 0) { 
        codegen_dp_and_cmp_common(
                ct,
                1,  /*imm*/
                codegen_lookup_opcode("mvn"),
                0,
                np->stmt.inst.dpimm.rn,     
                np->stmt.inst.dpimm.rd,
               ((np->stmt.inst.dpimm.imm + 1) * -1));
    }
    else {
        codegen_dp_and_cmp_common(
                ct,
                1,  /*imm*/
                codegen_lookup_opcode(np->stmt.inst.name),
                0,
                np->stmt.inst.dpimm.rn, 
                np->stmt.inst.dpimm.rd,
                np->stmt.inst.dpimm.imm);
    }
}

void codegen_cmp(struct codegen_table_st *ct, struct parse_node_st *np) { 
    codegen_dp_and_cmp_common(
            ct,
            0,      /*imm*/
            codegen_lookup_opcode(np->stmt.inst.name),
            1,
            np->stmt.inst.cmp.rd,
            np->stmt.inst.cmp.rn,
            np->stmt.inst.cmp.rm);
}

void codegen_cmpimm(struct codegen_table_st *ct, struct parse_node_st *np) { 
    codegen_dp_and_cmp_common(
            ct,
            1,      /*imm*/
            codegen_lookup_opcode(np->stmt.inst.name),
            1,
            np->stmt.inst.cmpimm.rd,
            np->stmt.inst.cmpimm.rn,
            np->stmt.inst.cmpimm.imm);
}

void codegen_mul_common(struct codegen_table_st *ct, uint32_t acc, uint32_t cond,
        uint32_t rd, uint32_t rn, uint32_t rs, uint32_t rm) {

    const uint32_t MUL_ACC_BIT = 21;
    const uint32_t MUL_COND_BIT = 20;
    const uint32_t MUL_RD_BIT = 16;
    const uint32_t MUL_RN_BIT = 12;
    const uint32_t MUL_RS_BIT = 8;
    const uint32_t MUL_SDT_BIT = 4;
    uint32_t inst = 0;

    inst = (COND_AL << COND_BIT)
        | (acc << MUL_ACC_BIT)
        | (cond << MUL_COND_BIT)
        | (rd << MUL_RD_BIT)
        | (rn << MUL_RN_BIT)
        | (rs << MUL_RS_BIT)
        | (0b1001 << MUL_SDT_BIT)
        | rm;
    codegen_add_inst(ct, inst);
}

void codegen_mul3(struct codegen_table_st *ct, struct parse_node_st *np) {
    codegen_mul_common(
            ct,
            0, /* acc */
            0, /* cond */
            np->stmt.inst.mul3.rd,
            np->stmt.inst.mul3.rs,
            np->stmt.inst.mul3.rm,
            np->stmt.inst.mul3.rn);
}

void codegen_mem_common(struct codegen_table_st *ct, uint32_t imm, uint32_t prepost,
        uint32_t updown, uint32_t byteword, uint32_t writeback, uint32_t loadstore,
        uint32_t rn, uint32_t rd, uint32_t offset) {

    const uint32_t MEM_SDT_BIT = 26;
    const uint32_t MEM_IMM_BIT = 25;
    const uint32_t MEM_PREPOST_BIT = 24;
    const uint32_t MEM_UPDOWN_BIT = 23;
    const uint32_t MEM_BYTEWORD_BIT = 22;
    const uint32_t MEM_WRITEBACK_BIT = 21;
    const uint32_t MEM_LOADSTORE_BIT = 20;    
    const uint32_t MEM_RN_BIT  = 16;
    const uint32_t MEM_RD_BIT  = 12;
    uint32_t inst = 0;

    inst = (COND_AL << COND_BIT)
        | (0b01 << MEM_SDT_BIT)
        | (imm << MEM_IMM_BIT)
        | (prepost << MEM_PREPOST_BIT)
        | (updown << MEM_UPDOWN_BIT)
        | (byteword << MEM_BYTEWORD_BIT)
        | (writeback << MEM_WRITEBACK_BIT)
        | (loadstore << MEM_LOADSTORE_BIT)
        | (rn  << MEM_RN_BIT)
        | (rd  << MEM_RD_BIT)
        | offset;
    codegen_add_inst(ct, inst);
}

void codegen_ls_common(struct codegen_table_st *ct, uint32_t imm, uint32_t op, uint32_t cond,
        uint32_t rn, uint32_t rd, uint32_t shift, uint32_t rm, int ls_flag) {
    const uint32_t LS_IMM_BIT = 25;
    const uint32_t LS_OP_BIT = 21;
    const uint32_t LS_COND_BIT = 20;
    const uint32_t LS_RN_BIT = 16;
    const uint32_t LS_RD_BIT = 12;
    const uint32_t SHIFT_AMM_BIT = 7;
    const uint32_t LS_SHIFT_BIT = 4;
    uint32_t inst = 0;
    uint32_t ls_bit = 0;

    if (ls_flag == 0) {
        ls_bit = 0b00;
    }
    else if (ls_flag == 1) {
        ls_bit = 0b10;
    }

    inst = (COND_AL << COND_BIT)
        | (imm << LS_IMM_BIT)
        | (op << LS_OP_BIT)
        | (rn << LS_RN_BIT)
        | (rd << LS_RD_BIT)
        | (shift << SHIFT_AMM_BIT)
        | (ls_bit << LS_SHIFT_BIT)
        | rm;

    codegen_add_inst(ct, inst);
}

void codegen_ls(struct codegen_table_st *ct, struct parse_node_st *np) {
    int ls_flag = 0;

    if (strncmp(np->stmt.inst.name, "lsl", SCAN_TOKEN_LEN) == 0) {
        ls_flag = 0;
    }
    else if (strncmp(np->stmt.inst.name, "lsr", SCAN_TOKEN_LEN) == 0) {
        ls_flag = 1;
    }

    codegen_ls_common(
            ct,
            0, /*imm*/
            codegen_lookup_opcode("mov"),
            0,
            0,                          /* rn = 0 */
            np->stmt.inst.ls.rd,        /* rm = second register */
            np->stmt.inst.ls.shift,     /* rd = first register */
            np->stmt.inst.ls.rm,        /* shift amount */
            ls_flag);                   /* last is 0b00 if lsl, 0b10 if lsr */
}

void codegen_memimm(struct codegen_table_st *ct, struct parse_node_st *np) {
    if (strncmp(np->stmt.inst.name, "str", SCAN_TOKEN_LEN) == 0) {
        codegen_mem_common(
                ct,
                0,                      /* imm */
                1,                      /* prepost */
                1,                      /* updown */
                0,                      /* byteword */
                0,                      /* writeback */
                0,                      /* loadstore - need to change when adding str */
                np->stmt.inst.memimm.rn,
                np->stmt.inst.memimm.rd,
                np->stmt.inst.memimm.imm);
    }
    else if (strncmp(np->stmt.inst.name, "ldr", SCAN_TOKEN_LEN) == 0) {
        codegen_mem_common(
                ct,
                0,                      /* imm */
                1,                      /* prepost */
                1,                      /* updown */
                0,                      /* byteword */
                0,                      /* writeback */
                1,                      /* loadstore - need to change when adding str */
                np->stmt.inst.memimm.rn,
                np->stmt.inst.memimm.rd,
                np->stmt.inst.memimm.imm);
    }
    else if (strncmp(np->stmt.inst.name, "strb", SCAN_TOKEN_LEN) == 0) {
        codegen_mem_common(
                ct,
                0,                      /* imm */
                1,                      /* prepost */
                1,                      /* updown */
                0,                      /* byteword */
                2,                      /* writeback */
                0,                      /* loadstore - need to change when adding str */
                np->stmt.inst.memimm.rn,
                np->stmt.inst.memimm.rd,
                np->stmt.inst.memimm.imm);
    }
    else if (strncmp(np->stmt.inst.name, "ldrb", SCAN_TOKEN_LEN) == 0) {
        codegen_mem_common(
                ct,
                0,                      /* imm */
                1,                      /* prepost */
                1,                      /* updown */
                0,                      /* byteword */
                2,                      /* writeback */
                1,                      /* loadstore - need to change when adding str */
                np->stmt.inst.memimm.rn,
                np->stmt.inst.memimm.rd,
                np->stmt.inst.memimm.imm);
    }
}

void codegen_mem3(struct codegen_table_st *ct, struct parse_node_st *np) {
    if (strncmp(np->stmt.inst.name, "str", SCAN_TOKEN_LEN) == 0) {
        codegen_mem_common(
                ct,
                1,                      /* imm */
                1,                      /* prepost */
                1,                      /* updown */
                0,                      /* byteword */
                0,                      /* writeback */
                0,                      /* loadstore - need to change when adding str */
                np->stmt.inst.mem3.rn,
                np->stmt.inst.mem3.rd,
                np->stmt.inst.mem3.rm);
    }
    else if (strncmp(np->stmt.inst.name, "ldr", SCAN_TOKEN_LEN) == 0) {
        codegen_mem_common(
                ct,
                1,                      /* imm */
                1,                      /* prepost */
                1,                      /* updown */
                0,                      /* byteword */
                0,                      /* writeback */
                1,                      /* loadstore - need to change when adding str */
                np->stmt.inst.mem3.rn,
                np->stmt.inst.mem3.rd,
                np->stmt.inst.mem3.rm);
    }
    else if (strncmp(np->stmt.inst.name, "strb", SCAN_TOKEN_LEN) == 0) {
        codegen_mem_common(
                ct,
                1,                      /* imm */
                1,                      /* prepost */
                1,                      /* updown */
                0,                      /* byteword */
                2,                      /* writeback */
                0,                      /* loadstore - need to change when adding str */
                np->stmt.inst.mem3.rn,
                np->stmt.inst.mem3.rd,
                np->stmt.inst.mem3.rm);
    }
    else if (strncmp(np->stmt.inst.name, "ldrb", SCAN_TOKEN_LEN) == 0) {
        codegen_mem_common(
                ct,
                1,                      /* imm */
                1,                      /* prepost */
                1,                      /* updown */
                0,                      /* byteword */
                2,                      /* writeback */
                1,                      /* loadstore - need to change when adding str */
                np->stmt.inst.mem3.rn,
                np->stmt.inst.mem3.rd,
                np->stmt.inst.mem3.rm);
    }
}

void codegen_mem2(struct codegen_table_st *ct, struct parse_node_st *np) {
    if (strncmp(np->stmt.inst.name, "str", SCAN_TOKEN_LEN) == 0) {
        codegen_mem_common(
                ct,
                0,                      /* imm */
                1,                      /* prepost */
                1,                      /* updown */
                0,                      /* byteword */
                0,                      /* writeback */
                0,                      /* loadstore - need to change when adding str */
                np->stmt.inst.mem2.rn,
                np->stmt.inst.mem2.rd,
                np->stmt.inst.mem2.rm);
    }
    else if (strncmp(np->stmt.inst.name, "ldr", SCAN_TOKEN_LEN) == 0) {
        codegen_mem_common(
                ct,
                0,                      /* imm */
                1,                      /* prepost */
                1,                      /* updown */
                0,                      /* byteword */
                0,                      /* writeback */
                1,                      /* loadstore - need to change when adding str */
                np->stmt.inst.mem2.rn,
                np->stmt.inst.mem2.rd,
                np->stmt.inst.mem2.rm);
    }
    else if (strncmp(np->stmt.inst.name, "strb", SCAN_TOKEN_LEN) == 0) {
        codegen_mem_common(
                ct,
                0,                      /* imm */
                1,                      /* prepost */
                1,                      /* updown */
                1,                      /* byteword */
                0,                      /* writeback */
                0,                      /* loadstore - need to change when adding str */
                np->stmt.inst.mem2.rn,
                np->stmt.inst.mem2.rd,
                np->stmt.inst.mem2.rm);
    }
    else if (strncmp(np->stmt.inst.name, "ldrb", SCAN_TOKEN_LEN) == 0) {
        codegen_mem_common(
                ct,
                0,                      /* imm */
                1,                      /* prepost */
                1,                      /* updown */
                1,                      /* byteword */
                0,                      /* writeback */
                1,                      /* loadstore - need to change when adding str */
                np->stmt.inst.mem2.rn,
                np->stmt.inst.mem2.rd,
                np->stmt.inst.mem2.rm);
    }
}

void codegen_bx(struct codegen_table_st *ct, struct parse_node_st *np) {
    const uint32_t BX_CODE_BIT = 4;
    const uint32_t bx_code = 0b000100101111111111110001;

    uint32_t inst = (COND_AL << COND_BIT)
        | (bx_code << BX_CODE_BIT)
        | np->stmt.inst.bx.rn;
    codegen_add_inst(ct, inst);
}

void codegen_b(struct codegen_table_st *ct, struct parse_node_st *np) {
    uint32_t lookup = codegen_lookup_bcc(np->stmt.inst.name);

    const uint32_t B_CODE_BIT = 25;
    const uint32_t L_CODE_BIT = 24;

    /* the current line that we're on */
    uint32_t curr_index = ct->len;              

    /* start from level 0 so we can see instructions ahead */
    uint32_t target_index = codegen_get_index(ct->tree, np->stmt.inst.b.label, 0); 
    uint32_t offset = 0;
    offset = target_index - (curr_index + 2);
    offset = offset & 0x00FFFFFF;

    uint32_t link_bit = 0;

    if (strncmp(np->stmt.inst.name, "bl", SCAN_TOKEN_LEN) == 0) {
        link_bit = 1;
    }

    uint32_t inst = (lookup << COND_BIT)
        | (0b101 << B_CODE_BIT)
        | (link_bit << L_CODE_BIT)
        | offset;
    codegen_add_inst(ct, inst);
}

void codegen_table_add_pair(struct codegen_label_pair *lp, int index, 
        char *label, int offset) {
    strncpy(lp[index].label, label, SCAN_TOKEN_LEN);
    lp[index].offset = offset;
}

void codegen_inst(struct codegen_table_st *ct, struct parse_node_st *np) {
    /*  if there's a label associated with the instruction, add to the labels array in the codegen table
        when add label to labels array, we need the offset/index. previous formula needs to ignore directives
        as indexes */
    if (strlen(np->stmt.inst.label) > 0) {
        codegen_table_add_pair(ct->labels, ct->label_count, np->stmt.inst.label, ct->len);
        ct->label_count = ct->label_count + 1;
    }

    switch (np->stmt.inst.type) {
        case DP3    : codegen_dp3(ct, np); break;
        case DPIMM  : codegen_dpimm(ct, np); break;
        case CMP    : codegen_cmp(ct, np); break;
        case CMPIMM : codegen_cmpimm(ct, np); break;
        case MUL3   : codegen_mul3(ct, np); break;
        case MEM2   : codegen_mem2(ct, np); break;
        case MEM3   : codegen_mem3(ct, np); break;
        case MEMIMM : codegen_memimm(ct, np); break;
        case LS     : codegen_ls(ct, np); break;
        case B      : codegen_b(ct, np); break;
        case BX     : codegen_bx(ct, np); break;
        default     : codegen_error("unknown stmt.inst.type");
    }
}

void codegen_stmt(struct codegen_table_st *ct, struct parse_node_st *np) {
    /* add to publics array in ct->labels. when adding global dir label to publics, the offset should be 0 */
    if (np->type == DIR) {
        ct->publics->offset = 0;
        codegen_table_add_pair(ct->publics, ct->public_count, np->stmt.inst.label, ct->len);
        ct->public_count = ct->public_count + 1;
    } else if (np->type == INST) {
        codegen_inst(ct, np);
    } else if (np->type == SEQ) {
        codegen_stmt(ct, np->stmt.seq.left);
        codegen_stmt(ct, np->stmt.seq.right);
    }
}

bool codegen_is_public_label(struct codegen_table_st *ct, struct codegen_label_pair *pl) {
    for (int l = 0; l < ct->public_count; l++) {
        if (!strcmp(ct->publics[l].label, pl->label))
            return true;
    }
    return false;
}

void codegen_print_hex(struct codegen_table_st *ct) {
    int i;

    printf("v2.0 raw\n");
    for (i = 0; i < ct->len; i++) {
        printf("%08X\n", ct->table[i]);
    }
}

void codegen_write(struct codegen_table_st *ct, char *path) {
    int i;
    FILE *obj = fopen(path, "w");

    fprintf(obj, "v2.0 raw\n");
    for (i = 0; i < ct->len; i++) {
        fprintf(obj, "%08X\n", ct->table[i]);
    }
    fclose(obj);
}

void codegen_elf_write(struct codegen_table_st *ct, char *path) {
    int i;
    elf_context elf;
    struct codegen_label_pair *pl;
    int binding;

    elf_init(&elf);
    for (i = 0; i < ct->label_count; i++) {
        pl = &ct->labels[i];
        if (codegen_is_public_label(ct, pl)) {
            binding = STB_GLOBAL;
        } else {
            binding = STB_LOCAL;
        }
        elf_add_symbol(&elf, pl->label, pl->offset * 4, binding);
    }
    for (i = 0; i < ct->len; i++) {
        elf_add_instr(&elf, ct->table[i]);
    }

    FILE *f = fopen(path, "w");
    if (!f) {
        perror(path);
        return;
    }
    if (!elf_write_file(&elf, f)) {
        printf("elf_write_file failed\n");
    }
    fclose(f);
}

void codegen_hex_write(struct codegen_table_st *ct, struct parse_node_st *np, char *path) {
    codegen_write(ct, path);
}
