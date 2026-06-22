#!/bin/bash
# =============================================================
# Phan 1 - Lap trinh shell: LAP LICH TAC VU (dung crontab)
# Chuc nang: liet ke, them, sua, xoa cong viec dinh ky.
# Cach chay:  chmod +x laplich_tacvu.sh && ./laplich_tacvu.sh
# Ghi chu cu phap crontab:  phut gio ngay thang thu  lenh
#   vi du:  */5 * * * * echo hello >> /tmp/log.txt
# =============================================================

# Liet ke cac tac vu da lap lich
liet_ke() {
    echo "----- Danh sach tac vu hien tai -----"
    crontab -l 2>/dev/null || echo "(Chua co tac vu nao)"
}

# Them mot tac vu moi
them() {
    read -p "Nhap lich (vd: */5 * * * * lenh): " lich
    (crontab -l 2>/dev/null; echo "$lich") | crontab -
    echo "[OK] Da them tac vu."
}

# Xoa mot tac vu theo so thu tu dong
xoa() {
    liet_ke
    read -p "Nhap so thu tu dong can xoa: " stt
    if ! [[ "$stt" =~ ^[1-9][0-9]*$ ]]; then
        echo "[Loi] So thu tu khong hop le."
        return
    fi
    tmp=$(mktemp)
    crontab -l 2>/dev/null > "$tmp"
    if [ "$stt" -gt "$(wc -l < "$tmp")" ]; then
        echo "[Loi] Khong ton tai dong $stt."
        rm "$tmp"; return
    fi
    sed -i "${stt}d" "$tmp"
    crontab "$tmp"
    rm "$tmp"
    echo "[OK] Da xoa tac vu dong $stt."
}

# Xoa toan bo tac vu
xoa_het() {
    read -p "Xoa TAT CA tac vu? (y/n): " xn
    if [ "$xn" = "y" ]; then
        crontab -r 2>/dev/null
        echo "[OK] Da xoa toan bo tac vu."
    fi
}

# Menu chinh
while true; do
    echo
    echo "========== LAP LICH TAC VU =========="
    echo "1. Liet ke tac vu"
    echo "2. Them tac vu"
    echo "3. Xoa mot tac vu"
    echo "4. Xoa toan bo tac vu"
    echo "0. Thoat"
    echo "===================================="
    read -p "Chon chuc nang: " c
    case $c in
        1) liet_ke ;;
        2) them ;;
        3) xoa ;;
        4) xoa_het ;;
        0) echo "Ket thuc chuong trinh."; break ;;
        *) echo "Lua chon khong hop le." ;;
    esac
done
