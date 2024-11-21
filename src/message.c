/*
 * By elPytel
 */

#include "message.h"

Data_block* create_data_block(DATA_TYPE data_type, uint16_t data_size, void* data) {
    Data_block* data_block = (Data_block*)malloc(sizeof(Data_block));
    data_block->data_type = data_type;
    data_block->data_size = data_size;
    data_block->data = data;
    return data_block;
}

int fill_data_block(Data_block* data_block, DATA_TYPE data_type, uint16_t data_size, void* data) {
    if (data_block == NULL) {
        return -1;
    }
    data_block->data_type = data_type;
    data_block->data_size = data_size;
    data_block->data = data;
    return 0;
}

void free_data_block(Data_block* data_block) {
    if (data_block == NULL) {
        return;
    }
    if (data_block->data != NULL) {
        free(data_block->data);
        data_block->data = NULL;
    }
    free(data_block);
    data_block = NULL;
}

/* Calculate size of data type
 * Arguments:
 * - data_type - typ dat
 *
 * Return:
 * - size_t - velikost datoveho typu
 */
size_t data_type_size(DATA_TYPE data_type) {
    switch (data_type) {
        case INT:
            return sizeof(int);
        case FLOAT:
            return sizeof(float);
        case CHAR:
            return sizeof(char);
        default:
            return 0;
    }
}

/* Calculate number of bytes for message
 * Arguments:
 * - number_of_sections - pocet polozek zpravy
 * - data_size* - velikosti jednotlivych dat
 *
 * Return:
 * - size_t - velikost zpravy
 */
size_t calculate_number_of_bytes(uint8_t number_of_sections, Data_block** data_block) {
    size_t number_of_bytes = sizeof(uint32_t) + sizeof(uint8_t) + sizeof(MSG_TYPE);
    for (uint8_t i = 0; i < number_of_sections; i++) {
        size_t base_size = data_type_size(data_block[i]->data_type);
        size_t data_block_size = data_block[i]->data_size * base_size;
        number_of_bytes += sizeof(DATA_TYPE) + sizeof(uint16_t) + data_block_size;
    }
    return number_of_bytes;
}

/* Create message structure from arguments
 * Arguments:
 * - number_of_bytes - velikost zpravy
 * - number_of_sections - pocet polozek zpravy
 * - msg_type - typ zpravy
 * - data_block* - ukazatel na pole datovych bloku
 */
Message* create_message(uint8_t number_of_sections, MSG_TYPE msg_type, Data_block** data_block) {
    uint32_t number_of_bytes = calculate_number_of_bytes(number_of_sections, data_block);

    Message* msg = (Message*)malloc(sizeof(Message));
    msg->number_of_bytes = number_of_bytes;
    msg->number_of_sections = number_of_sections;
    msg->msg_type = msg_type;
    msg->data_block = data_block;
    return msg;
}

/* Free message structure
 * Arguments:
 * - msg* - zprava
 */
void free_message(Message* msg) {
    if (msg == NULL) {
        return;
    }
    for (uint8_t i = 0; i < msg->number_of_sections; i++) {
        free_data_block(msg->data_block[i]);
    }
    free(msg->data_block);
    free(msg);
    msg = NULL;
}

Message* create_text_message(MSG_TYPE text_msg_type, char* text) {
    uint8_t number_of_sections = 1;
    Data_block** data = (Data_block**)malloc(number_of_sections * sizeof(Data_block*));
    data[0] = create_data_block(CHAR, strlen(text), text);
    return create_message(number_of_sections, text_msg_type, data);
}

/* Compare two messages
 * Arguments:
 * - msg1* - zprava
 * - msg2* - zprava
 *
 * Return:
 * - bool - true, pokud jsou zpravy stejne, jinak false
 */
bool compare_messages(Message* msg1, Message* msg2) {
    // compare pointers
    if (msg1 == msg2) {
        return true;
    }
    if (msg1->number_of_sections != msg2->number_of_sections) {
        return false;
    }
    if (msg1->msg_type != msg2->msg_type) {
        return false;
    }
    for (uint8_t i = 0; i < msg1->number_of_sections; i++) {
        if (msg1->data_block[i]->data_type != msg2->data_block[i]->data_type) {
            return false;
        }
        if (msg1->data_block[i]->data_size != msg2->data_block[i]->data_size) {
            return false;
        }
        void* data1 = msg1->data_block[i]->data;
        void* data2 = msg2->data_block[i]->data;
        size_t data_size = msg1->data_block[i]->data_size;
        if (memcmp(data1, data2, data_size) != 0) {
            return false;
        }
    }
    return true;
}

/* serialize message to Byte array for sending over serial port
 * msg* - zprava
 * buffer* - buffer pro serializaci
 */
void serialize_message(Message* msg, uint8_t* buffer) {
    uint32_t offset = 0;
    // serialize number_of_bytes
    memcpy(buffer, &msg->number_of_bytes, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    // serialize number_of_sections
    memcpy(buffer + offset, &msg->number_of_sections, sizeof(uint8_t));
    offset += sizeof(uint8_t);
    // serialize msg_type
    memcpy(buffer + offset, &msg->msg_type, sizeof(MSG_TYPE));
    offset += sizeof(MSG_TYPE);

    // serialize data_block
    for (uint32_t i = 0; i < msg->number_of_sections; i++) {
        Data_block* data_block = msg->data_block[i];
        // data_type
        memcpy(buffer + offset, &data_block->data_type, sizeof(DATA_TYPE));
        offset += sizeof(DATA_TYPE);
        // data_size
        memcpy(buffer + offset, &data_block->data_size, sizeof(uint16_t));
        offset += sizeof(uint16_t);
        // data
        void* src = data_block->data;
        void* dst = buffer + offset;
        size_t size = data_block->data_size * data_type_size(data_block->data_type);
        memcpy(dst, src, size);
        offset += data_block->data_size * data_type_size(data_block->data_type);
    }
}

/* deserialize message from Byte array
 * - buffer* - buffer pro deserializaci
 *
 * Return:
 * - msg* - zprava
 */
Message* deserialize_message(uint8_t* buffer) {
    Message* msg = (Message*)malloc(sizeof(Message));
    uint32_t offset = 0;
    // deserialize number_of_bytes
    memcpy(&msg->number_of_bytes, buffer, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    // deserialize number_of_sections
    memcpy(&msg->number_of_sections, buffer + offset, sizeof(uint8_t));
    offset += sizeof(uint8_t);
    // deserialize msg_type
    memcpy(&msg->msg_type, buffer + offset, sizeof(MSG_TYPE));
    offset += sizeof(MSG_TYPE);

    // allocate memory for data_block
    msg->data_block = (Data_block**)malloc(msg->number_of_sections * sizeof(Data_block*));

    // deserialize data_type, data_size and data
    for (uint32_t i = 0; i < msg->number_of_sections; i++) {
        Data_block* data_block = (Data_block*)malloc(sizeof(Data_block));
        // data_type
        memcpy(&data_block->data_type, buffer + offset, sizeof(DATA_TYPE));
        offset += sizeof(DATA_TYPE);
        // data_size
        memcpy(&data_block->data_size, buffer + offset, sizeof(uint16_t));
        offset += sizeof(uint16_t);
        // data
        size_t memory_size = data_block->data_size * data_type_size(data_block->data_type);
        data_block->data = malloc(memory_size);
        memcpy(data_block->data, buffer + offset, memory_size);
        offset += data_block->data_size * data_type_size(data_block->data_type);

        // add data_block to message
        msg->data_block[i] = data_block;
    }
    return msg;
}