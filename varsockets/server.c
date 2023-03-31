#include <stdio.h>
#include <string.h>	// Используется strlen()
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include <netinet/in.h>

int main(int argc , char *argv[])
{
	int socket_desc, client_sock, c, read_size;
	struct sockaddr_in server, client; // Создание структуры интернет сокетов клиент и сервер
	char client_message[2000]; // Сообщение от клиента 2000 символов
	
	socket_desc = socket(AF_INET , SOCK_STREAM, 0); // Создание сокета
	
	
	if (socket_desc == -1)
	{
		printf("Невозможно создать сокет.");
	}
	puts("Сокет создан.");
	
	// Подготовка структуры sockaddr_in
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(8885);
	
	// Соединение
	if(bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
	{
		// Печать сообщения об ошибке
		perror("Ошибка соединения!");
		return 1;
	}
	puts("Подсоединение прошло успешно!");
	
	// Ожидание
	listen(socket_desc , 3);
	
	// Разрешение на входящие подключения
	puts("Ожидание входящих подключений...");
	c = sizeof(struct sockaddr_in);
	
	// Осуществление соединения из входящего клиента
	client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
	if (client_sock < 0)
	{
		perror("Не удалось подсоединить.");
		return 1;
	}
	puts("-Произошло подключение-");
	
	// Получение сообщения от клиента
	while((read_size = recv(client_sock, client_message, 2000, 0)) > 0)
	{
		// Отправка сообщения клиенту
		write(client_sock, client_message, strlen(client_message));
		// client_message[0] = 0;
		//memset(client_message, 0, strlen(client_message));
		//memset(&client, 0, sizeof(struct sockaddr_in));
	}
	//memset(client_message, 0, strlen(client_message));
	
	if(read_size == 0)
	{
		puts("Клиент отсоединился");
		fflush(stdout);
	}
	else if(read_size == -1)
	{
		perror("Ошибка получения.");
	}
	
	return 0;
}
