#include <gtest/gtest.h>

extern "C" {
#include <interface.h>
}

TEST(data_obj, init_ok) {
    struct data* obj;
    obj = init();
    ASSERT_TRUE(obj);
    clear(obj);
}

TEST(string_obj, init_ok) {
    unsigned char str[] = "value";
    struct buffer* string = init_string(str);
    ASSERT_EQ(*string->buf, *str);
    clear_string(string);
}

TEST(vector_obj, vector_ok) {
    struct vector* obj = init_vector();
    ASSERT_TRUE(obj);
    delete_vector(obj);
}

TEST(vector_obj, push_val_ok) {
    unsigned char str[] = "value";
    struct vector* obj = init_vector();
    struct buffer* string = init_string(str);
    push(obj, string);
    ASSERT_EQ(obj->buf[0], string);
    delete_vector(obj);
}

TEST(vector_obj, resize_vector) {
    struct vector* buf = init_vector();
    resize(buf);
    ASSERT_EQ(buf->capacity, 2);
    delete_vector(buf);
}

TEST(main_process, count_bytes_from_data) {
    unsigned char str1[] = "aaabbbccc";
    unsigned char str2[] = "aaabbb";
    unsigned char str3[] = "aaa";
    struct vector* data = init_vector();
    push(data, init_string(str1));
    push(data, init_string(str2));
    push(data, init_string(str3));

    ASSERT_EQ(count_difference_bytes(data, 0), 12);
    ASSERT_EQ(count_difference_bytes(data, 1), 3);

    delete_vector(data);
}

TEST(file, open_ok) {
    char* filepath = getenv("TEST_FILE");
    FILE* fh = open_file(filepath);
    ASSERT_TRUE(fh);
    close_file(fh);
}

TEST(file, read_ok) {
    char* filepath = getenv("TEST_FILE");
    ASSERT_TRUE(filepath != nullptr);
    unsigned char str1[] = "aaabbccc";
    struct buffer* string = init_string(str1);

    FILE* fh = open_file(filepath);
    struct vector* data = read_data_from_file(fh);
    ASSERT_EQ(*data->buf[0]->buf, *str1);
    close_file(fh);

    delete_vector(data);
    clear_string(string);
}

TEST(file, process_ok) {
    char* filepath = getenv("TEST_FILE");
    ASSERT_TRUE(filepath != nullptr);

    struct data* result = process(filepath);
    ASSERT_EQ(result->diff[0], 12);
    ASSERT_EQ(result->diff[1], 3);
    clear(result);
}