#include <errno.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <zlib.h>

#include "include/request.h"

#define BUFFER_SIZE      8192
#define DIR_BUFF         256
#define HTTP_HEADER_SIZE 256
#define PORT             4221

typedef struct sockaddr_in addr_in;

char* split_and_keep_left(char* str);
void get_dir(int argc, char* argv[], char* dir);
int gzip(const char* input, int inputSize, char* output, int outputSize);
void print_hex(const char* data, int length);
addr_in create_addr_in(uint16_t port);
struct sockaddr* to_sockaddr(addr_in* addr);
int set_serv_settings(int server_sock, addr_in serv_addr);

int main(int argc, char* argv[]) {
    char dir[DIR_BUFF];
    get_dir(argc, argv, dir);

    setbuf(stdout, NULL);

    addr_in serv_addr = create_addr_in(PORT);
    addr_in client_addr;

    int server_sock = socket(AF_INET, SOCK_STREAM, 0);
    set_serv_settings(server_sock, serv_addr);

    for (;;) {
        int client_addr_len = sizeof(client_addr);
        int accept_fd = accept(server_sock, (struct sockaddr*) &client_addr, &client_addr_len);
        if (accept_fd < 0) {
            continue;
        }

        char req_buff[BUFFER_SIZE] = {0};
        char req_buff_copy[BUFFER_SIZE] = {0};
        char req_buff_copy2[BUFFER_SIZE] = {0};

        ssize_t bytes_received = recv(accept_fd, req_buff, BUFFER_SIZE, 0);
        if (bytes_received < 0) {
            printf("Failed to receive data: %s\n", strerror(errno));
            close(accept_fd);
            continue;
        }
        req_buff[bytes_received] = '\0';

        strncpy(req_buff_copy, req_buff, bytes_received);
        strncpy(req_buff_copy2, req_buff, bytes_received);

        char res[BUFFER_SIZE + HTTP_HEADER_SIZE] = {0};
        char response_buff[BUFFER_SIZE + HTTP_HEADER_SIZE] = {0};
        char* file_response = NULL;

        request req = parse_request(req_buff);
        char* method = req.method;
        char* path = req.path;
        // char* headers = req.headers; //TODO

        if (strncmp(path, "/echo/", 6) == 0) {
            char echo_tail[7];   // +1 for null terminator
            get_echo_tail(path, echo_tail);

            char* headers = strstr(req_buff_copy, "\n") + 1;   // Skip "\r\n\r\n"
            char* accept_encoding = NULL;
            char* header_line = strtok(headers, "\r\n");
            char* format = NULL;

            while (header_line != NULL) {
                size_t header_len = strlen("Accept-Encoding:");
                if (strncasecmp(header_line, "Accept-Encoding:", header_len) == 0) {
                    printf("Found header: %s\n", header_line);
                    accept_encoding = header_line + header_len;
                    while (*accept_encoding == ' ') {
                        accept_encoding++;
                    }
                    break;
                }
                header_line = strtok(NULL, "\r\n");
            }

            if (accept_encoding != NULL) {
                printf("Accept-Encoding has value: %s\n", accept_encoding);
                char* token;
                const char s[2] = " ";
                token = strtok(accept_encoding, s);

                while (token != NULL) {
                    if (strncmp(token, "gzip", 4) == 0 || strncmp(token, "gzip, ", 5) == 0) {
                        char comp[BUFFER_SIZE];
                        int comp_len = gzip(echo_tail, strlen(echo_tail), comp, BUFFER_SIZE);
                        printf("Compressed data length: %d\n", comp_len);
                        printf("Compressed data (hex):\n");
                        print_hex(comp, comp_len);
                        format = "HTTP/1.1 200 OK\r\nContent-Encoding: "
                                 "gzip\r\nContent-Type: "
                                 "text/plain\r\nContent-Length: %d\r\n\r\n%s";
                        snprintf(response_buff, BUFFER_SIZE + HTTP_HEADER_SIZE, format, comp_len, echo_tail);
                        break;
                    } else {
                        format = "HTTP/1.1 200 OK\r\nContent-Type: "
                                 "text/plain\r\nContent-Length: %zu\r\n\r\n%s\n";
                        snprintf(response_buff, BUFFER_SIZE + HTTP_HEADER_SIZE, format, strlen(echo_tail), echo_tail);
                    }
                    token = strtok(NULL, s);
                }
            } else {
                format = "HTTP/1.1 200 OK\r\nContent-Type: "
                         "text/plain\r\nContent-Length: %zu\r\n\r\n%s\n";
                snprintf(response_buff, BUFFER_SIZE + HTTP_HEADER_SIZE, format, strlen(echo_tail), echo_tail);
            }

            strncpy(res, response_buff, BUFFER_SIZE + HTTP_HEADER_SIZE);
        } else if (strncmp(path, "/user-agent", 11) == 0) {
            char string1[100], string2[100], string3[100], string4[100], string5[100], string6[100], string7[100],
                string8[100], string9[100];

            sscanf(req_buff_copy, "%s %s %s %s %s %s %s %s %s", string1, string2, string3, string4, string5, string6,
                   string7, string8, string9);

            char* format = "HTTP/1.1 200 OK\r\nContent-Type: "
                           "text/plain\r\nContent-Length: %zu\r\n\r\n%s\n";
            snprintf(response_buff, BUFFER_SIZE + HTTP_HEADER_SIZE, format, strlen(string7), string7);
            strncpy(res, response_buff, BUFFER_SIZE + HTTP_HEADER_SIZE);
        } else if (strcmp(path, "/") == 0) {
            strncpy(res, "HTTP/1.1 200 OK\r\n\r\n", BUFFER_SIZE + HTTP_HEADER_SIZE);
        } else if (strncmp(path, "/files", 6) == 0 && strcmp(method, "GET") == 0) {
            char* file = strchr(path + 1, '/');
            if (file != NULL) {
                char file_path[BUFFER_SIZE] = {0};
                snprintf(file_path, BUFFER_SIZE, "%s%s", dir, file);

                if (dir[0] != '\0') {
                    FILE* f = fopen(file_path, "r");
                    if (f == NULL) {
                        perror("Error opening file\n");
                        printf("File path: %s\n", file_path);
                        strncpy(res,
                                "HTTP/1.1 404 Not Found\r\nContent-Length: "
                                "0\r\n\r\n",
                                BUFFER_SIZE + HTTP_HEADER_SIZE);
                    } else {
                        char buffer[BUFFER_SIZE] = {0};
                        size_t bytes_read = fread(buffer, 1, sizeof(buffer) - 1, f);
                        if (bytes_read > 0) {
                            file_response = malloc(BUFFER_SIZE + HTTP_HEADER_SIZE);
                            snprintf(file_response, BUFFER_SIZE + HTTP_HEADER_SIZE,
                                     "HTTP/1.1 200 OK\r\nContent-Type: "
                                     "application/octet-stream\r\nContent-Length: "
                                     "%zu\r\n\r\n",
                                     bytes_read);
                            memcpy(file_response + strlen(file_response), buffer, bytes_read);
                            strncpy(res, file_response, BUFFER_SIZE + HTTP_HEADER_SIZE);
                            free(file_response);
                        } else {
                            printf("Bytes read from file was 0: %s\n", file_path);
                            strncpy(res,
                                    "HTTP/1.1 404 Not Found\r\nContent-Length: "
                                    "0\r\n\r\n",
                                    BUFFER_SIZE + HTTP_HEADER_SIZE);
                        }
                        fclose(f);
                    }
                } else {
                    printf("Directory not set\n");
                    strncpy(res, "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n", BUFFER_SIZE + HTTP_HEADER_SIZE);
                }
            } else {
                printf("File path is invalid\n");
                strncpy(res, "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n", BUFFER_SIZE + HTTP_HEADER_SIZE);
            }
        } else if (strncmp(path, "/files", 6) == 0 && strcmp(method, "POST") == 0) {
            char* file_name = path + 7;
            char file_path[BUFFER_SIZE];
            snprintf(file_path, sizeof(file_path), "%s/%s", dir, file_name);

            char* content = strstr(req_buff_copy, "\r\n\r\n");
            if (content != NULL) {
                content += 4;   // Skip \r\n\r\n
                size_t content_length = bytes_received - (content - req_buff_copy);

                FILE* file_ptr = fopen(file_path, "w");
                fwrite(content, 1, content_length, file_ptr);
                fclose(file_ptr);
                strncpy(res, "HTTP/1.1 201 Created\r\n\r\n", BUFFER_SIZE + HTTP_HEADER_SIZE);
            } else {
                printf("Content not found in request\n");
                strncpy(res, "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\n\r\n", BUFFER_SIZE + HTTP_HEADER_SIZE);
            }
        } else {
            printf("Path %s is unknown\n", path);
            strncpy(res, "HTTP/1.1 404 Not Found\r\n\r\n", BUFFER_SIZE + HTTP_HEADER_SIZE);
        }

        int res_len = strlen(res);
        long bytes_sent = send(accept_fd, res, res_len, 0);

        if (bytes_sent <= 0) {
            perror("Failed to send response");
            close(accept_fd);
            return 1;
        }
        close(accept_fd);
    }
    close(server_sock);
    return 0;
}

