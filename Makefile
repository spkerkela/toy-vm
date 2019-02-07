SRC_FILES = main.c
CC_FLAGS = -Wall -Wextra -g -std=c99 -O1
CC = clang

all:
	${CC} ${SRC_FILES} ${CC_FLAGS} -o vm
