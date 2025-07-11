// instruction.h
#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <stdint.h>

#define INSTR_MEM_SIZE 1024

typedef struct {
    uint16_t raw;
    uint16_t pc;
    uint8_t opcode;
    uint8_t r1;
    uint8_t r2_or_imm;
} Instruction;

void load_instructions(const char* filename);
Instruction decode(uint16_t raw, uint16_t pc);
uint16_t assemble_instruction(const char* line);
int get_program_size(void);

#endif // INSTRUCTION_H

