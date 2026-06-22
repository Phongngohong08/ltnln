#!/bin/bash
# =============================================================
# Phan 1 - Lap trinh shell: THIET LAP THOI GIAN HE THONG
# Chuc nang: xem gio, dat gio, dat ngay, dat mui gio,
#            dong bo gio qua NTP.
# Luu y: cac thao tac thay doi gio can quyen sudo.
# Cach chay:  chmod +x thoigian_hethong.sh && ./thoigian_hethong.sh
# =============================================================

xem_gio() {
    echo "Thoi gian hien tai: $(date '+%Y-%m-%d %H:%M:%S')"
    echo "Mui gio        : $(date '+%Z %z')"
}

dat_gio() {
    read -p "Nhap gio moi (HH:MM:SS): " g
    if sudo date +%T -s "$g"; then
        echo "[OK] Da dat gio."
        xem_gio
    else
        echo "[Loi] Khong the dat gio."
    fi
}

dat_ngay() {
    read -p "Nhap ngay moi (YYYY-MM-DD): " n
    if sudo date +%F -s "$n"; then
        echo "[OK] Da dat ngay."
        xem_gio
    else
        echo "[Loi] Khong the dat ngay."
    fi
}

dat_muigio() {
    read -p "Nhap mui gio (vd: Asia/Ho_Chi_Minh): " tz
    if sudo timedatectl set-timezone "$tz"; then
        echo "[OK] Da dat mui gio $tz."
        xem_gio
    else
        echo "[Loi] Khong the dat mui gio."
    fi
}

dong_bo_ntp() {
    echo "Dang cai dat va dong bo gio qua NTP..."
    sudo apt-get install -y ntpdate >/dev/null 2>&1
    if sudo ntpdate pool.ntp.org; then
        echo "[OK] Da dong bo gio tu may chu NTP."
        xem_gio
    else
        echo "[Loi] Khong the dong bo (kiem tra ket noi mang)."
    fi
}

while true; do
    echo
    echo "===== THIET LAP THOI GIAN HE THONG ====="
    echo "1. Xem thoi gian hien tai"
    echo "2. Dat gio"
    echo "3. Dat ngay"
    echo "4. Dat mui gio"
    echo "5. Dong bo gio qua NTP"
    echo "0. Thoat"
    echo "========================================"
    read -p "Chon chuc nang: " c
    case $c in
        1) xem_gio ;;
        2) dat_gio ;;
        3) dat_ngay ;;
        4) dat_muigio ;;
        5) dong_bo_ntp ;;
        0) echo "Ket thuc chuong trinh."; break ;;
        *) echo "Lua chon khong hop le." ;;
    esac
done
