/* =============================================================
 * Phan 2 - Lap trinh C: QUAN LY TIEN TRINH tren Ubuntu
 * Minh hoa: liet ke tien trinh, tao tien trinh con (fork/exec),
 * gui tin hieu (kill), tieu diet tien trinh, doi tien trinh con.
 * Bien dich:  gcc quanly_tientrinh.c -o quanly_tientrinh
 * Chay     :  ./quanly_tientrinh
 * ============================================================= */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/* Liet ke tat ca tien trinh bang lenh ps */
void liet_ke(void) {
    printf("====== Danh sach tien trinh ======\n");
    system("ps -e -o pid,ppid,stat,cmd --sort=pid | head -n 30");
}

/* Tao mot tien trinh con chay mot lenh do nguoi dung nhap */
void tao_tientrinh(void) {
    char lenh[256];
    printf("Nhap lenh cho tien trinh con (vd: ls -l): ");
    getchar();
    fgets(lenh, sizeof(lenh), stdin);
    lenh[strcspn(lenh, "\n")] = '\0';

    pid_t pid = fork();
    if (pid < 0) { perror("fork"); return; }
    if (pid == 0) {                          /* tien trinh con */
        execl("/bin/sh", "sh", "-c", lenh, (char *)NULL);
        perror("execl");                     /* chi chay neu exec loi */
        exit(1);
    } else {                                 /* tien trinh cha */
        int st;
        waitpid(pid, &st, 0);
        printf("[OK] Tien trinh con PID=%d ket thuc voi ma %d\n", pid, WEXITSTATUS(st));
    }
}

/* Gui mot tin hieu bat ky den tien trinh */
void gui_tinhieu(void) {
    int pid, sig;
    printf("Nhap PID: "); scanf("%d", &pid);
    printf("Nhap so tin hieu (vd 9=SIGKILL, 15=SIGTERM, 19=SIGSTOP): ");
    scanf("%d", &sig);
    if (kill(pid, sig) == 0) printf("[OK] Da gui tin hieu %d den PID %d\n", sig, pid);
    else perror("kill");
}

/* Tieu diet tien trinh (SIGKILL) */
void tieu_diet(void) {
    int pid;
    printf("Nhap PID can tieu diet: "); scanf("%d", &pid);
    if (kill(pid, SIGKILL) == 0) printf("[OK] Da tieu diet PID %d\n", pid);
    else perror("kill");
}

int main(void) {
    int chon;
    do {
        printf("\n========== QUAN LY TIEN TRINH (C) ==========\n");
        printf("PID cua chuong trinh nay: %d\n", getpid());
        printf("1. Liet ke tien trinh\n");
        printf("2. Tao tien trinh con (fork/exec)\n");
        printf("3. Gui tin hieu den tien trinh\n");
        printf("4. Tieu diet tien trinh\n");
        printf("0. Thoat\n");
        printf("Chon: ");
        if (scanf("%d", &chon) != 1) break;
        switch (chon) {
            case 1: liet_ke(); break;
            case 2: tao_tientrinh(); break;
            case 3: gui_tinhieu(); break;
            case 4: tieu_diet(); break;
            case 0: printf("Ket thuc.\n"); break;
            default: printf("Lua chon khong hop le.\n");
        }
    } while (chon != 0);
    return 0;
}
