#include "utils.h"
#include "memory.h"
#include <stdio.h>

// Global function to print flag status - useful for debugging
void print_flags() {
    printf("Flags: Z=%d N=%d S=%d V=%d C=%d\n",
           (SREG >> Z_FLAG) & 1,
           (SREG >> N_FLAG) & 1,
           (SREG >> S_FLAG) & 1,
           (SREG >> V_FLAG) & 1,
           (SREG >> C_FLAG) & 1);
}

void clear_sreg() {
    SREG &= 0b11100000; // Clear bottom 5 bits (flags)
}

void set_bit(uint8_t bit) { SREG |= (1 << bit); }
void clear_bit(uint8_t bit) { SREG &= ~(1 << bit); }

void update_zero_negative(uint8_t result) {
    if (result == 0) set_bit(Z_FLAG); else clear_bit(Z_FLAG);
    if ((result >> 7) & 1) set_bit(N_FLAG); else clear_bit(N_FLAG);
}

void update_sign() {
    uint8_t n = (SREG >> N_FLAG) & 1;
    uint8_t v = (SREG >> V_FLAG) & 1;
    if (n ^ v) set_bit(S_FLAG); else clear_bit(S_FLAG);
}

void set_flags_add(uint8_t a, uint8_t b, uint16_t result) {
    // Calculate result before modifying flags
    uint8_t res8 = result & 0xFF;
    clear_sreg();
    
    // Carry flag - Set if result doesn't fit in 8 bits
    if (result > 0xFF) set_bit(C_FLAG);
    
    // Overflow flag - Set if sign of result is unexpected
    uint8_t sa = (a >> 7) & 1;
    uint8_t sb = (b >> 7) & 1;
    uint8_t sr = (res8 >> 7) & 1;
    if ((sa == sb) && (sr != sa)) set_bit(V_FLAG);

    // Set zero and negative flags
    update_zero_negative(res8);
    
    // Set sign flag (S = N ⊕ V)
    update_sign();
    
    print_flags();
}

void set_flags_sub(uint8_t a, uint8_t b, uint16_t result) {
    uint8_t res8 = result & 0xFF;
    clear_sreg();

    // Set carry flag if b > a (for unsigned)
    if (b > a) set_bit(C_FLAG);

    // Overflow flag - Set if sign of result is unexpected
    uint8_t sa = (a >> 7) & 1;
    uint8_t sb = (b >> 7) & 1;
    uint8_t sr = (res8 >> 7) & 1;
    if ((sa != sb) && (sr == sb)) set_bit(V_FLAG);

    // Set zero and negative flags
    update_zero_negative(res8);
    
    // Set sign flag
    update_sign();
    
    print_flags();
}

void set_flags_logic(uint8_t result) {
    clear_sreg();
    
    // Clear carry and overflow flags
    clear_bit(C_FLAG);
    clear_bit(V_FLAG);
    
    // Set zero and negative flags
    update_zero_negative(result);
    
    // For logical operations, S = N
    if ((result >> 7) & 1) set_bit(S_FLAG); else clear_bit(S_FLAG);
    
    print_flags();
}

void set_flags_shift(uint8_t result) {
    clear_sreg();
    
    // Overflow is cleared
    clear_bit(V_FLAG);
    
    // Set zero and negative flags
    update_zero_negative(result);
    
    // For shift operations, S = N
    if ((result >> 7) & 1) set_bit(S_FLAG); else clear_bit(S_FLAG);
    
    print_flags();
}

void set_flags_mul(uint8_t result) {
    clear_sreg();
    
    // Set zero and negative flags
    update_zero_negative(result);
    
    // For multiply, S = N
    if ((result >> 7) & 1) set_bit(S_FLAG); else clear_bit(S_FLAG);
    
    print_flags();
}
