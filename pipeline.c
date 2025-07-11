// pipeline.c
#include "pipeline.h"
#include "utils.h"
#include "memory.h"
#include <stdio.h>
#include <string.h>

Stage IF = {0}, ID = {0}, EX = {0};
int stall=0;

void fetch_stage() {
    if (!stall && !IF.valid && PC < INSTR_MEM_SIZE) {
        // Only fetch if there's a valid instruction
        if (instr_mem[PC] != 0) {
            IF.instr.raw = instr_mem[PC];
            IF.instr.pc = PC;
            IF.valid = 1;
            printf("[FETCH] PC: %d, Instruction: 0x%04X\n", PC, instr_mem[PC]);
        } else {
            printf("[FETCH] PC: %d, No valid instruction found\n", PC);
        }
        PC++;
    } else if (IF.valid) {
        printf("[FETCH] Fetch stage full, waiting...\n");
    } else {
        printf("[FETCH] No more instructions to fetch\n");
    }
}

void decode_stage() {
    if (IF.valid && !ID.valid) {
        ID.instr = decode(IF.instr.raw, IF.instr.pc);
        ID.valid = 1;
        IF.valid = 0;
        printf("[DECODE] PC: %d, Opcode: 0x%X, R1: %d, R2/IMM: %d\n", 
               ID.instr.pc, ID.instr.opcode, ID.instr.r1, ID.instr.r2_or_imm);
    } else if (ID.valid) {
        printf("[DECODE] Decode stage full, waiting...\n");
    } else {
        printf("[DECODE] No instruction to decode\n");
    }
}

