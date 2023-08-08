#include <stdio.h>
#include <stdlib.h>

typedef struct {
    uint32_t signature;
    uint16_t versionToExtract;
    uint16_t generalPurposeBitFlag;
    uint16_t compressionMethod;
    uint16_t modificationTime;
    uint16_t modificationDate;
    uint32_t crc32;
    uint32_t compressedSize;
    uint32_t uncompressedSize;
    uint16_t filenameLength;
    uint16_t extraFieldLength;
    uint8_t *filename;
    uint8_t *extraField;
} LocalFileHeader;

int count_signatures(const char* filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        printf("Ошибка открытия файла\n");
        return -1;
    }

    int count = 0;
    uint32_t sign = 0x04034b50;
    while (1) {
        LocalFileHeader lfd;
        size_t bytes_read = fread(&lfd.signature, sizeof(uint32_t), 1, file);
        if (bytes_read < 1) {
            break;
        }

        if (lfd.signature == sign) {
            fread(&lfd.versionToExtract, sizeof(uint16_t), 1, file);
            fread(&lfd.generalPurposeBitFlag, sizeof(uint16_t), 1, file);
            fread(&lfd.compressionMethod, sizeof(uint16_t), 1, file);
            fread(&lfd.modificationTime, sizeof(uint16_t), 1, file);
            fread(&lfd.modificationDate, sizeof(uint16_t), 1, file);
            fread(&lfd.crc32, sizeof(uint32_t), 1, file);
            fread(&lfd.compressedSize, sizeof(uint32_t), 1, file);
            fread(&lfd.uncompressedSize, sizeof(uint32_t), 1, file);
            fread(&lfd.filenameLength, sizeof(uint16_t), 1, file);
            fread(&lfd.extraFieldLength, sizeof(uint16_t), 1, file);
            lfd.filename = malloc(lfd.filenameLength + 1);
            fread(lfd.filename, sizeof(uint8_t), lfd.filenameLength, file);
            lfd.filename[lfd.filenameLength] = '\0';
            printf("%s\n", lfd.filename);
            free(lfd.filename);

            count++;
        }
        else {
            fseek(file, -3, SEEK_CUR); // Возврат назад на 3 байта перед считанной сигнатурой
        }
    }

    fclose(file);
    return count;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    char* filename = argv[1];
    int count = count_signatures(filename);
    if (count == 0) {
        printf("Файл не является rarjpeg\n");
    }

    return 0;}
