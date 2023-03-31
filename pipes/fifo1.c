#include <stdio.h>
#include <string.h>
#include <fcntl.h> // Определение констант
#include <sys/stat.h> // определяет структуру данных
#include <sys/types.h> // Определяет типы и структуру
#include <unistd.h> // Определяет различные символьные константы и типы

int main()
{
    int fd;

    // FIFO файл PATH
    char *myfifo = "/tmp/myfifo";

    // Создание именнованного пайпа (FIFO)
    // mkfifo(<pathname>, <permission>)
    mkfifo(myfifo, 0656);

    char arr1[80], arr2[80];
    while (1)
    {
        // Открытие FIFO для записи
        fd = open(myfifo, O_WRONLY);

        // Получаем строчку данных arr2 от пользователя.
        // Максимальная длина 80!!!
        fgets(arr2, 80, stdin);

        /* Запись входа arr2 на FIFO
        и закрытие*/
        write(fd, arr2, strlen(arr2)+1);
        close(fd);

        // Открытие FIFO для чтения
        fd = open(myfifo, O_RDONLY);

        // Чтение из FIFO
        read(fd, arr1, sizeof(arr1));

        // Печать прочитанного сообщения
        printf("User2: %s\n", arr1);
        close(fd);
    }
    return 0;
}