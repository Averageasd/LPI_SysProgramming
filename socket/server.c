#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#define NUM_PENDING_CONNECTIONS 1
#define BUFFER_SIZE 100
#define PORT "3000"

int main()
{
    int server_fd;
    int client_handler_fd;
    int reuse_addr_flag;
    int bind_status;
    int num_client;
    int i;
    int client_byte_read;
    struct addrinfo addr_info_ptr;
    struct addrinfo *server_info_struct;
    struct sockaddr_storage client_addr;
    socklen_t client_sock_size;
    char buffer[BUFFER_SIZE];
    FILE * file_stg_manipulation;

    memset(&addr_info_ptr, 0, sizeof addr_info_ptr);
    addr_info_ptr.ai_family = AF_UNSPEC;
    addr_info_ptr.ai_socktype = SOCK_STREAM;
    addr_info_ptr.ai_flags = AI_PASSIVE;
    reuse_addr_flag = 1;
    bind_status = 0;
    num_client = 0;
    int send_dat_status = 0;
    i = 0;
    file_stg_manipulation = NULL;

    getaddrinfo(NULL, PORT, &addr_info_ptr, &server_info_struct);

    server_fd = socket(server_info_struct->ai_family, server_info_struct->ai_socktype, server_info_struct->ai_protocol);

    if (server_fd == -1)
    {
        perror("error");
        exit(1);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse_addr_flag, sizeof(int)) == -1)
    {
        perror("setsockopt");
        exit(1);
    }

    // bind current socket to the addr struct we created aove
    bind_status = bind(server_fd, server_info_struct->ai_addr, server_info_struct->ai_addrlen);
    if (bind_status == -1)
    {
        perror("bind error");
        close(server_fd);
        exit(1);
    }

    if (listen(server_fd, NUM_PENDING_CONNECTIONS) == -1)
    {
        perror("listen");
        exit(1);
    }

    client_sock_size = sizeof(client_addr);
    client_handler_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_sock_size);
    client_byte_read = recv(client_handler_fd, buffer, BUFFER_SIZE, 0);
    buffer[client_byte_read] = '\0';
    file_stg_manipulation = fopen(buffer, "w+");
    printf("string len of %s is %ld\n", buffer, strlen(buffer));
    send(client_handler_fd, buffer, strlen(buffer),0);
    memset(&buffer, 0, sizeof(buffer));

    client_byte_read = recv(client_handler_fd, buffer, BUFFER_SIZE, 0);
    buffer[client_byte_read] = '\0';
    fprintf(file_stg_manipulation, "%s",buffer);
    send(client_handler_fd, buffer, strlen(buffer),0);
    memset(&buffer, 0, sizeof(buffer));

    close(client_handler_fd);
    close(server_fd);   
    

    return 0;
}