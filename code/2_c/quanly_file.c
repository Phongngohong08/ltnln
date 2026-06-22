/* =============================================================
 * Phan 2 - Lap trinh C: QUAN LY FILE tren Ubuntu
 * Su dung cac system call: open/read/write, stat, chmod,
 * opendir/readdir, remove...
 * Bien dich:  gcc quanly_file.c -o quanly_file
 * Chay     :  ./quanly_file
 * ============================================================= */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

/* Kiem tra file co ton tai */
int ton_tai(const char *ten) {
    struct stat st;
    return stat(ten, &st) == 0;
}

/* Tao file rong */
void tao_file(const char *ten) {
    FILE *f = fopen(ten, "w");
    if (f) { fclose(f); printf("[OK] Da tao file '%s'\n", ten); }
    else   printf("[Loi] Khong tao duoc '%s'\n", ten);
}

/* Doc va in noi dung file */
void doc_file(const char *ten) {
    FILE *f = fopen(ten, "r");
    if (!f) { printf("[Loi] Khong doc duoc '%s'\n", ten); return; }
    int ch;
    printf("----- Noi dung '%s' -----\n", ten);
    while ((ch = fgetc(f)) != EOF) putchar(ch);
    printf("\n-------------------------\n");
    fclose(f);
}

/* Ghi them noi dung vao file */
void ghi_file(const char *ten) {
    FILE *f = fopen(ten, "a");
    if (!f) { printf("[Loi] Khong mo duoc '%s'\n", ten); return; }
    char buf[256];
    printf("Nhap noi dung can ghi them: ");
    getchar();                       /* bo ky tu newline con lai */
    fgets(buf, sizeof(buf), stdin);
    fputs(buf, f);
    fclose(f);
    printf("[OK] Da ghi vao '%s'\n", ten);
}

/* Xoa file */
void xoa_file(const char *ten) {
    if (remove(ten) == 0) printf("[OK] Da xoa '%s'\n", ten);
    else printf("[Loi] Khong xoa duoc '%s': %s\n", ten, strerror(errno));
}

/* Xem quyen cua file */
void xem_quyen(const char *ten) {
    struct stat st;
    if (stat(ten, &st) != 0) { printf("[Loi] Khong doc duoc thong tin '%s'\n", ten); return; }
    printf("Quyen cua '%s': ", ten);
    printf("%c%c%c%c%c%c%c%c%c\n",
        (st.st_mode & S_IRUSR) ? 'r' : '-', (st.st_mode & S_IWUSR) ? 'w' : '-', (st.st_mode & S_IXUSR) ? 'x' : '-',
        (st.st_mode & S_IRGRP) ? 'r' : '-', (st.st_mode & S_IWGRP) ? 'w' : '-', (st.st_mode & S_IXGRP) ? 'x' : '-',
        (st.st_mode & S_IROTH) ? 'r' : '-', (st.st_mode & S_IWOTH) ? 'w' : '-', (st.st_mode & S_IXOTH) ? 'x' : '-');
    printf("Kich thuoc: %ld bytes\n", (long)st.st_size);
}

/* Cap quyen cho file (chmod) */
void cap_quyen(const char *ten) {
    char q[10];
    printf("Nhap quyen (vd 755): ");
    scanf("%s", q);
    if (chmod(ten, strtol(q, NULL, 8)) == 0)
        printf("[OK] Da cap quyen %s cho '%s'\n", q, ten);
    else
        printf("[Loi] Khong cap quyen duoc: %s\n", strerror(errno));
}

/* Liet ke file/thu muc trong thu muc hien tai */
void liet_ke(const char *path) {
    DIR *d = opendir(path);
    if (!d) { printf("[Loi] Khong mo duoc thu muc '%s'\n", path); return; }
    struct dirent *e;
    printf("----- Noi dung thu muc '%s' -----\n", path);
    while ((e = readdir(d)) != NULL) printf("%s\n", e->d_name);
    closedir(d);
}

int main(void) {
    int chon;
    char ten[256];
    do {
        printf("\n========== QUAN LY FILE (C) ==========\n");
        printf("1. Kiem tra file ton tai\n");
        printf("2. Tao file\n");
        printf("3. Doc file\n");
        printf("4. Ghi them vao file\n");
        printf("5. Xoa file\n");
        printf("6. Xem quyen file\n");
        printf("7. Cap quyen file\n");
        printf("8. Liet ke thu muc hien tai\n");
        printf("0. Thoat\n");
        printf("Chon: ");
        if (scanf("%d", &chon) != 1) break;

        switch (chon) {
            case 1: printf("Ten file: "); scanf("%s", ten);
                    printf(ton_tai(ten) ? "File ton tai\n" : "File khong ton tai\n"); break;
            case 2: printf("Ten file: "); scanf("%s", ten); tao_file(ten); break;
            case 3: printf("Ten file: "); scanf("%s", ten); doc_file(ten); break;
            case 4: printf("Ten file: "); scanf("%s", ten); ghi_file(ten); break;
            case 5: printf("Ten file: "); scanf("%s", ten); xoa_file(ten); break;
            case 6: printf("Ten file: "); scanf("%s", ten); xem_quyen(ten); break;
            case 7: printf("Ten file: "); scanf("%s", ten); cap_quyen(ten); break;
            case 8: liet_ke("."); break;
            case 0: printf("Ket thuc.\n"); break;
            default: printf("Lua chon khong hop le.\n");
        }
    } while (chon != 0);
    return 0;
}
