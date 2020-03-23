#include "static_lib.h"

//
// Created by talkytitan on 23.03.2020.
//

struct data* init() {
    struct data* container = malloc(sizeof(struct data));
    if (!container) {
        return NULL;
    }

    for (int i = 0; i < DATA_SIZE; i++) {
        container->diff[i] = 0;
    }

    return container;
}

void clear(struct data* container) {
    if (container->diff) {
        free(container->diff);
    }
    free(container);
}

FILE* open_file(char* filepath) {
    FILE* fh = fopen(filepath, "rb");
    if (!fh) {
        return NULL;
    }
    return fh;
}

struct buffer* read_byte_string(FILE* file_handler) {
    char c;
    int count_bytes = 0;
    while (c = fgetc(file_handler), c != EOF && c != '\n') {
        count_bytes++;
    }

    if (!count_bytes) {
        return NULL;
    }

    struct buffer* string = malloc(sizeof(struct buffer));
    if (!string) {
        return NULL;
    }
    string->size = count_bytes + 1;
    unsigned char* tmp = (unsigned char*)malloc(string->size * sizeof(unsigned char));

    if (c == EOF) {
        fseek(file_handler, -count_bytes, SEEK_END);
    } else {
        fseek(file_handler, -string->size, SEEK_CUR);
    }
    for (int i = 0; i < count_bytes; i++) {
        tmp[i] = fgetc(file_handler);
    }
    tmp[count_bytes + 1] = '\0';
    string->buf = tmp;

    if (c == '\n') {
        c = fgetc(file_handler);
    }
    return string;
}

void clear_string(struct buffer* string) {
    if (string->buf) {
        free(string->buf);
    }
    free(string);
}

void close_file(FILE* file_handler) {
    fclose(file_handler);
}

struct data* count_difference_bytes(FILE* file_handler) {
    struct data* container = init();
    if (!container) {
        return NULL;
    }
    struct buffer* string;
    while (string = read_byte_string(file_handler), string) {
        for (int i = 0; i < string->size - 1; i++) {
            int left = string->buf[i];
            int right = string->buf[i + 1];
            int result = abs(left - right);
            if (result < DATA_SIZE) {
                container->diff[result]++;
            }
        }
        clear_string(string);
    }

    return container;
}

struct data* process(char* filepath) {
    FILE* fh = open_file(filepath);
    if (!fh) {
        return NULL;
    }

    struct data* container = count_difference_bytes(fh);
    if (!container) {
        return NULL;
    }

    close_file(fh);
    return container;
}

void print_result(struct data* container) {
    if (!container) {
        printf("Пустой контейнер\n");
        return;
    }

    printf("Итоги обработки файла:\n");
    for (int i = 0; i < DATA_SIZE; i++) {
        printf("Всего %d соседних байт, разница значений которых равна %d\n", container->diff[i], i);
    }
}