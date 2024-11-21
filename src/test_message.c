#include <assert.h>
#include <stdio.h>
#include "message.h"
#include "tools.c"

Data_block ** prepare_data_blocks(uint8_t number_of_sections, DATA_TYPE data_type[], uint16_t data_size[], void* data[]) {
    Data_block **data_blocks = (Data_block**)malloc(number_of_sections * sizeof(Data_block*));
    for (uint8_t i = 0; i < number_of_sections; i++) {
        data_blocks[i] = create_data_block(data_type[i], data_size[i], data[i]);
    }
    return data_blocks;
}

void test_create_message() {
    // prepare data
    uint8_t number_of_sections = 2;
    MSG_TYPE msg_type = LOGING_INFO;
    DATA_TYPE data_type[] = {INT, FLOAT};
    uint16_t data_size[] = {1, 1};
    // data as pointers
    int* int_data = Int(5);
    float* float_data = Float(3.14);

    // create data blocks
    Data_block **data_blocks = (Data_block**)malloc(number_of_sections * sizeof(Data_block*));
    for (uint8_t i = 0; i < number_of_sections; i++) {
        void *data;
        switch (data_type[i]) {
            case INT:
                data = int_data;
                break;
            case FLOAT:
                data = float_data;
                break;
            default:
                data = NULL;
                break;
        }
        data_blocks[i] = create_data_block(data_type[i], data_size[i], data);
    }
    
    Message* msg = create_message(number_of_sections, msg_type, data_blocks);

    // testing
    assert(msg->number_of_sections == number_of_sections);
    assert(msg->msg_type == msg_type);

    // free memory
    free_message(msg);
}

void test_create_text_message() {
    // prepare data
    MSG_TYPE msg_type = LOGING_INFO;
    // data as pointers
    char* text = "Hello world!";
    char* data = (char*)malloc(strlen(text) * sizeof(char));
    strcpy(data, text);

    Message* msg = create_text_message(msg_type, data);

    // free memory
    free_message(msg);
}

// serializace and check size
void test_serialize_message() {
    // prepare data
    uint8_t number_of_sections = 2;
    MSG_TYPE msg_type = LOGING_INFO;
    DATA_TYPE data_type[] = {INT, FLOAT};
    uint16_t data_size[] = {1, 1};
    // data as pointers
    int* int_data = Int(5);
    float* float_data = Float(3.14);
    void* data[] = {int_data, float_data};

    // create data blocks
    Data_block **data_blocks = prepare_data_blocks(number_of_sections, data_type, data_size, data);

    // create message
    Message* msg = create_message(number_of_sections, msg_type, data_blocks);

    // serialize message
    uint8_t* buffer = (uint8_t*)malloc(msg->number_of_bytes);
    serialize_message(msg, buffer);

    // free memory
    free(buffer);
    free_message(msg);
}

// serialize and deserialize
void test_serialize_deserialize_message() {
    // prepare data
    uint8_t number_of_sections = 2;
    MSG_TYPE msg_type = LOGING_INFO;
    DATA_TYPE data_type[] = {INT, FLOAT};
    uint16_t data_size[] = {1, 1};
    // data as pointers
    int* int_data = Int(5);
    float* float_data = Float(3.14);
    void* data[] = {int_data, float_data};

    // create data blocks
    Data_block **data_blocks = prepare_data_blocks(number_of_sections, data_type, data_size, data);

    // create message
    Message* msg = create_message(number_of_sections, msg_type, data_blocks);

    // serialize message
    uint8_t* buffer = (uint8_t*)malloc(msg->number_of_bytes);
    serialize_message(msg, buffer);

    // deserialize message
    Message* deserialized_msg = deserialize_message(buffer);

    // testing
    assert(compare_messages(msg, deserialized_msg));

    // free memory
    free(buffer);
    free_message(msg);
    free_message(deserialized_msg);
}

int main() {
    ///*
    test_create_message();
    test_create_text_message();
    //*/
    test_serialize_message();
    test_serialize_deserialize_message();
    printf("All tests of message passed!\n");
    return 0;
}