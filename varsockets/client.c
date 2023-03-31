#include <stdio.h>	// Используется printf
#include <string.h>	// Используется strlen
#include <sys/socket.h>	// Описание socket
#include <arpa/inet.h>	// Использование inet_addr
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>

int main(int argc , char *argv[])
{
	int sock;
	struct sockaddr_in server;
	char message[1000] , server_reply[2000];
	
	// Создание сокета
	sock = socket(AF_INET , SOCK_STREAM , 0);
	if (sock == -1)
	{
		printf("Невозможно создать сокет.");
	}
	puts("Сокет создан.");
	
	server.sin_addr.s_addr = inet_addr("127.0.0.1"); // IP-address
	server.sin_family = AF_INET;
	server.sin_port = htons(8885); // PORT

	// Подключение к удаленному серверу
	if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		perror("Соединение провалено.");
		return 1;
	}
	
	puts("Подсоединен\n");
	
	// keep communicating with server
	while(1)
	{
		printf("Введите сообщение: ");
		scanf("%s" , message);
		
		// Отправка каких-то данных
		if( send(sock, message, strlen(message), 0) < 0)
		{
			puts("Отправка провалена.");
			return 1;
		}
		
		// Получить реакцию от сервера
		if( recv(sock, server_reply, 2000, 0) < 0)
		{
			puts("Нет ответа.");
			break;
		}
		
		puts("Реакция сервера:");
		puts(server_reply);
	}
	
	close(sock);
	return 0;
}