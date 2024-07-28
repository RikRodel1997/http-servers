#include "../serv_settings.h"

int bind_to_port(int server_sock, addr_in serv_addr);
addr_in create_addr_in(uint16_t port);
struct sockaddr* to_sockaddr(addr_in* addr);

int set_serv_settings(int server_sock) {
    addr_in serv_addr = create_addr_in(PORT);
    if (server_sock == -1) {
        printf("Socket creation failed: %s...\n", strerror(errno));
        return 1;
    }

    int reuse = 1;
    if (setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1) {
        printf("Socket reuse option failed: %s...\n", strerror(errno));
        return 1;
    }

    if (bind_to_port(server_sock, serv_addr) == -1) {
        printf("Was unable to bind to any port.\n");
        return 1;
    }

    int max_conn = 5;
    if (listen(server_sock, max_conn) == -1) {
        printf("Listen failed: %s \n", strerror(errno));
        return 1;
    }
    return 0;
}

int bind_to_port(int server_sock, addr_in serv_addr) {
    int bind_success = bind(server_sock, to_sockaddr(&serv_addr), sizeof(serv_addr));
    int bind_tries = 0;
    while (bind_success == -1 && bind_tries < MAX_BIND_TRIES) {
        bind_tries++;
        int tries_left = MAX_BIND_TRIES - bind_tries;
        printf("Bind to port %d failed: %s. %d tries left.\n", ntohs(serv_addr.sin_port), strerror(errno), tries_left);
        serv_addr.sin_port = htons(ntohs(serv_addr.sin_port) + 1);
        bind_success = bind(server_sock, to_sockaddr(&serv_addr), sizeof(serv_addr));
        if (bind_success == 0) {
            printf("Successfully retried to bind to port %d\n", ntohs(serv_addr.sin_port));
            return 0;
        } else if (bind_success == 0 && bind_tries == MAX_BIND_TRIES) {
            return -1;
        }
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
