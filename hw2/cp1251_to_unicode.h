#include <stdint.h>

typedef struct {
    uint8_t cp1251;
    uint16_t unicode;
} Cp1251ToUnicode;

const Cp1251ToUnicode cp1251Table[] = {
        {0x80, 0x0402}, {0x81, 0x0403}, {0x82, 0x201A}, {0x83, 0x0453},
        {0x84, 0x201E}, {0x85, 0x2026}, {0x86, 0x2020}, {0x87, 0x2021},
        {0x88, 0x20AC}, {0x89, 0x2030}, {0x8A, 0x0409}, {0x8B, 0x2039},
        {0x8C, 0x040A}, {0x8D, 0x040C}, {0x8E, 0x040B}, {0x8F, 0x040F},
        {0x90, 0x0452}, {0x91, 0x2018}, {0x92, 0x2019}, {0x93, 0x201C},
        {0x94, 0x201D}, {0x95, 0x2022}, {0x96, 0x2013}, {0x97, 0x2014},
        {0x98, 0x2122}, {0x99, 0x0459}, {0x9A, 0x203A}, {0x9B, 0x045A},
        {0x9C, 0x045C}, {0x9D, 0x045B}, {0x9E, 0x045F}, {0x9F, 0x00A0},
        {0xA0, 0x040E}, {0xA1, 0x045E}, {0xA2, 0x0408}, {0xA3, 0x00A3},
        {0xA4, 0x00A4}, {0xA5, 0x00A5}, {0xA6, 0x00A6}, {0xA7, 0x00A7},
        {0xA8, 0x00A8}, {0xA9, 0x00A9}, {0xAA, 0x0401}, {0xAB, 0x00AB},
        {0xAC, 0x00AC}, {0xAD, 0x00AD}, {0xAE, 0x00AE}, {0xAF, 0x0407},
        {0xB0, 0x00B0}, {0xB1, 0x00B1}, {0xB2, 0x0406}, {0xB3, 0x0456},
        {0xB4, 0x0491}, {0xB5, 0x00B5}, {0xB6, 0x00B6}, {0xB7, 0x00B7},
        {0xB8, 0x0451}, {0xB9, 0x2116}, {0xBA, 0x0454}, {0xBB, 0x00BB},
        {0xBC, 0x0458}, {0xBD, 0x0405}, {0xBE, 0x0455}, {0xBF, 0x0457},
        {0xC0, 0x0410}, {0xC1, 0x0411}, {0xC2, 0x0412}, {0xC3, 0x0413},
        {0xC4, 0x0414}, {0xC5, 0x0415}, {0xC6, 0x0416}, {0xC7, 0x0417},
        {0xC8, 0x0418}, {0xC9, 0x0419}, {0xCA, 0x041A}, {0xCB, 0x041B},
        {0xCC, 0x041C}, {0xCD, 0x041D}, {0xCE, 0x041E}, {0xCF, 0x041F},
        {0xD0, 0x0420}, {0xD1, 0x0421}, {0xD2, 0x0422},  {0xD3, 0x0423}, {0xD4, 0x0424}, {0xD5, 0x0425}, {0xD6, 0x0426},
        {0xD7, 0x0427}, {0xD8, 0x0428}, {0xD9, 0x0429}, {0xDA, 0x042A},
        {0xDB, 0x042B}, {0xDC, 0x042C}, {0xDD, 0x042D}, {0xDE, 0x042E},
        {0xDF, 0x042F}, {0xE0, 0x0430}, {0xE1, 0x0431}, {0xE2, 0x0432},
        {0xE3, 0x0433}, {0xE4, 0x0434}, {0xE5, 0x0435}, {0xE6, 0x0436},
        {0xE7, 0x0437}, {0xE8, 0x0438}, {0xE9, 0x0439}, {0xEA, 0x043A},
        {0xEB, 0x043B}, {0xEC, 0x043C}, {0xED, 0x043D}, {0xEE, 0x043E},
        {0xEF, 0x043F}, {0xF0, 0x0440}, {0xF1, 0x0441}, {0xF2, 0x0442},
        {0xF3, 0x0443}, {0xF4, 0x0444}, {0xF5, 0x0445}, {0xF6, 0x0446},
        {0xF7, 0x0447}, {0xF8, 0x0448}, {0xF9, 0x0449}, {0xFA, 0x044A},
        {0xFB, 0x044B}, {0xFC, 0x044C}, {0xFD, 0x044D}, {0xFE, 0x044E},
        {0xFF, 0x044F}};

uint16_t cp1251_to_unicode(uint8_t cp1251) {
    int i, tableSize = sizeof(cp1251Table) / sizeof(Cp1251ToUnicode);
    for (i = 0; i < tableSize; i++) {
        if (cp1251Table[i].cp1251 == cp1251) {
            return cp1251Table[i].unicode;
        }
    }
    return 0; // Возвращаем 0, если символ не найден в таблице
}

