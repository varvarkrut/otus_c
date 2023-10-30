#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <syslog.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include "ini.h"
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>


char* filename="";
#define NAME "daemon.socket"
const char* configFilePath = "/Users/ishkurko/daemon.conf";


int configHandler(void* user, const char* section, const char* name, const char* value) {
    // Обработка конфигурационных параметров
    if (strcmp(section, "Main") == 0) {
        if (strcmp(name, "filename") == 0) {
            filename = (char *) malloc(strlen(value) + 1); // Выделение памяти для копирования строки
            strcpy(filename, value); // Копирование строки
        }
    }
    return 0; // Возвращайте 0, чтобы остановить обработку конфигурации
}

void loadConfig(const char* configFilePath) {
    if (access(configFilePath, F_OK) != 0) {
        // если попали сюда, то конфига не существует и надо его сделать
        FILE* file = fopen(configFilePath, "w");
        openlog("sipmle_daemon", LOG_PID | LOG_CONS, LOG_USER); // Открыть журнал
        syslog(LOG_INFO, "Создали дефолтный конфиг"); // Записать лог-сообщение
        printf("Создали конфиг\n");
        closelog(); // Закрыть журнал
        char str[] = "[Main]\nfilename=please, fill the config ;";
        fprintf(file, "%s", str);
        fclose(file);
    }
    printf("Начинаем чтение\n");
    FILE* configFile = fopen(configFilePath, "r");
    if (configFile == NULL) {
        openlog("sipmle_daemon", LOG_PID | LOG_CONS, LOG_USER); // Открыть журнал
        syslog(LOG_INFO, "Поврежден конфиг-файл, не удалось прочитать"); // Записать лог-сообщение
        closelog(); // Закрыть журнал
    }

    if (ini_parse_file(configFile, configHandler, NULL) < 0) {
        printf("Ошибка при чтении конфиг-файла, проверьте корректность конфиг-файла\n");
        openlog("sipmle_daemon", LOG_PID | LOG_CONS, LOG_USER); // Открыть журнал
        syslog(LOG_INFO, "Ошибка при чтении конфиг-файла, проверьте корректность конфиг-файла"); // Записать лог-сообщение
        closelog(); // Закрыть журнал
    }
    printf("Спарсили конфиг\n");
    fclose(configFile);
}

void removeEscaping(char* str) {
    int k = 0;
    int counter = 0;
    int len = strlen(str); // Хранение длины строки

    for (int i = 0; i < len; ++i) {
        if (str[i] != '\\' && str[i] != '"') {
            str[k++] = str[i];
            counter++;
        }
    }
    str[counter + 1] = '\0';
    str[counter] = '\0';
}

long int get_fsz(const char* filename) {
    removeEscaping(filename);
    FILE* file = fopen(filename, "rb"); // Открываем файл в режиме чтения бинарного файла
    if (file == NULL) {
        // Обработка ошибки открытия файла
        perror("File opening failed");
        return -1;
    }
    fseek(file, 0, SEEK_END); // Перемещаем указатель в конец файла
    long int size = ftell(file); // Получаем текущую позицию указателя
    fclose(file); // Закрываем файл
    return size;
}


void get_fsz_and_add_to_buf(char* buf){
    long int fsz = get_fsz(filename);
    if (fsz == -1){
        strcat(buf, "Файла не существует или доступ к нему запрещен");
    }
    else{
        sprintf(buf,"%ld", fsz);
    }
}

