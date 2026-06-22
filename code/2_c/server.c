/* =============================================================
 * Phan 2 - Lap trinh C: SOCKET - PHIA SERVER (TCP)
 * Tao socket, bind, listen, accept va trao doi tin nhan voi client.
 * Bien dich:  gcc server.c -o server
 * Chay     :  ./server      (chay truoc, roi chay ./client)
 * ============================================================= */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define MAXLEN 1024

int main(void) {
    int server_fd, client_fd;
    struct sockaddr_in addr;
    socklen_t addrlen = sizeof(addr);
    char buffer[MAXLEN], reply[MAXLEN];

    /* 1. Tao socket */
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket"); exit(1);
    }

    /* Cho phep tai su dung dia chi ngay sau khi tat server */
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    /* 2. Bind socket vao cong */
    if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind"); exit(1);
    }

    /* 3. Lang nghe ket noi */
    if (listen(server_fd, 3) < 0) { perror("listen"); exit(1); }
    printf("Server dang lang nghe tai cong %d ...\n", PORT);

    /* 4. Chap nhan ket noi tu client */
    if ((client_fd = accept(server_fd, (struct sockaddr *)&addr, &addrlen)) < 0) {
        perror("accept"); exit(1);
    }
    printf("Da ket noi voi client %s\n", inet_ntoa(addr.sin_addr));

    /* 5. Trao doi tin nhan */
    while (1) {
        memset(buffer, 0, MAXLEN);
        int n = read(client_fd, buffer, MAXLEN - 1);
        if (n <= 0) { printf("Client da ngat ket noi.\n"); break; }
        printf("Client: %s", buffer);
        if (strncmp(buffer, "bye", 3) == 0) break;

        printf("Tra loi client: ");
        memset(reply, 0, MAXLEN);
        fgets(reply, MAXLEN, stdin);
        send(client_fd, reply, strlen(reply), 0);
    }

    close(client_fd);
    close(server_fd);
    return 0;
}
