#include "instruction.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint16_t instr_mem[INSTR_MEM_SIZE];
int program_length = 0;  // Track how many instructions were loaded

uint16_t assemble_instruction(const char* line) {
    char opcode[10];
    int r1, r2_or_imm;

    // Handle LDI instruction format (LDI Rx value)
    if (sscanf(line, "LDI R%d %d", &r1, &r2_or_imm) == 2) {
        return (0xB << 12) | (r1 << 6) | (r2_or_imm & 0x3F); // LDI is opcode 0xB
    }
    // Handle standard instruction format (OP Rx Ry)
    else if (sscanf(line, "%s R%d R%d", opcode, &r1, &r2_or_imm) == 3) {
        uint16_t op = 0;

        if (strcmp(opcode, "ADD") == 0) op = 0;
        else if (strcmp(opcode, "SUB") == 0) op = 1;
        else if (strcmp(opcode, "MUL") == 0) op = 2;
        else if (strcmp(opcode, "AND") == 0) op = 3;
        else if (strcmp(opcode, "OR") == 0) op = 4;
        else if (strcmp(opcode, "JR") == 0) op = 0xA;
        else return 0;

        return (op << 12) | (r1 << 6) | (r2_or_imm & 0x3F);
    }
    // Handle immediate format for shift operations (OP Rx #imm)
    else if (sscanf(line, "%s R%d %d", opcode, &r1, &r2_or_imm) == 3) {
        uint16_t op = 0;

        if (strcmp(opcode, "SLC") == 0) op = 5;
        else if (strcmp(opcode, "SRC") == 0) op = 6;
        else if (strcmp(opcode, "LB") == 0) op = 7;
        else if (strcmp(opcode, "SB") == 0) op = 8;
        else if (strcmp(opcode, "BEQZ") == 0) op = 9;
        else return 0;

        return (op << 12) | (r1 << 6) | (r2_or_imm & 0x3F);
    }

    return 0;
}

void load_instructions(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening input file");
        return;
    }

    char line[100];
    int i = 0;
    program_length = 0;  // Reset program length
    
    while (fgets(line, sizeof(line), file) && i < INSTR_MEM_SIZE) {
        // Skip empty lines and comments (lines starting with ;)
        if (strlen(line) <= 1 || line[0] == ';') continue;
        
        // Remove comments from the line
        char* comment = strchr(line, ';');
        if (comment) *comment = '\0';
        
        // Skip if line is now empty after removing comment
        if (strlen(line) <= 1) continue;
        
        uint16_t instr = assemble_instruction(line);
        if (instr != 0) {
            instr_mem[i++] = instr;
            program_length = i;  // Update program length
            printf("Loaded instruction %d: 0x%04X\n", i-1, instr);
        } else {
            printf("Warning: Could not assemble instruction: %s", line);
        }
    }

    fclose(file);
}

int get_program_size(void) {
    return program_length;
}

Instruction decode(uint16_t raw, uint16_t pc) {
    Instruction instr;
    instr.raw = raw;
    instr.opcode = (raw >> 12) & 0xF;
    instr.r1 = (raw >> 6) & 0x3F;
    instr.r2_or_imm = raw & 0x3F;
    instr.pc = pc;
    return instr;
}
