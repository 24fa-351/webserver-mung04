#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <http_message.h>

#define PORT 9000

// Bind sometimes fails even if no code is changed. Re-compiling will eventually fix it.
// it's almost 4 am im going to sleep. finish this later...

int respond_to_http_client_msg(int sock_fd, http_msg_client_t* msg) {
    char* response = "HTTP/1.1 200 OK\r\nContent-Length: 0\r\n\r\n";
    write(sock_fd, response, strlen(response));
    return 0;
}

void connection(int* sock_fd_addr) {
    int sock_conn = *sock_fd_addr;
    // free(sock_fd_addr); <- gives invalid pointer error?

    printf("Handling %d\n", sock_conn);
    char buffer[1024];
    int bytes = read(sock_conn, buffer, sizeof(buffer));
    printf("Received from %d: %s\n", sock_conn, buffer);
    write(sock_conn, buffer, bytes);
    close(sock_conn);
    return;
}

int main(int argc, char* argv[]) {
    
    int fd_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (fd_socket == -1) {
        printf("Failed creation\n");
        exit(0);
    }

    if (argc > 2) {
        if (strcmp(argv[1], "-p") == 0) {
            printf("Current Port: %d\n", PORT);
        }
    }

    struct sockaddr_in server_address;

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);    

    int bindval = bind(fd_socket, (struct sockaddr*)&server_address, sizeof(server_address));

    if (bindval < 0) {
        printf("Bind failed\n");
        exit(0);
    }

    bindval = listen(fd_socket, 5);

    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);

    // int conn_sock_fd = accept(fd_socket, (struct sockaddr*)&client_addr, &addr_len);

    while (1) {
        pthread_t thd;

        int conn_sock_fd = accept(fd_socket, (struct sockaddr*)&client_addr, &addr_len);

        pthread_create(&thd, NULL, (void* (*) (void*))connection, (void*)&conn_sock_fd);

        // char buffer[1024];
        // int bytes = read(conn_sock_fd, buffer, sizeof(buffer));
        // printf("Received: %s\n", buffer);
        // write(conn_sock_fd, buffer, bytes);
        // if (bytes < 0) {
        //     printf("Exiting...\n");
        //     break;
        // }
    
    }

    // close(conn_sock_fd);

    return 0;
}