#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define SERVER_PORT 8080

void processing_message(unsigned char *s, int n) {
    printf("processing message...\n");
    unsigned char *p;
    for (p=s; p < s + n; p++)
    {
        if (islower(*p))
        {
            *p += 13;
            if(*p > 'z') *p -= 26;
        }
    }
}  

void processing_message_service(int in, int out) {
    printf("processing message service...\n");
    printf("fd is: %f\n", in);
    unsigned char buf[1024]; // Pre-allocated buffer
    int count;

    // ssize_t recv(int sockfd, void *buf, size_t len, int flags);
    // ssize_t read(int fd, void *buf, size_t count);
    //count = recv(in, &buf, 1024,0);
    //printf("%p\n",count);
    //if (count <0)  {perror("HH_ERROR: recv() failed");}
    //while ((count = recv(in, buf, 1024,0)) > 0)

    while  ((count = read(in, buf, 1024)) > 0) {
        printf("%p\n",count);
        printf("reading...\n");
        processing_message(buf, count);
        //send(out, buf, count, 0);
        write(out, buf, count);
    }

}  


int main() {

    printf("HH: TCP SERVER is starting\n");
    // we will use this rc variable to test if the
    // function calls were successfull
    int rc;

    char server_message[256] = "HH: You have reached the server!\n";

    // create the server socket
    int server_socket;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (server_socket == -1) {
        perror("HH_ERROR: error in calling socket()");
        exit(1);
    };

    // define the server address
    // htons means host to network short and is related to network byte order
    // htonl host to network long
    struct  sockaddr_in server_address;
    struct  sockaddr_in client_address;

    server_address.sin_family = AF_INET;           // IPv4
    server_address.sin_port = htons(SERVER_PORT);
    // port is 16 bit so htons is enough.
    //htons returns host_uint16 converted to network byte order
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    // returns host_uint32 converted to network byte order

    // bind the socket to our specified IP and port
    // int bind(int sockfd, const struct  sockaddr *addr, socklen_t addrlen);
    // sockfd argument is a file descriptor obtained from a previous call to socket()

    // addr agrument is a pointer to a struckture specifying the address to which this socket is to be bound

    rc = bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address));

    // bind should return 0;
    if (rc < 0)
    {
        perror("HH_ERROR: bind() call failed");
        exit(1);
    }

    // second agrument is a backlog - how many connections can be waiting for this socket simultaneously
    rc = listen(server_socket, 5);

    if (rc < 0)
        {
            perror("HH_ERROR: listen() call failed");
            exit(1);
        }

    printf("listening ...\n");

    int keep_socket = 1;
    while (keep_socket) {

        int client_socket_fd;
        int client_address_len;

        // int accept(int sockfd, struct  sockaddr *addr, socklen_t *addrlen);
        // the application that called listen() then accepts the connection using accept()
        // accept() creates a new socket and this new socket is connected to the peer socket that performed the connect()
        // a file descriptor for the connected socket is returned as the function result of the accept() call
        // client_socket = accept(server_socket, NULL, NULL);

        client_address_len = sizeof(client_address);
        client_socket_fd = accept(server_socket, (struct sockaddr *)&client_address, &client_address_len);

        printf("Someone connected with address %d\n", client_address.sin_addr.s_addr);



        // т.е. мы взяли слушающий сокет, что-то из него достали и создали новый сокет куда положили то что надо
        // теперь client_socket это сокет (connection descriptor) который соединен с клиентом который запускал connect()

        if (client_socket_fd < 0)
        {
            perror("HH_ERROR: accept() call failed");
            continue;
        }

        // send the message
        //send(client_socket_fd, server_message, sizeof(server_message), 0);

        processing_message_service(client_socket_fd, client_socket_fd);
        close(client_socket_fd);
    };

    // close the socket


	return 0;
}