# CSEN601 Processor Simulation: Package 4 (Double McHarvard)

![C](https://img.shields.io/badge/C-17%2B-blue)
![Harvard](https://img.shields.io/badge/Architecture-Harvard-brightgreen)
![Pipeline](https://img.shields.io/badge/Pipeline-3_stage-orange)
![License](https://img.shields.io/badge/License-MIT-yellow)

A C implementation of a Harvard architecture processor simulator with circular shift operations, developed for CSEN601 Computer Systems Architecture.

## Key Features
- 🏗️ **Harvard Architecture**: Separate 16-bit instruction memory (1024 words) and 8-bit data memory (2048 words)
- ⚙️ **66 Registers**: 64x 8-bit GPRs + 1x 8-bit status register (SREG) + 1x 16-bit PC
- 🔄 **Circular Shifts**: Unique SLC (Shift Left Circular) and SRC (Shift Right Circular) instructions
- 🚦 **3-Stage Pipeline**: IF (Fetch), ID (Decode), EX (Execute) with hazard handling
- 🏷️ **Status Flags**: 5-bit SREG with Carry (C), Overflow (V), Negative (N), Sign (S), Zero (Z) flags
- ⏱️ **Cycle-Accurate Simulation**: Prints pipeline state after each clock cycle

## Instruction Set Architecture (12 Instructions)
| Mnemonic | Format | Operation |
|----------|--------|-----------|
| ADD      | R      | R1 = R1 + R2 |
| SUB      | R      | R1 = R1 - R2 |
| MUL      | R      | R1 = R1 * R2 |
| LDI      | I      | R1 = IMM |
| BEQZ     | I      | Branch if R1 == 0 |
| AND      | R      | R1 = R1 & R2 |
| OR       | R      | R1 = R1 \| R2 |
| JR       | R      | Jump to R1\|R2 |
| SLC      | I      | Circular left shift |
| SRC      | I      | Circular right shift |
| LB       | I      | Load from memory |
| SB       | I      | Store to memory |


## Requirements
- C compiler (GCC recommended)
- Make (for build automation)

## Build & Run
1. Clone the repository:
   ```bash
   git clone https://github.com/your-username/csen601-package4.git
   cd csen601-package4
2. Compile:
     make
3. Run simulation:
  
   ./simulator programs/example1.asm

