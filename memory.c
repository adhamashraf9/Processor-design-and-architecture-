
#include "memory.h"
#include <stdio.h>
#include <string.h>

uint8_t data_mem[DATA_MEM_SIZE];
uint8_t R[64];
uint8_t SREG;
uint16_t PC = 0;

void initialize_memory() {
    memset(data_mem, 0, DATA_MEM_SIZE);
    memset(R, 0, 64);
    SREG = 0;
    PC = 0;
}

void print_final_state() {
    printf("\n=== Final Register State ===\n");
    for (int i = 0; i < 64; i++) {
        if (R[i] != 0) {
            printf("R%-2d: 0x%02X\n", i, R[i]);
        }
    }
    printf("PC  : 0x%04X\n", PC);
    printf("SREG: 0x%02X\n", SREG);

    printf("\n=== Final Data Memory ===\n");
    for (int i = 0; i < DATA_MEM_SIZE; i++) {
        if (data_mem[i] != 0) {
            printf("MEM[0x%04X]: 0x%02X\n", i, data_mem[i]);
        }
    }
}