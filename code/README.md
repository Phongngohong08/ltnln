# Lập trình nhân Linux — Mã nguồn & Kết quả

Đề tài: **Lập trình shell quản lý hệ thống — Lập trình quản lý tiến trình/file/socket/network — Xây dựng module nhân — Hệ thống ARM Linux mô phỏng bằng Buildroot & QEMU.**

Thư mục này chứa mã nguồn của **3 phần đầu**. Phần 4 (ARM Linux + Buildroot/QEMU) được trình bày riêng trong báo cáo (`bao_cao_final.docx`).

> Toàn bộ chương trình được viết và chạy trên **Ubuntu** (kiểm thử trên Ubuntu 20.04/22.04). Cần cài: `sudo apt-get install build-essential linux-headers-$(uname -r)`.

```
code/
├── 1_shell/        # Phần 1: lập trình shell quản lý hệ thống
│   ├── menu.sh                  # menu tổng hợp gọi 4 chương trình con
│   ├── quanly_file.sh          # quản lý file/thư mục
│   ├── laplich_tacvu.sh        # lập lịch tác vụ (crontab)
│   ├── thoigian_hethong.sh     # thiết lập thời gian hệ thống
│   └── caidat_goibo.sh         # cài đặt/gỡ bỏ chương trình (APT)
├── 2_c/            # Phần 2: lập trình C quản lý tiến trình/file/socket/network
│   ├── quanly_file.c           # quản lý file bằng system call
│   ├── quanly_tientrinh.c      # quản lý tiến trình (fork/exec/kill)
│   ├── network.c               # liệt kê giao diện & IP
│   ├── server.c                # socket TCP phía server
│   ├── client.c                # socket TCP phía client
│   └── Makefile
└── 3_kernel/       # Phần 3: lập trình module nhân
    ├── hello_module.c
    └── Makefile
```

---

## Phần 1 — Lập trình Shell quản lý hệ thống

Cấp quyền thực thi rồi chạy menu tổng hợp:

```bash
cd 1_shell
chmod +x *.sh
./menu.sh
```

| Script | Chức năng | Kết quả mong đợi |
|--------|-----------|------------------|
| `quanly_file.sh` | Tạo/đổi tên/sao chép/di chuyển/xóa, điều hướng, liệt kê, phân quyền (`chmod`), nén (`tar`) | Thao tác đúng trên file/thư mục, in thông báo `[OK]` / `[Lỗi]` |
| `laplich_tacvu.sh` | Liệt kê/thêm/sửa/xóa tác vụ định kỳ qua `crontab` | Tác vụ được ghi vào crontab của người dùng |
| `thoigian_hethong.sh` | Xem/đặt giờ, ngày, múi giờ; đồng bộ NTP (`ntpdate`) | Giờ hệ thống thay đổi (cần `sudo`) |
| `caidat_goibo.sh` | Kiểm tra/cài/gỡ/cập nhật/tìm gói bằng `apt`, `dpkg` | Gói được cài hoặc gỡ thành công (cần `sudo`) |

---

## Phần 2 — Lập trình C (tiến trình, file, socket, network)

Biên dịch toàn bộ bằng Makefile:

```bash
cd 2_c
make            # tạo: quanly_file quanly_tientrinh network server client
```

- **`./quanly_file`** — quản lý file qua system call (`stat`, `chmod`, `opendir/readdir`, `fopen`...): kiểm tra tồn tại, tạo, đọc, ghi thêm, xóa, xem/cấp quyền, liệt kê thư mục.
- **`./quanly_tientrinh`** — quản lý tiến trình: liệt kê (`ps`), tạo tiến trình con bằng `fork()` + `execl()`, gửi tín hiệu bằng `kill()`, tiêu diệt tiến trình.
- **`./network`** — liệt kê các giao diện mạng và địa chỉ IPv4/IPv6 bằng `getifaddrs()`.
- **Socket TCP (client–server):** mở 2 cửa sổ terminal:

  ```bash
  ./server        # cửa sổ 1: lắng nghe cổng 8080
  ./client        # cửa sổ 2: kết nối 127.0.0.1:8080
  ```

  Hai bên nhắn tin qua lại; gõ `bye` để kết thúc. (Chạy khác máy thì sửa `SERVER_IP` trong `client.c`.)

Dọn dẹp: `make clean`.

---

## Phần 3 — Lập trình Module nhân Linux

```bash
cd 3_kernel
make                                   # tạo hello_module.ko
sudo insmod hello_module.ko n=10 ten="Linux"
dmesg | tail -n 20                     # xem log của module
sudo rmmod hello_module                # gỡ module
make clean
```

Module minh hoạ vòng đời module (`__init` / `__exit`), nhận tham số đầu vào (`module_param`), và tính toán trong không gian nhân (tổng `1..n`, liệt kê số nguyên tố ≤ `n`) rồi ghi ra log nhân bằng `printk`.

**Kết quả mong đợi trong `dmesg`:**

```
hello_module: Xin chao, Linux!
hello_module: Module da duoc nap. Tham so n=10
hello_module: Tong 1..10 = 55
hello_module: Cac so nguyen to <= 10:
2 3 5 7
hello_module: Co tong cong 4 so nguyen to <= 10
```

Khi gỡ module:

```
hello_module: Tam biet, Linux! Module da duoc go.
```

---

## Ghi chú

- Các thao tác đổi giờ hệ thống, cài/gỡ gói và nạp/gỡ module cần quyền **`sudo`**.
- Mã nguồn dùng các API của Linux (POSIX, libc, Linux kernel API) nên **chỉ biên dịch & chạy trên Linux/Ubuntu**, không chạy trên Windows.
