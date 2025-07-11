// main.c
#include <stdio.h>
#include "instruction.h"
#include "memory.h"
#include "pipeline.h"
#include "utils.h"

int main() {
    initialize_memory();
    
    // Load the test program
    printf("=== Loading Program ===\n");
    load_instructions("test.asm");
    
    // Track actual program size
    int program_size = get_program_size();
    printf("Program size: %d instructions\n", program_size);
    
    int cycle = 1;
    
    printf("\n=== Starting Program Execution ===\n");
    
    while ((pipeline_active() || PC < program_size) && PC < INSTR_MEM_SIZE) {
        printf("\n=== Clock Cycle %d ===\n", cycle);
        
        // Execute stages in reverse order (to avoid data conflicts)
        execute_stage();
        decode_stage();
        fetch_stage();
        
        // Print pipeline state after execution
        print_pipeline_state();
        
        // Print register changes if any
        printf("\n=== Register State ===\n");
        for (int i = 0; i < 64; i++) {
            if (R[i] != 0 ) {
                printf("R%-2d: 0x%02X\n", i, R[i]);
            }
        }
        
        // Print status register
        printf("\n=== Status Register ===\n");
        printf("SREG: 0x%02X\n", SREG);
        printf("  Z (Zero):      %d\n", (SREG >> Z_FLAG) & 1);
        printf("  N (Negative):  %d\n", (SREG >> N_FLAG) & 1);
        printf("  S (Sign):      %d\n", (SREG >> S_FLAG) & 1);
        printf("  V (Overflow):  %d\n", (SREG >> V_FLAG) & 1);
        printf("  C (Carry):     %d\n", (SREG >> C_FLAG) & 1);
        
        // Print memory changes if any
        printf("\n=== Memory State ===\n");
        int memory_used = 0;
        for (int i = 0; i < DATA_MEM_SIZE; i++) {
            if (data_mem[i] != 0) {
                printf("MEM[0x%04X]: 0x%02X\n", i, data_mem[i]);
                memory_used = 1;
            }
        }
        if (!memory_used) {
            printf("No memory locations used yet.\n");
        }
        
        cycle++;
        
        // Stop if we've hit a reasonable cycle count (to prevent infinite loops)
        if (cycle > 100) {
            printf("\n=== Maximum cycle count reached, halting simulation ===\n");
            break;
        }
    }

    printf("\n=== Program Execution Complete ===\n");
    printf("Total Clock Cycles: %d\n", cycle - 1);
    print_final_state();
    
    return 0;
}
