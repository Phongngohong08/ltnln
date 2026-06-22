/* =============================================================
 * Phan 2 - Lap trinh C: SOCKET - PHIA CLIENT (TCP)
 * Tao socket, connect den server va trao doi tin nhan.
 * Bien dich:  gcc client.c -o client
 * Chay     :  ./client            (chay sau khi da chay ./server)
 * Go "bye" de ket thuc.
 * ============================================================= */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define MAXLEN 1024
#define SERVER_IP "127.0.0.1"   /* doi thanh IP server neu chay tren may khac */

int main(void) {
    int sock;
    struct sockaddr_in serv;
    char msg[MAXLEN], buffer[MAXLEN];

    /* 1. Tao socket */
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket"); return 1;
    }

    serv.sin_family = AF_INET;
    serv.sin_port = htons(PORT);
    if (inet_pton(AF_INET, SERVER_IP, &serv.sin_addr) <= 0) {
        perror("inet_pton"); return 1;
    }

    /* 2. Ket noi den server */
    if (connect(sock, (struct sockaddr *)&serv, sizeof(serv)) < 0) {
        perror("connect"); return 1;
    }
    printf("Da ket noi den server %s:%d\n", SERVER_IP, PORT);

    /* 3. Trao doi tin nhan */
    while (1) {
        printf("Ban: ");
        memset(msg, 0, MAXLEN);
        if (!fgets(msg, MAXLEN, stdin)) break;
        send(sock, msg, strlen(msg), 0);
        if (strncmp(msg, "bye", 3) == 0) { printf("Ket thuc.\n"); break; }

        memset(buffer, 0, MAXLEN);
        int n = read(sock, buffer, MAXLEN - 1);
        if (n <= 0) { printf("Server da ngat ket noi.\n"); break; }
        printf("Server: %s", buffer);
    }

    close(sock);
    return 0;
}
