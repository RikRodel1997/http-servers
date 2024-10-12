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

#ifndef SERV_SETTINGS_H
#define SERV_SETTINGS_H

#define MAX_BIND_TRIES 10
#define PORT           4221

typedef struct sockaddr_in addr_in;

int set_serv_settings(int server_sock);
struct sockaddr* to_sockaddr(addr_in* addr);
addr_in create_addr_in(uint16_t port);

#endif   // SERV_SETTINGS_H
