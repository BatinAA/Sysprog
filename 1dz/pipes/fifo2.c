#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    int fd1;

    // FIFO файл PATH
    char *myfifo = "/tmp/myfifo";

    // Создание именнованного пайпа (FIFO)
    // mkfifo(<pathname>,<permission>)
    mkfifo(myfifo, 0666);

    char str1[80], str2[80];
    while (1)
    {
        // Сначала открываем на чтение и читаем
        fd1 = open(myfifo,O_RDONLY);
        read(fd1, str1, 80);

        // Печать прочитанной строки и закрытие
        printf("User1: %s\n", str1);
        close(fd1);

        // Теперь открываем в режиме записи и записываем
        // строчку, введенную пользователем.
        printf("Введите ответ - ");
        fd1 = open(myfifo,O_WRONLY);
        fgets(str2, 80, stdin);
        write(fd1, str2, strlen(str2)+1);
        close(fd1);
    }
    return 0;
}