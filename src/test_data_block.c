#include <assert.h>
#include <stdio.h>

#include "message.h"
#include "tools.c"

void test_create_data_block_with_zero_data() {
    // prepare data
    DATA_TYPE data_type = CHAR;
    uint16_t data_size = 0;

    // create data block
    Data_block* data_block = create_data_block(data_type, data_size, NULL);

    // testing
    assert(data_block->data_type == data_type);
    assert(data_block->data_size == data_size);
    assert(data_block->data == NULL);

    // free memory
    free_data_block(data_block);
}

void test_create_data_block_with_one_data() {
    // prepare data
    DATA_TYPE data_type = INT;
    uint16_t data_size = 1;
    // data - pointer to int
    int* data = Int(5);

    // create data block
    Data_block* data_block = create_data_block(data_type, data_size, data);

    // testing
    assert(data_block->data_type == data_type);
    assert(data_block->data_size == data_size);
    assert(*(int*)data_block->data == *data);

    // free memory
    free_data_block(data_block);
}

void test_create_data_block_with_array_data() {
    // prepare data
    DATA_TYPE data_type = INT;
    uint16_t data_size = 3;
    // data - pointer to int
    int* data = (int*)malloc(data_size * sizeof(int));
    for (uint16_t i = 0; i < data_size; i++) {
        data[i] = i * 5;
    }

    // create data block
    Data_block* data_block = create_data_block(data_type, data_size, data);

    // testing
    assert(data_block->data_type == data_type);
    assert(data_block->data_size == data_size);
    assert((int*)data_block->data == data);

    // free memory
    free_data_block(data_block);
}

void test_create_array_of_data_blocks() {
    // prepare data
    uint8_t number_of_sections = 3;
    DATA_TYPE data_type[] = {INT, FLOAT, CHAR};
    uint16_t data_size[] = {1, 1, 1};

    // data block with NULL data
    Data_block** data_blocks = (Data_block**)malloc(number_of_sections * sizeof(Data_block*));
    for (uint8_t i = 0; i < number_of_sections; i++) {
        Data_block* data_block = create_data_block(data_type[i], data_size[i], NULL);
        data_blocks[i] = data_block;
    }

    // testing
    for (uint8_t i = 0; i < number_of_sections; i++) {
        assert(data_blocks[i]->data_type == data_type[i]);
        assert(data_blocks[i]->data_size == data_size[i]);
        assert(data_blocks[i]->data == NULL);
    }

    // free memory
    for (uint8_t i = 0; i < number_of_sections; i++) {
        free_data_block(data_blocks[i]);
    }
    free(data_blocks);
}

/*
typedef struct {
    uint32_t number_of_bytes;
    uint8_t number_of_sections;
    MSG_TYPE msg_type;
    Data_block* data_block;
} Message;
*/
void test_calculate_number_of_bytes() {
    // prepare data
    uint8_t number_of_sections = 3;
    DATA_TYPE data_type[] = {INT, FLOAT, CHAR};
    uint16_t data_size[] = {1, 1, 1};

    // data block with NULL data
    Data_block** data_blocks = (Data_block**)malloc(number_of_sections * sizeof(Data_block*));
    for (uint8_t i = 0; i < number_of_sections; i++) {
        Data_block* data_block = create_data_block(data_type[i], data_size[i], NULL);
        data_blocks[i] = data_block;
    }

    size_t number_of_bytes = calculate_number_of_bytes(number_of_sections, data_blocks);

    // testing
    size_t expected_number_of_bytes = sizeof(uint32_t) + sizeof(uint8_t) + sizeof(MSG_TYPE) +
                                      3 * (sizeof(DATA_TYPE) + sizeof(uint16_t)) + sizeof(int) +
                                      sizeof(float) + sizeof(char);
    assert(number_of_bytes == expected_number_of_bytes);

    // free memory
    for (uint8_t i = 0; i < number_of_sections; i++) {
        free_data_block(data_blocks[i]);
    }
    free(data_blocks);
}

int main() {
    test_create_data_block_with_zero_data();
    test_create_data_block_with_one_data();
    test_create_data_block_with_array_data();
    test_create_array_of_data_blocks();
    test_calculate_number_of_bytes();
    printf("All tests of data block passed!\n");
    return 0;
}