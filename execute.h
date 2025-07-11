#ifndef EXECUTE_H
#define EXECUTE_H

#include "instruction.h"

void execute_instruction(Instruction instr);
void flush_pipeline(); // required for BEQZ, JR, BR

#endif
