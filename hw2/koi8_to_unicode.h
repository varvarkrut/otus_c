#include <stdint.h>
#include "stdio.h"

typedef struct {
    uint8_t koi8; // Код символа в кодировке KOI8
    uint16_t unicode; // Код символа в Unicode
} Koi8ToUnicode;

static const Koi8ToUnicode koi8Table[] = {
        {0xC0, 0x044E},  // ю
        {0xC1, 0x0430},  // а
        {0xC2, 0x0431},  // б
        {0xC3, 0x0446},  // ц
        {0xC4, 0x0434},  // д
        {0xC5, 0x0435},  // е
        {0xC6, 0x0444},  // ф
        {0xC7, 0x0433},  // г
        {0xC8, 0x0445},  // х
        {0xC9, 0x0438},  // и
        {0xCA, 0x0439},  // й
        {0xCB, 0x043A},  // к
        {0xCC, 0x043B},  // л
        {0xCD, 0x043C},  // м
        {0xCE, 0x043D},  // н
        {0xCF, 0x043E},  // о
        {0xD0, 0x043F},  // п
        {0xD1, 0x044F},  // я
        {0xD2, 0x0440},  // р
        {0xD3, 0x0441},  // с
        {0xD4, 0x0442},  // т
        {0xD5, 0x0443},  // у
        {0xD6, 0x0436},  // ж
        {0xD7, 0x0432},  // в
        {0xD8, 0x044C},  // ь
        {0xD9, 0x044B},  // ы
        {0xDA, 0x0437},  // з
        {0xDB, 0x0448},  // ш
        {0xDC, 0x044D},  // э
        {0xDD, 0x0449},  // щ
        {0xDE, 0x0447},  // ч
        {0xDF, 0x044A},  // ъ
        {0xE0, 0x042E},  // Ю
        {0xE1, 0x0410},  // А
        {0xE2, 0x0411},  // Б
        {0xE3, 0x0426},  // Ц
        {0xE4, 0x0414},  // Д
        {0xE5, 0x0415},  // Е
        {0xE6, 0x0424},  // Ф
        {0xE7, 0x0413},  // Г
        {0xE8, 0x0425},  // Х
        {0xE9, 0x0418},  // И
        {0xEA, 0x0419},  // Й
        {0xEB, 0x041A},  // К
        {0xEC, 0x041B},  // Л
        {0xED, 0x041C},  // М
        {0xEE, 0x041D},  // Н
        {0xEF, 0x041E},  // О
        {0xF0, 0x041F},  // П
        {0xF1, 0x042F},  // Я
        {0xF2, 0x0420},  // Р
        {0xF3, 0x0421},  // С
        {0xF4, 0x0422},  // Т
        {0xF5, 0x0423},  // У
        {0xF6, 0x0416},  // Ж
        {0xF7, 0x0412},  // В
        {0xF8, 0x042C},  // Ь
        {0xF9, 0x042B},  // Ы
        {0xFA, 0x0417},  // З
        {0xFB, 0x0428},  // Ш
        {0xFC, 0x042D},  // Э
        {0xFD, 0x0429},  // Щ
        {0xFE, 0x0427},  // Ч
        {0xFF, 0x042A},
};

uint16_t koi_8_to_unicode(uint8_t koi8) {
    int i, tableSize = sizeof(koi8Table) / sizeof(Koi8ToUnicode);
    for (i = 0; i < tableSize; i++) {
        if (koi8Table[i].koi8 == koi8) {
            printf("HERE %hu\n", koi8Table[i].unicode);
            return koi8Table[i].unicode;
        }
    }
    return 0; // Возвращаем 0, если символ не найден в таблице
}
