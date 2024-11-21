/*
 * By elPytel
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    INT,
    FLOAT,
    CHAR
} DATA_TYPE;

/* Struktura datoveho bloku zpravy
 * Attributes:
 * - data_type - typ dat
 * - data_size - velikost dat
 * - data* - ukazatel na data
 */
typedef struct {
    DATA_TYPE data_type;
    uint16_t data_size;
    void* data;
} Data_block;

typedef enum {
    LOGING_INFO,
    LOGING_WARNING,
    LOGING_ERROR
} MSG_TYPE;

/* Struktura zpravy
 * Attributes:
 * - number_of_bytes - velikost zpravy
 * - number_of_sections - pocet polozek zpravy
 * - msg_type - typ zpravy
 * - data_block** - ukazatel na pole datovych bloku
 */
typedef struct {
    uint32_t number_of_bytes;
    uint8_t number_of_sections;
    MSG_TYPE msg_type;
    Data_block** data_block;
} Message;

// Declare the functions
int* Int(int value);
float* Float(float value);

Data_block* create_data_block(DATA_TYPE data_type, uint16_t data_size, void* data);
int fill_data_block(Data_block* data_block, DATA_TYPE data_type, uint16_t data_size, void* data);
void free_data_block(Data_block* data_block);

size_t data_type_size(DATA_TYPE data_type);

size_t calculate_number_of_bytes(uint8_t number_of_sections, Data_block** data_blocks);
Message* create_message(uint8_t number_of_sections, MSG_TYPE msg_type, Data_block** data_blocks);

void free_message(Message* msg);
bool compare_messages(Message* msg1, Message* msg2);

Message* create_text_message(MSG_TYPE text_msg_type, char* text);

void serialize_message(Message* msg, uint8_t* buffer);

Message* deserialize_message(uint8_t* buffer);