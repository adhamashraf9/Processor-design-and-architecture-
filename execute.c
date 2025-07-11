#include "execute.h"
#include "memory.h"
#include "utils.h"
#include <stdio.h>
#include "instruction.h"

void execute_instruction(Instruction instr) {
    uint8_t op = instr.opcode;
    uint8_t r1 = instr.r1;
    uint8_t val = instr.r2_or_imm;
    uint16_t result;  // For operations needing carry detection

    switch (op) {
        case 0x0: // ADD
            result = R[r1] + R[val];
            set_flags_add(R[r1], R[val], result);
            R[r1] = result & 0xFF;
            printf("[EXEC] ADD R%d, R%d -> R%d = 0x%02X\n", r1, val, r1, R[r1]);
            break;

        case 0x1: // SUB
            result = R[r1] - R[val];
            set_flags_sub(R[r1], R[val], result);
            R[r1] = result & 0xFF;
            printf("[EXEC] SUB R%d, R%d -> R%d = 0x%02X\n", r1, val, r1, R[r1]);
            break;

        case 0x2: // MUL
            result = R[r1] * R[val];
            set_flags_mul(result & 0xFF);
            R[r1] = result & 0xFF;
            printf("[EXEC] MUL R%d, R%d -> R%d = 0x%02X\n", r1, val, r1, R[r1]);
            break;

        case 0x3: // AND
            R[r1] = R[r1] & R[val];
            set_flags_logic(R[r1]);
            printf("[EXEC] AND R%d, R%d -> R%d = 0x%02X\n", r1, val, r1, R[r1]);
            break;

        case 0x4: // OR
            R[r1] = R[r1] | R[val];
            set_flags_logic(R[r1]);
            printf("[EXEC] OR R%d, R%d -> R%d = 0x%02X\n", r1, val, r1, R[r1]);
            break;

        case 0x5: // SLC
            R[r1] = (R[r1] << val) | (R[r1] >> (8 - val));
            set_flags_shift(R[r1]);
            printf("[EXEC] SLC R%d, #%d -> R%d = 0x%02X\n", r1, val, r1, R[r1]);
            break;

        case 0x6: // SRC
            R[r1] = (R[r1] >> val) | (R[r1] << (8 - val));
            set_flags_shift(R[r1]);
            printf("[EXEC] SRC R%d, #%d -> R%d = 0x%02X\n", r1, val, r1, R[r1]);
            break;

        case 0x7: // LB
            R[r1] = data_mem[val];
            printf("[EXEC] LB R%d, [%d] -> R%d = 0x%02X\n", r1, val, r1, R[r1]);
            break;

        case 0x8: // SB
            data_mem[val] = R[r1];
            printf("[EXEC] SB R%d, [%d] -> MEM[%d] = 0x%02X\n", r1, val, val, R[r1]);
            break;

        case 0x9: // BEQZ
            if (R[r1] == 0) {
                PC = instr.pc + 1 + (int8_t)val;  // Sign-extend immediate
                flush_pipeline();
                printf("[EXEC] BEQZ R%d, %d -> Branch taken, PC = %d\n", r1, val, PC);
            } else {
                printf("[EXEC] BEQZ R%d, %d -> Branch not taken\n", r1, val);
            }
            break;

        case 0xA: // JR
            PC = (R[r1] << 8) | R[val];
            flush_pipeline();
            printf("[EXEC] JR R%d, R%d -> PC = 0x%04X\n", r1, val, PC);
            break;

        case 0xB: // MOVI
            R[r1] = val;
            printf("[EXEC] MOVI R%d, #%d -> R%d = 0x%02X\n", r1, val, r1, R[r1]);
            break;
    }
}