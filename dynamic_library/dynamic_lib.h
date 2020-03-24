//
// Created by talkytitan on 23.03.2020.
//

#ifndef DZ2_STATIC_LIB_H
#define DZ2_STATIC_LIB_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

#define DATA_SIZE 11

struct data {
    uint32_t diff[DATA_SIZE];
};

struct buffer {
    unsigned char* buf;
    size_t size;
};

struct vector {
    struct buffer** buf;
    size_t size;
    size_t capacity;
};

// struct data's method
struct data* init();

void clear(struct data* container);


// file's methods
FILE* open_file(char* filepath);

struct vector* read_data_from_file(FILE* file_handler);

void close_file(FILE* file_handler);

// string's methods
struct buffer* read_byte_string(FILE* file_handler);

void clear_string(struct buffer* string);

int count_difference_bytes(const struct vector* data, int diff);

struct data* process(char* filepath);

void print_result(struct data* container);


//vector's method
struct vector* init_vector();

int resize(struct vector* object);

int push(struct vector* object, struct buffer* value);

void delete_vector(struct vector* object);

#endif //DZ2_STATIC_LIB_H