void execute_stage() {
    if (ID.valid && !EX.valid) {
        EX.instr = ID.instr;
        EX.valid = 1;
        ID.valid = 0;

        switch (EX.instr.opcode) {
            case 0x0: // ADD
                R[EX.instr.r1] = R[EX.instr.r1] + R[EX.instr.r2_or_imm];
                set_flags_add(R[EX.instr.r1], R[EX.instr.r2_or_imm], R[EX.instr.r1]);
                printf("[EXEC] ADD R%d, R%d -> R%d = 0x%02X\n", 
                       EX.instr.r1, EX.instr.r2_or_imm, EX.instr.r1, R[EX.instr.r1]);
                break;

            case 0x1: // SUB
                R[EX.instr.r1] = R[EX.instr.r1] - R[EX.instr.r2_or_imm];
                set_flags_sub(R[EX.instr.r1], R[EX.instr.r2_or_imm], R[EX.instr.r1]);
                printf("[EXEC] SUB R%d, R%d -> R%d = 0x%02X\n", 
                       EX.instr.r1, EX.instr.r2_or_imm, EX.instr.r1, R[EX.instr.r1]);
                break;

            case 0x2: // MUL
                R[EX.instr.r1] = R[EX.instr.r1] * R[EX.instr.r2_or_imm];
                set_flags_mul(R[EX.instr.r1]);
                printf("[EXEC] MUL R%d, R%d -> R%d = 0x%02X\n", 
                       EX.instr.r1, EX.instr.r2_or_imm, EX.instr.r1, R[EX.instr.r1]);
                break;

            case 0x3: // AND
                R[EX.instr.r1] = R[EX.instr.r1] & R[EX.instr.r2_or_imm];
                set_flags_logic(R[EX.instr.r1]);
                printf("[EXEC] AND R%d, R%d -> R%d = 0x%02X\n", 
                       EX.instr.r1, EX.instr.r2_or_imm, EX.instr.r1, R[EX.instr.r1]);
                break;

            case 0x4: // OR
                R[EX.instr.r1] = R[EX.instr.r1] | R[EX.instr.r2_or_imm];
                set_flags_logic(R[EX.instr.r1]);
                printf("[EXEC] OR R%d, R%d -> R%d = 0x%02X\n", 
                       EX.instr.r1, EX.instr.r2_or_imm, EX.instr.r1, R[EX.instr.r1]);
                break;

            case 0x5: // SLC
                R[EX.instr.r1] = (R[EX.instr.r1] << EX.instr.r2_or_imm) | 
                                (R[EX.instr.r1] >> (8 - EX.instr.r2_or_imm));
                set_flags_shift(R[EX.instr.r1]);
                printf("[EXEC] SLC R%d, #%d -> R%d = 0x%02X\n", 
                       EX.instr.r1, EX.instr.r2_or_imm, EX.instr.r1, R[EX.instr.r1]);
                break;

            case 0x6: // SRC
                R[EX.instr.r1] = (R[EX.instr.r1] >> EX.instr.r2_or_imm) | 
                                (R[EX.instr.r1] << (8 - EX.instr.r2_or_imm));
                set_flags_shift(R[EX.instr.r1]);
                printf("[EXEC] SRC R%d, #%d -> R%d = 0x%02X\n", 
                       EX.instr.r1, EX.instr.r2_or_imm, EX.instr.r1, R[EX.instr.r1]);
                break;

            case 0x7: // LB
                R[EX.instr.r1] = data_mem[EX.instr.r2_or_imm];
                printf("[EXEC] LB R%d, [%d] -> R%d = 0x%02X\n", 
                       EX.instr.r1, EX.instr.r2_or_imm, EX.instr.r1, R[EX.instr.r1]);
                break;

            case 0x8: // SB
                data_mem[EX.instr.r2_or_imm] = R[EX.instr.r1];
                printf("[EXEC] SB R%d, [%d] -> MEM[%d] = 0x%02X\n", 
                       EX.instr.r1, EX.instr.r2_or_imm, EX.instr.r2_or_imm, R[EX.instr.r1]);
                break;

            case 0x9: // BEQZ
                if (R[EX.instr.r1] == 0) {
                    PC = EX.instr.pc + 1 + EX.instr.r2_or_imm;
                    flush_pipeline();
                    printf("[EXEC] BEQZ R%d, %d -> Branch taken, PC = %d\n", 
                           EX.instr.r1, EX.instr.r2_or_imm, PC);
                } else {
                    printf("[EXEC] BEQZ R%d, %d -> Branch not taken\n", 
                           EX.instr.r1, EX.instr.r2_or_imm);
                }
                break;

            case 0xA: // JR
                PC = (R[EX.instr.r1] << 8) | R[EX.instr.r2_or_imm];
                flush_pipeline();
                printf("[EXEC] JR R%d, R%d -> Jump to 0x%04X\n", 
                       EX.instr.r1, EX.instr.r2_or_imm, PC);
                break;
                
            case 0xB: // LDI (Load Immediate)
                R[EX.instr.r1] = EX.instr.r2_or_imm;
                printf("[EXEC] LDI R%d, #%d -> R%d = 0x%02X\n", 
                       EX.instr.r1, EX.instr.r2_or_imm, EX.instr.r1, R[EX.instr.r1]);
                break;
                
            default:
                printf("[EXEC] ERROR: Unknown opcode 0x%X\n", EX.instr.opcode);
                break;
        }
        
        // Clear EX stage after execution
        EX.valid = 0;
    } else if (EX.valid) {
        printf("[EXEC] Execute stage full, waiting...\n");
    } else {
        printf("[EXEC] No instruction to execute\n");
    }
}

int pipeline_active() {
    return IF.valid || ID.valid || EX.valid;
}

void flush_pipeline() {
    IF.valid = 0;
    ID.valid = 0;
    // EX stage continues (already being executed)
}

void print_pipeline_state() {
    printf("\n=== Pipeline State ===\n");
    if (IF.valid) {
        printf("[IF] PC: %d, Instruction: 0x%04X\n", IF.instr.pc, IF.instr.raw);
    } else {
        printf("[IF] Empty\n");
    }

    if (ID.valid) {
        printf("[ID] PC: %d, Opcode: 0x%X, R1: %d, R2/IMM: %d\n", 
               ID.instr.pc, ID.instr.opcode, ID.instr.r1, ID.instr.r2_or_imm);
    } else {
        printf("[ID] Empty\n");
    }

    if (EX.valid) {
        printf("[EX] PC: %d, Opcode: 0x%X, R1: %d, R2/IMM: %d\n", 
               EX.instr.pc, EX.instr.opcode, EX.instr.r1, EX.instr.r2_or_imm);
    } else {
        printf("[EX] Empty\n");
    }
}
