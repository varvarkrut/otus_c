#include <stdio.h>
#include <string.h>
#include "hash_table.h"

int count_words(const char* filename_to_in_frm) {
    FILE *filein = fopen(filename_to_in_frm, "rb");
    if (filein == NULL) {
        printf("Failed to open file %s for input.\n", filename_to_in_frm);
        return 1;
    }
    char word[1024];
    char buffer;
    char value[1024] = "0\0";
    HashTable* hash_table = create_table(1024);

    while ((buffer = fgetc(filein)) != EOF) {
        if (buffer == ' ' || buffer == ',' || buffer == '.')
        // the end of the word
        {
            value[0] = get_value(hash_table, word);
            value[0]= (atoi(&value[0]) + 1) + '0';
            ht_insert(hash_table, word, value);
            memset(word, 0, sizeof(word));
        }
        else {
         // continuation of the word
            strncat(word, &buffer, sizeof (buffer));
        }
        }
    // the last word
    value[0] = get_value(hash_table, word);
    value[0]= (atoi(&value[0]) + 1) + '0';
    ht_insert(hash_table, word, value);

    print_values(hash_table);
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s <filename to read from> \n", argv[0]);
        return 1;
    }
    char* filein = argv[1];
    count_words(filein);
}
