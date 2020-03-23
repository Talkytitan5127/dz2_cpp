//
// Created by talkytitan on 23.03.2020.
//

#ifndef DZ2_STATIC_LIB_H
#define DZ2_STATIC_LIB_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

#define DATA_SIZE 11

struct data {
    uint32_t diff[DATA_SIZE];
};

struct buffer {
    unsigned char* buf;
    size_t size;
    size_t capacity;
};

struct data* init();

void clear(struct data* container);

FILE* open_file(char* filepath);

struct buffer* read_byte_string(FILE* file_handler);

void clear_string(struct buffer* string);

void close_file(FILE* file_handler);

struct data* count_difference_bytes(FILE* file_handler);

struct data* process(char* filepath);

void print_result(struct data* container);

#endif //DZ2_STATIC_LIB_H
