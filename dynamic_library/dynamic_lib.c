#include "dynamic_lib.h"

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

struct vector* read_data_from_file(FILE* file_handler) {
    struct vector* result;
    struct buffer* string;

    result = init_vector();
    while(string = read_byte_string(file_handler), string) {
        push(result, string);
    }

    return result;
}

void close_file(FILE* file_handler) {
    fclose(file_handler);
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
    tmp[string->size] = '\0';
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

int count_difference_bytes(const struct vector* data, int diff) {
    int count = 0;
    for (int word = 0; word < data->size; word++) {
        struct buffer* string = data->buf[word];
        for (int i = 0; i < string->size - 1; i++) {
            int left = string->buf[i];
            int right = string->buf[i + 1];
            int result = abs(left - right);
            if (result == diff) {
                count++;
            }
        }
    }

    return count;
}

struct data* process(char* filepath) {
    FILE* fh = open_file(filepath);
    if (!fh) {
        close_file(fh);
        return NULL;
    }

    struct vector* data_from_file;
    data_from_file = read_data_from_file(fh);
    close_file(fh);

    if (!data_from_file) {
        return NULL;
    }

    struct data* container = init();
    if (!container) {
        delete_vector(data_from_file);
        return NULL;
    }

    for (int diff_ind = 0; diff_ind < DATA_SIZE; diff_ind++) {
        int status = 0;
        int* shared_count = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE,
                MAP_SHARED | MAP_ANONYMOUS, -1, 0);

        *shared_count = 0;
        if (!fork()) {
            *shared_count = count_difference_bytes(&data_from_file[0], diff_ind);
            exit(0);
        } else {
            wait(NULL);
        }

        container->diff[diff_ind] = *shared_count;

    }

    delete_vector(data_from_file);

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

struct vector* init_vector() {
    struct vector* obj;
    obj = malloc(sizeof(struct vector));
    if (!obj) {
        return NULL;
    }
    obj->size = 0;
    obj->capacity = 1;
    obj->buf = (struct buffer**)malloc(obj->capacity * sizeof(struct buffer*));
    if (!obj->buf) {
        free(obj);
        return NULL;
    }

    return obj;
}

int resize(struct vector* object) {
    if (!object) {
        return EXIT_FAILURE;
    }

    object->capacity *= 2;
    struct buffer** tmp = (struct buffer**)malloc(object->capacity * sizeof(struct buffer*));
    if (!tmp) {
        return EXIT_FAILURE;
    }
    for (int i = 0; i < object->size; i++) {
        tmp[i] = object->buf[i];
    }

    free(object->buf);
    object->buf = tmp;
    return EXIT_SUCCESS;
}

int push(struct vector* object, struct buffer* value) {
    size_t new_size = object->size + 1;
    if (new_size >= object->capacity) {
        if (resize(object)) {
            return EXIT_FAILURE;
        }
    }
    object->buf[object->size++] = value;
    return EXIT_SUCCESS;
}

void delete_vector(struct vector* object) {
    for (int i = 0; i < object->size; i++) {
        if (object->buf[i]) {
            clear_string(object->buf[i]);
        }
    }
    if (object->buf) {
        free(object->buf);
    }
    free(object);
}