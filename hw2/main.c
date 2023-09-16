#include "cp1251_to_unicode.h"
#include "koi8_to_unicode.h"
#include "iso8859-5.h"
#include "stdio.h"
#include "string.h"

void convertUnicodeToUTF8(int unicodeChar, char utf8Char[4]) {
    if (unicodeChar <= 0x7F) {
        utf8Char[0] = unicodeChar & 0x7F; // ASCII character
        utf8Char[1] = '\0';
    } else if (unicodeChar <= 0x7FF) {
        utf8Char[0] = 0xC0 | ((unicodeChar >> 6) & 0x1F);
        utf8Char[1] = 0x80 | (unicodeChar & 0x3F);
        utf8Char[2] = '\0';
    } else if (unicodeChar <= 0xFFFF) {
        utf8Char[0] = 0xE0 | ((unicodeChar >> 12) & 0x0F);
        utf8Char[1] = 0x80 | ((unicodeChar >> 6) & 0x3F);
        utf8Char[2] = 0x80 | (unicodeChar & 0x3F);
        utf8Char[3] = '\0';
    }
}



int print_unicode(const char* filename_to_in_frm, const char* filename_to_out, const  char* encoding){
    uint16_t unicode_char;
    FILE *filein = fopen(filename_to_in_frm, "rb");
    FILE* fileout = fopen(filename_to_out, "w");

    // Проверяем, удалось ли открыть файл
    if (filein == NULL) {
        printf("Не удалось открыть файл на in.\n");
        return 1;
    }

    if (fileout == NULL) {
        printf("Не удалось открыть файл на out");
        return 1;
    }

    unsigned char buffer; // Объявляем буфер типа unsigned char для хранения байта

    while (fread(&buffer, sizeof(buffer), 1, filein) == 1) {
        if (buffer == ' ') {
            fprintf(fileout, " ");
        }
        if (buffer == '.') {
            fprintf(fileout, ".");
        }
        if (strcmp(encoding, "cp1251") == 0) {
            unicode_char = cp1251_to_unicode(buffer);
        }
        if (strcmp(encoding, "koi8") == 0) {
            unicode_char = koi_8_to_unicode(buffer);
        }
        if (strcmp(encoding, "iso-8859-5") == 0) {
            unicode_char = iso8859_5_to_unicode(buffer);
        }
        else {
            printf("Не указана ни одна из поддерживаемых кодировок:\n* cp1251\n* koi8\n* iso-8859-5\nЗавершаю работу ");
            return 1;
        }
        char utf8Char[4] = {'\0'};
        convertUnicodeToUTF8(unicode_char, utf8Char);
        fprintf(fileout, "%s", utf8Char);
    }

    fclose(filein);
    fclose(fileout);// Закрываем файл
    return 0;
}


int main(int argc, char* argv[]) {
    if (argc < 4) {
        printf("Usage: %s <filename to read from> <encoding>  <filename to out>\n", argv[0]);
        return 1;
    }
    char* filein = argv[1];
    char* encoding = argv[2];
    char* fileout = argv[3];
    print_unicode(filein, fileout, encoding);
    return 0;
}