int set_serv_settings(int server_sock, addr_in serv_addr) {
    if (server_sock == -1) {
        printf("Socket creation failed: %s...\n", strerror(errno));
        return 1;
    }

    int reuse = 1;
    if (setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1) {
        printf("Socket reuse option failed: %s...\n", strerror(errno));
        return 1;
    }

    if (bind(server_sock, to_sockaddr(&serv_addr), sizeof(serv_addr)) == -1) {
        printf("Bind failed: %s \n", strerror(errno));
        return 1;
    }

    int max_conn = 5;
    if (listen(server_sock, max_conn) == -1) {
        printf("Listen failed: %s \n", strerror(errno));
        return 1;
    }
}

addr_in create_addr_in(uint16_t port) {
    addr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    return addr;
}

struct sockaddr* to_sockaddr(addr_in* addr) {
    return (struct sockaddr*) addr;
}

void get_dir(int argc, char* argv[], char* dir) {
    if (argc >= 3) {
        char* find_arg = "--directory";
        if (strncmp(argv[1], find_arg, strlen(find_arg)) == 0) {
            printf("--directory argument passed: %s.\n", argv[2]);
            char* file_to_find = argv[2];
            if (access(file_to_find, F_OK) == 0) {
                printf("%s does exist\n", file_to_find);
                strcpy(dir, file_to_find);
            } else {
                printf("%s doesn't exist\n", file_to_find);
                dir[0] = '\0';
            }
        }
    }
}

char* split_and_keep_left(char* str) {
    char* pos = strchr(str, ' ');
    if (pos != NULL) {
        *pos = '\0';
    }
    return str;
}

int gzip(const char* input, int inputSize, char* output, int outputSize) {
    z_stream zs = {0};
    zs.zalloc = Z_NULL;
    zs.zfree = Z_NULL;
    zs.opaque = Z_NULL;

    zs.avail_in = (uInt) inputSize;
    zs.next_in = (Bytef*) input;
    zs.avail_out = (uInt) outputSize;
    zs.next_out = (Bytef*) output;

    deflateInit2(&zs, Z_DEFAULT_COMPRESSION, Z_DEFLATED, 15 | 16, 8, Z_DEFAULT_STRATEGY);
    deflate(&zs, Z_FINISH);
    deflateEnd(&zs);
    return zs.total_out;
}

void print_hex(const char* data, int length) {
    for (int i = 0; i < length; i++) {
        printf("%02x ", (unsigned char) data[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        }
    }
    printf("\n");
}