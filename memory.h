#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

#define DATA_MEM_SIZE 2048
#define INSTR_MEM_SIZE 1024

extern uint16_t instr_mem[INSTR_MEM_SIZE];
extern uint8_t data_mem[DATA_MEM_SIZE];
extern uint8_t R[64];
extern uint8_t SREG;
extern uint16_t PC;

void initialize_memory();
void print_final_state();

#endif