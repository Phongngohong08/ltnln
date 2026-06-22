/* =============================================================
 * Phan 3 - Lap trinh nhan: MODULE NHAN LINUX CO BAN
 * Module minh hoa:
 *   - Ham init (__init) chay khi nap module (insmod)
 *   - Ham exit (__exit) chay khi go module (rmmod)
 *   - Nhan tham so dau vao bang module_param
 *   - Tinh toan trong khong gian nhan va ghi log bang printk
 * Cach build:  make
 * Nap module:  sudo insmod hello_module.ko n=10 ten="Linux"
 * Xem log   :  dmesg | tail -n 20
 * Go module :  sudo rmmod hello_module
 * ============================================================= */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/moduleparam.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sinh vien LTNL");
MODULE_DESCRIPTION("Module nhan Linux co ban: tham so + tinh toan");
MODULE_VERSION("1.0");

/* Tham so truyen vao khi nap module */
static int n = 5;
static char *ten = "the gioi";
module_param(n, int, 0644);
MODULE_PARM_DESC(n, "So nguyen de tinh tong va liet ke so nguyen to");
module_param(ten, charp, 0644);
MODULE_PARM_DESC(ten, "Ten hien thi trong loi chao");

/* Kiem tra so nguyen to */
static int la_nguyen_to(int x) {
    int i;
    if (x < 2) return 0;
    for (i = 2; (long)i * i <= x; i++)
        if (x % i == 0) return 0;
    return 1;
}

static int __init hello_init(void) {
    int i, tong = 0, dem_nt = 0;

    printk(KERN_INFO "hello_module: Xin chao, %s!\n", ten);
    printk(KERN_INFO "hello_module: Module da duoc nap. Tham so n=%d\n", n);

    /* Tinh tong tu 1 den n */
    for (i = 1; i <= n; i++) tong += i;
    printk(KERN_INFO "hello_module: Tong 1..%d = %d\n", n, tong);

    /* Liet ke so nguyen to <= n */
    printk(KERN_INFO "hello_module: Cac so nguyen to <= %d:\n", n);
    for (i = 2; i <= n; i++) {
        if (la_nguyen_to(i)) {
            dem_nt++;
            printk(KERN_CONT "%d ", i);
        }
    }
    printk(KERN_INFO "hello_module: Co tong cong %d so nguyen to <= %d\n", dem_nt, n);

    return 0;   /* tra ve 0 = nap thanh cong */
}

static void __exit hello_exit(void) {
    printk(KERN_INFO "hello_module: Tam biet, %s! Module da duoc go.\n", ten);
}

module_init(hello_init);
module_exit(hello_exit);
