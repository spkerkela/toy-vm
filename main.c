#include "vm.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int *instructions;
int instruction_count = 0;

int stack[256];
int registers[NUM_OF_REGISTERS];

bool running = true;

void sp_incr(int i) { registers[SP] += i; }
void ip_incr(int i) { registers[IP] += i; }

int sp() { return registers[SP]; }
int ip() { return registers[IP]; }

int fetch() { return instructions[ip()]; }

void eval(int instruction) {
  switch (instruction) {
  case HLT:
    running = false;
    break;
  case PSH:
    sp_incr(1);
    ip_incr(1);
    stack[sp()] = instructions[ip()];
    break;
  case POP: {
    sp_incr(-1);
    break;
  }
  case PRN: {
    printf("%d\n", stack[sp()]);
    break;
  }
  case ADD: {
    int a = stack[sp()];
    sp_incr(-1);
    int b = stack[sp()];
    sp_incr(-1);
    int res = a + b;
    sp_incr(1);
    stack[sp()] = res;
    break;
  }
  case MUL: {
    int a = stack[sp()];
    sp_incr(-1);
    int b = stack[sp()];
    sp_incr(-1);
    int res = a * b;
    sp_incr(1);
    stack[sp()] = res;
    break;
  }
  case DIV: {
    int a = stack[sp()];
    sp_incr(-1);
    int b = stack[sp()];
    sp_incr(-1);
    int res = b / a;
    sp_incr(1);
    stack[sp()] = res;
    break;
  }
  case SET: {
    registers[instructions[ip() + 1]] = instructions[ip() + 2];
    ip_incr(2);
    break;
  }
  case GET: {
    int register_value = registers[instructions[ip() + 1]];
    sp_incr(1);
    stack[sp()] = register_value;
    ip_incr(1);
    break;
  }
  case SAV: {
    registers[instructions[ip() + 1]] = stack[sp()];
    ip_incr(1);
    break;
  }
  default:
    printf("unknown: %d\n", instruction);
    break;
  }
}

int get_instruction(char *inst) {
  if (strcmp(inst, "PRN") == 0) {
    return PRN;
  }
  if (strcmp(inst, "PSH") == 0) {
    return PSH;
  }
  if (strcmp(inst, "POP") == 0) {
    return POP;
  }
  if (strcmp(inst, "MUL") == 0) {
    return MUL;
  }
  if (strcmp(inst, "HLT") == 0) {
    return HLT;
  }
  if (strcmp(inst, "ADD") == 0) {
    return ADD;
  }
  if (strcmp(inst, "DIV") == 0) {
    return DIV;
  }
  if (strcmp(inst, "SET") == 0) {
    return SET;
  }
  if (strcmp(inst, "GET") == 0) {
    return GET;
  }
  if (strcmp(inst, "SAV") == 0) {
    return SAV;
  }

  if (strcmp(inst, "A") == 0) {
    return A;
  }
  if (strcmp(inst, "B") == 0) {
    return B;
  }
  if (strcmp(inst, "C") == 0) {
    return C;
  }
  if (strcmp(inst, "D") == 0) {
    return D;
  }
  if (strcmp(inst, "E") == 0) {
    return E;
  }
  if (strcmp(inst, "F") == 0) {
    return F;
  }
  if (strcmp(inst, "SP") == 0) {
    return SP;
  }
  if (strcmp(inst, "IP") == 0) {
    return IP;
  }
  return atoi(inst);
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "No input files given.\n");
    exit(1);
  }
  registers[IP] = 0;
  registers[SP] = -1;
  FILE *file;

  file = fopen(argv[1], "r");
  if (file == NULL) {
    fprintf(stderr, "Can't open input file!\n");
    exit(1);
  }
  int instruction_space = 4;
  instructions =
      malloc(sizeof(*instructions) * instruction_space); // 4 instructions

  char inst[50];
  int i = 0;
  int instruction_decoded = 0;
  while (fscanf(file, "%s", inst) > 0) {
    instruction_decoded = get_instruction(inst);
    instructions[i] = instruction_decoded;
    i++;
    if (i >= instruction_space) {
      instruction_space = instruction_space * 2;
      int *tmp =
          realloc(instructions, sizeof(*instructions) * instruction_space);
      if (tmp == NULL) {
      } else {
        instructions = tmp;
      }
    }
  }

  instruction_count = i;

  while (running && ip() < instruction_count) {
    eval(fetch());
    ip_incr(1);
  }
  fclose(file);
  return 0;
}
