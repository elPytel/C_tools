# Zasílání zpráv mezi dvěma zařízeními

## Zpráva

Dělí se na datové bloky, které mohou obsahovat různé typy dat. Každý datový blok obsahuje typ dat, velikost dat a samotná data. 

```C
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
```

Zpráva obsahuje pole datových bloků, počet datových bloků, velikost zprávy a typ zprávy.

```C
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
```

## Přidání dalších typů zpráv

Pro přidání dalších typů zpráv je potřeba upravit výčtový typ `MSG_TYPE`, ostatní struktury zůstávají stejné. 

```C
typedef enum { 
    LOGING_INFO, 
    LOGING_WARNING, 
    LOGING_ERROR,
    INT_AND_FLOAT_ARRAY
} MSG_TYPE;
```

Podpůrné funkce také modou zůstat stejné.

Vytvoření zprávy obsahující jeden datový blok s typem `INT` a pole datových bloků s typem `FLOAT_ARRAY`.
```C
// pouzijeme:
// Message* create_message(uint8_t number_of_sections, MSG_TYPE msg_type, Data_block** data_blocks);

// inicializace zpravy
int number_of_sections = 2;
Data_block** data_blocks = (Data_block**)malloc(number_of_sections * sizeof(Data_block*));

// vytvoreni datoveho bloku s typem INT
// vsechna data jsou pristupovana pres ukazatele, aby slo dalta genericky od-alokovat
int* int_data_pointer = (int*)malloc(sizeof(int));
*int_data_pointer = 5;
Data_block* data_block_int = CreateDataBlock(INT, sizeof(int), int_data_pointer);

// vytvoreni datoveho bloku s typem FLOAT_ARRAY
size_t float_array_size = 5;
float* float_data_pointer = (float*)malloc(float_array_size * sizeof(float));
for (size_t i = 0; i < float_array_size; i++) {
    float_data_pointer[i] = i*3.14;
}
Data_block* data_block_float_array = CreateDataBlock(FLOAT_ARRAY, float_array_size * sizeof(float), float_data_pointer);

// ulozeni datovych bloku do pole
data_blocks[0] = data_block_int;
data_blocks[1] = data_block_float_array;

// Kompletni zprava
Message* message = CreateMessage(number_of_sections, INT_AND_FLOAT_ARRAY, data_blocks);
```

Toto vytvoří naší požadovanou zprávu. 

## Serializace zprávy
Pokud naší zprávu chceme odeslat, můžeme použít funkci pro serializaci zprávy.


### Generický buffer
```C
uint8_t* buffer[BUFFER_SIZE];
bool valid = serialize_message_prealloc(message, buffer, BUFFER_SIZE);
```

Funkce `serialize_message_prealloc` zkontroluje, zda je buffer dostatečně velký pro serializaci zprávy. Pokud ano, zprávu serializuje a vrátí `true`, jinak vrátí `false` a do bufferu se nic nezapíše.

### Alokace bufferu pro zprávu
```C
uint8_t* buffer = (uint8_t*)malloc(message->number_of_bytes);
serialize_message(message, buffer) 
```

Nyní máme zprávu serializovanou do pole bytů, které můžeme odeslat.
