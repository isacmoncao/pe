# pe - português estruturado
#
# Copyright (c) 2025 José Isac Araujo Monção
#                                                   
# See LICENSE file for copyright and license details.
CC = gcc
TARGET = pe # The program name
CFLAGS = -std=c99 -Wall -Wextra -pedantic -O2
SRCS = src/main.c src/lexer.c src/util.c
OBJS = ${SRCS:.c=.o}
BUILD_DIR = build

# Main rule
all: ${BUILD_DIR}/${TARGET}

# Default rule to compile .c to .o
%.o: %.c
	${CC} ${CFLAGS} -c $< -o $@

${BUILD_DIR}/${TARGET}: ${OBJS}
	@mkdir -p ${BUILD_DIR}
	${CC} ${OBJS} -o $@

run: all
	@echo "Running ${TARGET} with examples/ex1.pe"
	@./${BUILD_DIR}/${TARGET} examples/ex1.pe 

clean:
	rm -rf ${OBJS} ${BUILD_DIR}

debug: clean
	${MAKE} CFLAGS="${CFLAGS} -g -O0" all

.PHONY: all clean run debug
