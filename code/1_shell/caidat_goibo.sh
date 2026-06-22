#!/bin/bash
# =============================================================
# Phan 1 - Lap trinh shell: CAI DAT / GO BO CHUONG TRINH
# Tu dong kiem tra goi, cai dat, go bo, cap nhat va tim kiem
# goi phan mem bang APT/dpkg tren Ubuntu.
# Luu y: can quyen sudo de cai dat / go bo.
# Cach chay:  chmod +x caidat_goibo.sh && ./caidat_goibo.sh
# =============================================================

kiem_tra() {
    read -p "Nhap ten goi: " goi
    if dpkg -s "$goi" >/dev/null 2>&1; then
        echo "[Co] Goi '$goi' da duoc cai dat."
    else
        echo "[Khong] Goi '$goi' chua duoc cai dat."
    fi
}

cai_dat() {
    read -p "Nhap ten goi can cai: " goi
    if dpkg -s "$goi" >/dev/null 2>&1; then
        echo "Goi '$goi' da co san, khong can cai lai."
        return
    fi
    sudo apt-get update
    if sudo apt-get install -y "$goi"; then
        echo "[OK] Da cai dat '$goi'."
    else
        echo "[Loi] Khong the cai dat '$goi'."
    fi
}

go_bo() {
    read -p "Nhap ten goi can go bo: " goi
    if ! dpkg -s "$goi" >/dev/null 2>&1; then
        echo "Goi '$goi' chua duoc cai, khong co gi de go."
        return
    fi
    if sudo apt-get remove -y "$goi"; then
        echo "[OK] Da go bo '$goi'."
    else
        echo "[Loi] Khong the go bo '$goi'."
    fi
}

cap_nhat() {
    echo "Dang cap nhat danh sach goi va nang cap he thong..."
    sudo apt-get update && sudo apt-get upgrade -y
    echo "[OK] Hoan tat cap nhat."
}

tim_kiem() {
    read -p "Nhap tu khoa tim goi: " tk
    apt-cache search "$tk"
}

while true; do
    echo
    echo "===== CAI DAT / GO BO CHUONG TRINH ====="
    echo "1. Kiem tra goi da cai chua"
    echo "2. Cai dat goi"
    echo "3. Go bo goi"
    echo "4. Cap nhat / nang cap he thong"
    echo "5. Tim kiem goi"
    echo "0. Thoat"
    echo "========================================"
    read -p "Chon chuc nang: " c
    case $c in
        1) kiem_tra ;;
        2) cai_dat ;;
        3) go_bo ;;
        4) cap_nhat ;;
        5) tim_kiem ;;
        0) echo "Ket thuc chuong trinh."; break ;;
        *) echo "Lua chon khong hop le." ;;
    esac
done
