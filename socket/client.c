#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT "3000"
#define BUFF_SIZE 100
#define FILE_BUFFER 4000

int main(void)
{
    int client_sock_fd;
    int data_size_recv;
    int data_sent_size;
    int connect_status;
    int addrinfo_status;
    char serv_client_buffer[BUFF_SIZE];
    struct addrinfo addr_info_ptr;
    struct addrinfo *addr_sock_bound_to;

    memset(&addr_info_ptr, 0, sizeof(addr_info_ptr));
    addr_info_ptr.ai_family = AF_UNSPEC;
    addr_info_ptr.ai_socktype = SOCK_STREAM;

    addrinfo_status = getaddrinfo("127.0.0.0", PORT, &addr_info_ptr, &addr_sock_bound_to);
    printf("addrinfo_status %d\n", addrinfo_status);

    client_sock_fd = socket(addr_sock_bound_to->ai_family, addr_sock_bound_to->ai_socktype, addr_sock_bound_to->ai_protocol);
    connect_status = connect(client_sock_fd, addr_sock_bound_to->ai_addr, addr_sock_bound_to->ai_addrlen);
    
    strcpy(serv_client_buffer, "file.txt");
    send(client_sock_fd, serv_client_buffer, strlen(serv_client_buffer), 0);
    memset(&serv_client_buffer, 0, sizeof(serv_client_buffer));
    data_size_recv = recv(client_sock_fd, serv_client_buffer, BUFF_SIZE, 0);
    serv_client_buffer[data_size_recv] = '\0';
    printf("server echo: %s\n", serv_client_buffer);
    memset(&serv_client_buffer, 0, sizeof(serv_client_buffer));
    
    
    strcpy(serv_client_buffer, "new content");
    send(client_sock_fd, serv_client_buffer, strlen(serv_client_buffer), 0);
    memset(&serv_client_buffer, 0, sizeof(serv_client_buffer));  
    data_size_recv = recv(client_sock_fd, serv_client_buffer, BUFF_SIZE, 0);
    serv_client_buffer[data_size_recv] = '\0';
    printf("server echo: %s\n", serv_client_buffer);
    memset(&serv_client_buffer, 0, sizeof(serv_client_buffer)); 


    close(client_sock_fd);
    
    return 0;
}