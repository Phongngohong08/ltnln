/* =============================================================
 * Phan 2 - Lap trinh C: THONG TIN MANG (NETWORK)
 * Liet ke cac giao dien mang va dia chi IP (IPv4/IPv6) cua may.
 * Bien dich:  gcc network.c -o network
 * Chay     :  ./network
 * ============================================================= */
#include <stdio.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

int main(void) {
    struct ifaddrs *ds, *d;
    if (getifaddrs(&ds) == -1) { perror("getifaddrs"); return 1; }

    printf("%-10s %-6s %s\n", "GIAO DIEN", "LOAI", "DIA CHI");
    printf("-------------------------------------------\n");
    for (d = ds; d != NULL; d = d->ifa_next) {
        if (d->ifa_addr == NULL) continue;
        int fam = d->ifa_addr->sa_family;
        if (fam == AF_INET || fam == AF_INET6) {
            char host[NI_MAXHOST];
            int len = (fam == AF_INET) ? sizeof(struct sockaddr_in)
                                       : sizeof(struct sockaddr_in6);
            if (getnameinfo(d->ifa_addr, len, host, sizeof(host),
                            NULL, 0, NI_NUMERICHOST) == 0)
                printf("%-10s %-6s %s\n", d->ifa_name,
                       (fam == AF_INET) ? "IPv4" : "IPv6", host);
        }
    }
    freeifaddrs(ds);
    return 0;
}
