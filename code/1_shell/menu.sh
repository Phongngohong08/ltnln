#!/bin/bash
# =============================================================
# Phan 1 - Lap trinh shell: MENU TONG HOP
# Goi den 4 chuong trinh con: quan ly file, lap lich tac vu,
# thiet lap thoi gian, cai dat/go bo chuong trinh.
# Cach chay:  chmod +x *.sh && ./menu.sh
# =============================================================

THUMUC="$(cd "$(dirname "$0")" && pwd)"

while true; do
    echo
    echo "############ LAP TRINH SHELL - PHAN 1 ############"
    echo "1. Quan ly file / thu muc"
    echo "2. Lap lich tac vu (crontab)"
    echo "3. Thiet lap thoi gian he thong"
    echo "4. Cai dat / go bo chuong trinh"
    echo "0. Thoat"
    echo "##################################################"
    read -p "Chon chuong trinh: " c
    case $c in
        1) bash "$THUMUC/quanly_file.sh" ;;
        2) bash "$THUMUC/laplich_tacvu.sh" ;;
        3) bash "$THUMUC/thoigian_hethong.sh" ;;
        4) bash "$THUMUC/caidat_goibo.sh" ;;
        0) echo "Tam biet!"; break ;;
        *) echo "Lua chon khong hop le." ;;
    esac
done
