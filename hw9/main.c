#include "loglib.c"

int main() {
    logger_initFileLogger("log.txt");

    log_message(DEBUG, "This is a debug message\n");
    log_message(INFO, "This is an info message\n");
    log_message(WARNING, "This is a warning message\n");
    log_message(ERROR, "This is an error message");
    return 0;
}