void serve_forever(){
    int sock, msgsock;
    struct sockaddr_un server;
    char buf[1024];
    sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("opening stream socket");
        exit(1);
    }
    server.sun_family = AF_UNIX;
    strcpy(server.sun_path, NAME);
    if (bind(sock, (struct sockaddr *) &server, sizeof(struct sockaddr_un))) {
        perror("binding stream socket");
        exit(1);
    }
    printf("Socket has name %s\n", server.sun_path);
    listen(sock, 5);
    for (;;) {
        loadConfig(configFilePath);
        msgsock = accept(sock, 0, 0);
        printf("Приняли входящий запрос клиента\n");
        if (msgsock == -1)
        {
            perror("accept");
            break;
        }

        else{
            get_fsz_and_add_to_buf(buf);
            printf("Отправляем  ответ, %s\n", buf);
            send(msgsock, buf, strlen(buf) + 1, 0);
            memset(buf, 0, sizeof buf);
        }
        close(msgsock);
    }
    close(sock);
    unlink(NAME);
}

static void sigterm (int signo) {
    if (remove("/usr/local/bin/my_daemon/daemon.socket") == 0) {

    } else {
        perror("Во время завершения работы не получилось удалить сокет ");
    }
    exit (EXIT_SUCCESS);
}

void daemonize(const char* cmd)
{
    int i, fd0, fd1, fd2;
    pid_t pid;
    struct rlimit rl;
    struct sigaction sa;
/*
* Инициализировать файл журнала.
*/
    openlog(cmd, LOG_CONS, LOG_DAEMON);
/*
* Сбросить маску режима создания файла.
*/
    umask(0);
/*
* Получить максимально возможный номер дескриптора файла.
*/
    if (getrlimit(RLIMIT_NOFILE, &rl) < 0)
        perror("невозможно получить максимальный номер дескриптора");
/*
* Стать лидером нового сеанса, чтобы утратить управляющий терминал.
*/
    if ((pid = fork()) < 0)
        perror("ошибка вызова функции fork");
    else if (pid != 0) /* родительский процесс */
        exit(0);
    setsid();
/*
* Обеспечить невозможность обретения управляющего терминала в будущем.
*/
    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGHUP, &sa, NULL) < 0)
        syslog(LOG_CRIT, "невозможно игнорировать сигнал SIGHUP");
    if ((pid = fork()) < 0)
        syslog(LOG_CRIT, "ошибка вызова функции fork");
    else if (pid != 0) /* родительский процесс */
        exit(0);
/*
* Назначить корневой каталог текущим рабочим каталогом,
* чтобы впоследствии можно было отмонтировать файловую систему.
*/
    if (chdir("/") < 0)
        syslog(LOG_CRIT, "невозможно сделать текущим рабочим каталогом /");
/*
* Закрыть все открытые файловые дескрипторы.
*/
    if (rl.rlim_max == RLIM_INFINITY)
        rl.rlim_max = 1024;
    for (i = 0; i < rl.rlim_max; i++)
        close(i);
/*
* Присоединить файловые дескрипторы 0, 1 и 2 к /dev/null.
*/
    fd0 = open("/dev/null", O_RDWR);
    fd1 = dup(0);
    fd2 = dup(0);
    if (fd0 != 0 || fd1 != 1 || fd2 != 2)
        syslog(LOG_CRIT, "ошибочные файловые дескрипторы %d %d %d",
               fd0, fd1, fd2);

    if(setuid(0) < 0) {
        perror("Ошибка установки UID");
        exit(EXIT_FAILURE);
    }

    if(setgid(0) < 0) {
        perror("Ошибка установки GID");
        exit(EXIT_FAILURE);
    }

    while (1) {
        if (chdir("/usr/local/bin/my_daemon") == -1) {
            perror("chdir");
        }
        if (signal (SIGTERM, sigterm) == SIG_ERR) {
            perror("Сломались при обработке SIGTERM");
            exit (EXIT_FAILURE);
        }
        serve_forever();
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s <True> or <False>", argv[0]);
        return 1;
    }

    char* daemonize_bool = argv[1];
    if (strcmp(daemonize_bool, "-True") == 0)
    {
        daemonize("my_daemon");
    }
    if (strcmp(daemonize_bool, "False") == 0) {
        serve_forever();
    }
    else{
        printf("Usage: %s <True> or <False> \n", argv[0]);
        return 1;
    }
}

