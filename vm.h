#ifndef __VM_H
#define __VM_H
typedef enum {
  HLT,
  PSH,
  ADD,
  POP,
  MUL,
  DIV,
  SET,
  GET,
  SAV,
  PRN,
  INC,
  DEC,
  IF,
  IFN
} InstructionSet;
typedef enum { A, B, C, D, E, F, SP, IP, NUM_OF_REGISTERS } Registers;
#endif
