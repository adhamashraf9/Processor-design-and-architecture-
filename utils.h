#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>


// Bit positions (do NOT redefine these in .c)
#define Z_FLAG 0
#define N_FLAG 1
#define S_FLAG 2
#define V_FLAG 3
#define C_FLAG 4

// Function to print flag status
void print_flags(void);

void set_flags_add(uint8_t a, uint8_t b, uint16_t result);
void set_flags_sub(uint8_t a, uint8_t b, uint16_t result);
void set_flags_logic(uint8_t result);
void set_flags_shift(uint8_t result);
void set_flags_mul(uint8_t result);

#endif
