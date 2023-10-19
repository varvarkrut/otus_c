#include <stdio.h>
#include <stdarg.h>
#include <execinfo.h>
#include <stdlib.h>


typedef enum {
    DEBUG,
    INFO,
    WARNING,
    ERROR
} LogLevel;

static LogLevel current_level;
static FILE* log_file;

void printStackTrace(int to_log_file);


FILE* logger_initConsoleLogger(){
    FILE* file = fdopen(1, "w");
    if (file) {
        fprintf(file, "Start console logging\n");
    }
    return file;
}


int logger_initFileLogger(const char* filename){
    if (filename == NULL){
        log_file = logger_initConsoleLogger();
    } else{
        log_file = fopen(filename, "a");
    }
    current_level = INFO;

    if (!log_file) {
        printf("Error opening log file\n");
        return -1;
    }
    return 0;
}


void logger_setLevel(LogLevel level){
    current_level = level;
}


LogLevel logger_getLevel(void){
    return current_level;
}


void log_message(LogLevel level, const char* format, ...) {
    if (!log_file) {
        printf("Logger is not initialized\n");
        return;
    }

    va_list args;
    va_start(args, format);

    fprintf(log_file, "[%s:%d] ", __FILE__, __LINE__); // добавляем информацию о месте вызова

    switch (level) {
        case DEBUG:
            fprintf(log_file, "[DEBUG]: ");
            vfprintf(log_file, format, args);
            break;
        case INFO:
            fprintf(log_file, "[INFO]: ");
            vfprintf(log_file, format, args);
            break;
        case WARNING:
            fprintf(log_file, "[WARNING]: ");
            vfprintf(log_file, format, args);
            break;
        case ERROR:
            fprintf(log_file, "[ERROR]: ");
            vfprintf(log_file, format, args);
            printStackTrace(1);
            break;
        default:
            fprintf(log_file, "[UNKNOWN LEVEL]: ");
            vfprintf(log_file, format, args);
            break;
    }
}



void printStackTrace(int to_log_file) {
    void* stack[20];
    int stackSize;
    char** stackSymbols;

    stackSize = backtrace(stack, 20);
    stackSymbols = backtrace_symbols(stack, stackSize);

    if (stackSymbols == NULL) {
        perror("backtrace_symbols");
        return;
    }

    for (int i = 0; i < stackSize; i++) {
        if (to_log_file == 1) {
            fprintf(log_file, "%s\n", stackSymbols[i]);
        } else {
            printf("%s\n", stackSymbols[i]);
        }
    }

    free(stackSymbols);
}

