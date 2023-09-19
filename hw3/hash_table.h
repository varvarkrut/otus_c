#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

uint32_t hash(char* key) {
    uint32_t hash_value = 0;

    for (int i = 0; key[i] != '\0'; i++) {
        hash_value += key[i];
    }

    return hash_value % 1024;
}

typedef struct Ht_item Ht_item;

// Define the Hash Table Item here
struct Ht_item {
    char* key;
    char* value;
};


typedef struct HashTable HashTable;

// Define the Hash Table here
struct HashTable {
    // Contains an array of pointers
    // to items
    Ht_item** items;
    int size;
    int count;
};

unsigned long handle_collision(HashTable* table, unsigned long index){
    while (table->items[index]->key != NULL) {
        index = (index + 1) % table->size; // переход к следующему слоту (циклически)
    }
    return index;
}

Ht_item* create_item(char* key, char* value) {
    // Creates a pointer to a new hash table item
    Ht_item* item = (Ht_item*) malloc (sizeof(Ht_item));
    item->key = (char*) malloc (strlen(key) + 1);
    item->value = (char*) malloc (strlen(value) + 1);

    strcpy(item->key, key);
    strcpy(item->value, value);

    return item;
}

HashTable* create_table(int size) {
    // Creates a new HashTable
    HashTable* table = (HashTable*) malloc (sizeof(HashTable));
    table->size = size;
    table->count = 0;
    table->items = (Ht_item**) calloc (table->size, sizeof(Ht_item*));
    for (int i=0; i<table->size; i++)
        table->items[i] = NULL;
    return table;
}

char get_value(HashTable* table, char* key){
    char ans;
    uint32_t index = hash(key);
    Ht_item* current_item = table->items[index];
    if (current_item == NULL){
        return 0;
    }
    ans = *(current_item->value);
    return ans;

}
void ht_insert(HashTable* table, char* key, char* value) {
    // Create the item
    uint32_t index = hash(key);
    Ht_item *item = create_item(key, value);

    Ht_item *current_item = table->items[index];

    if (current_item == NULL) {
        // Key does not exist.
        if (table->count == table->size) {
            // Hash Table Full
            printf("Insert Error: Hash Table is full\n");
            return;
        }


        // Insert directly
        table->items[index] = item;
        table->count++;
    } else {
        // Scenario 1: We only need to update value
        if (strcmp(current_item->key, key) == 0) {
            strcpy(table->items[index]->value, value);
            return;
        } else {
            // Scenario 2: Collision
            // We will handle case this a bit later
            handle_collision(table, index);
            return;
        }
    }
}

void print_values(HashTable* table) {
    for (int i = 0; i < table->size; i++)
        if (table->items[i]) {
            printf("Строка %s встречается %s раз\n", table->items[i]->key, table->items[i]->value);

        }
}
