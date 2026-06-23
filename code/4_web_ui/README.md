# Giao diện Web — Hệ thống Quản trị Linux

Giao diện web (phong cách cổng thông tin điện tử hành chính) cho **3 phần đầu** của đề tài:
quản lý hệ thống bằng Shell, lập trình C, và module nhân. Backend dùng **Flask**, thực thi
các thao tác hệ thống thật sự trên Ubuntu.

> ⚠️ Ứng dụng chạy lệnh hệ thống thật (crontab, apt, kill, insmod…). Hãy chạy trên **Ubuntu**.
> Trên Windows chỉ xem được giao diện, các thao tác hệ thống sẽ báo lỗi.

## Cài đặt & chạy

```bash
cd code/4_web_ui
python3 -m venv venv && source venv/bin/activate   # (tùy chọn)
pip install -r requirements.txt
python3 app.py
```

Mở trình duyệt: **http://127.0.0.1:5000**

Một số chức năng cần quyền quản trị (đổi giờ, cài/gỡ gói, nạp/gỡ module). Nếu cần, chạy với
sudo hoặc cấu hình sudoers:

```bash
sudo python3 app.py
```

## Chức năng theo từng phần

| Phần | Trang | Chức năng |
|------|-------|-----------|
| **1 · Shell** | Quản lý tệp tin | Tạo, đổi tên, sao chép, di chuyển, xóa, phân quyền, nén — trong `~/ltnln_workspace` |
| | Lập lịch tác vụ | Liệt kê / thêm / xóa tác vụ `crontab` |
| | Thời gian hệ thống | Xem giờ, đặt giờ/ngày/múi giờ, đồng bộ NTP |
| | Quản lý gói | Kiểm tra / cài / gỡ / tìm / cập nhật gói (apt, dpkg) |
| **2 · C** | Quản lý tiến trình | Liệt kê tiến trình, tạo tiến trình (fork/exec), gửi tín hiệu/kill |
| | Quản lý tệp tin (C) | Như phần file, minh họa system call |
| | Thông tin mạng | Liệt kê giao diện & IP (IPv4/IPv6) |
| | Socket TCP | Client gửi tin → echo server phản hồi (server.c/client.c) |
| **3 · Nhân** | Module nhân | Biên dịch (make), nạp (insmod), gỡ (rmmod), trạng thái (lsmod), nhật ký (dmesg) |

## Cấu trúc thư mục

```
4_web_ui/
├── app.py                 # backend Flask + các API
├── requirements.txt
├── templates/             # giao diện (Jinja2)
│   ├── base.html          # bố cục chung: header, nav, sidebar, footer
│   ├── index.html         # trang chủ
│   ├── file.html  cron.html  time.html  pkg.html
│   ├── proc.html  net.html   socket.html
│   └── kernel.html
└── static/
    ├── css/style.css      # giao diện phong cách hành chính (navy/đỏ)
    └── js/main.js         # tiện ích gọi API & hiển thị kết quả
```

## Ghi chú
- Chức năng quản lý tệp tin chỉ thao tác trong `~/ltnln_workspace` để an toàn cho hệ thống.
- Trang Module nhân biên dịch mã nguồn ở `code/3_kernel`, cần `build-essential` và
  `linux-headers-$(uname -r)`.
