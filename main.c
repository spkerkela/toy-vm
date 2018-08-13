#include "vm.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int *instructions;
int instruction_count = 0;

int stack[256];
int registers[NUM_OF_REGISTERS];

bool running = true;

void stack_pointer_incr(int i) { registers[SP] += i; }
void instruction_pointer_incr(int i) { registers[IP] += i; }

int stack_pointer() { return registers[SP]; }
int instruction_pointer() { return registers[IP]; }

int fetch() {
  int instruction = instructions[instruction_pointer()];
  return instruction;
}

void eval(int instruction) {
  switch (instruction) {
  case HLT:
    running = false;
    break;
  case PSH:
    stack_pointer_incr(1);
    instruction_pointer_incr(1);
    stack[stack_pointer()] = instructions[instruction_pointer()];
    break;
  case POP: {
    int value = stack[stack_pointer()];
    stack_pointer_incr(-1);
    printf("%d\n", value);
    break;
  }
  case ADD: {
    int a = stack[stack_pointer()];
    stack_pointer_incr(-1);
    int b = stack[stack_pointer()];
    stack_pointer_incr(-1);
    int res = a + b;
    stack_pointer_incr(1);
    stack[stack_pointer()] = res;
    break;
  }
  case MUL: {
    int a = stack[stack_pointer()];
    stack_pointer_incr(-1);
    int b = stack[stack_pointer()];
    stack_pointer_incr(-1);
    int res = a * b;
    stack_pointer_incr(1);
    stack[stack_pointer()] = res;
    break;
  }
  case DIV: {
    int a = stack[stack_pointer()];
    stack_pointer_incr(-1);
    int b = stack[stack_pointer()];
    stack_pointer_incr(-1);
    int res = b / a;
    stack_pointer_incr(1);
    stack[stack_pointer()] = res;
    break;
  }
  case SET: {
    instruction_pointer_incr(1);
    int reg = instructions[instruction_pointer()];
    instruction_pointer_incr(1);
    int value = instructions[instruction_pointer()];
    registers[reg] = value;
    break;
  }
  default:
    printf("unknown: %d\n", instruction);
    break;
  }
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
  // read the "binary" file
  int instruction_space = 4;
  instructions =
      malloc(sizeof(*instructions) * instruction_space); // 4 instructions

  int num;
  int i = 0;
  while (fscanf(file, "%d", &num) > 0) {
    instructions[i] = num;
    i++;
    if (i >= instruction_space) {
      instruction_space *= 2;
      instructions =
          realloc(instructions,
                  sizeof(*instructions) * instruction_space); // double size
    }
  }

  // set 'instruction_count' to number of instructions read
  instruction_count = i;

  // close the file
  fclose(file);

  while (running && instruction_pointer() < instruction_count) {
    eval(fetch());
    instruction_pointer_incr(1);
  }
  return 0;
}
