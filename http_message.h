#ifndef HTTP_MESSAGE_H
#define HTTP_MESSAGE_H

typedef struct http_message
{
    char* method;
    char* path;
    char* http_version;
    char* body;
    int body_len;
    char* headers;
} http_msg_client_t;

typedef enum {
    BAD_REQUEST,
    CLOSED_CONNECTION,
    MESSAGE
} http_msg_read_result_t;

read_http_client_message(int client_sock, http_msg_client_t** msg, http_msg_read_result_t* result);
#endif