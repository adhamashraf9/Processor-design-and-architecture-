// pipeline.h
#ifndef PIPELINE_H
#define PIPELINE_H

#include "instruction.h"

typedef struct {
    int valid;
    Instruction instr;
} Stage;

extern Stage IF, ID, EX;

void fetch_stage();
void decode_stage();
void execute_stage();
int pipeline_active();
void print_pipeline_state();
void flush_pipeline();

#endif