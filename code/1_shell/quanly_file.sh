#!/bin/bash
# =============================================================
# Phan 1 - Lap trinh shell: QUAN LY FILE / THU MUC
# Chuc nang: tao, doi ten, sao chep, di chuyen, xoa, dieu huong,
#            liet ke, phan quyen, nen thu muc/file.
# Cach chay:  chmod +x quanly_file.sh && ./quanly_file.sh
# =============================================================

# Tao moi thu muc
tao_thumuc() {
    read -p "Nhap ten thu muc moi: " ten
    if mkdir -p "$ten"; then
        echo "[OK] Da tao thu muc '$ten'."
    else
        echo "[Loi] Khong the tao thu muc '$ten'."
    fi
}

# Tao moi file rong
tao_file() {
    read -p "Nhap ten file moi: " ten
    if touch "$ten"; then
        echo "[OK] Da tao file '$ten'."
    else
        echo "[Loi] Khong the tao file '$ten'."
    fi
}

# Doi ten file/thu muc
doi_ten() {
    read -p "Nhap ten cu: " cu
    read -p "Nhap ten moi: " moi
    if mv "$cu" "$moi"; then
        echo "[OK] Da doi '$cu' thanh '$moi'."
    else
        echo "[Loi] Khong the doi ten."
    fi
}

# Sao chep
sao_chep() {
    read -p "Nhap nguon: " nguon
    read -p "Nhap dich: " dich
    if cp -r "$nguon" "$dich"; then
        echo "[OK] Da sao chep '$nguon' -> '$dich'."
    else
        echo "[Loi] Khong the sao chep."
    fi
}

# Di chuyen
di_chuyen() {
    read -p "Nhap nguon: " nguon
    read -p "Nhap dich: " dich
    if mv "$nguon" "$dich"; then
        echo "[OK] Da di chuyen '$nguon' -> '$dich'."
    else
        echo "[Loi] Khong the di chuyen."
    fi
}

# Xoa
xoa() {
    read -p "Nhap ten can xoa: " ten
    read -p "Ban chac chan muon xoa '$ten'? (y/n): " xn
    if [ "$xn" = "y" ]; then
        if rm -rf "$ten"; then
            echo "[OK] Da xoa '$ten'."
        else
            echo "[Loi] Khong the xoa."
        fi
    else
        echo "Da huy thao tac xoa."
    fi
}

# Liet ke noi dung thu muc hien tai
liet_ke() {
    read -p "Hien thi chi tiet? (y/n): " ct
    if [ "$ct" = "y" ]; then
        ls -lah
    else
        ls
    fi
}

# Dieu huong thu muc
dieu_huong() {
    read -p "Nhap duong dan can chuyen den (.. = thu muc cha): " dd
    if cd "$dd"; then
        echo "[OK] Dang o: $(pwd)"
    else
        echo "[Loi] Khong the chuyen den '$dd'."
    fi
}

# Phan quyen
phan_quyen() {
    read -p "Nhap ten file/thu muc: " ten
    read -p "Nhap quyen (vd: 755): " q
    if chmod "$q" "$ten"; then
        echo "[OK] Da cap quyen $q cho '$ten'."
    else
        echo "[Loi] Khong the cap quyen."
    fi
}

# Nen file/thu muc
nen() {
    read -p "Nhap ten file/thu muc can nen: " ten
    if tar -zcvf "${ten}.tar.gz" "$ten"; then
        echo "[OK] Da nen thanh '${ten}.tar.gz'."
    else
        echo "[Loi] Khong the nen."
    fi
}

# Menu chinh
while true; do
    echo
    echo "========== QUAN LY FILE =========="
    echo "1. Tao thu muc"
    echo "2. Tao file"
    echo "3. Doi ten"
    echo "4. Sao chep"
    echo "5. Di chuyen"
    echo "6. Xoa"
    echo "7. Liet ke thu muc hien tai"
    echo "8. Dieu huong thu muc"
    echo "9. Phan quyen (chmod)"
    echo "10. Nen file/thu muc"
    echo "0. Thoat"
    echo "=================================="
    read -p "Chon chuc nang: " c
    case $c in
        1) tao_thumuc ;;
        2) tao_file ;;
        3) doi_ten ;;
        4) sao_chep ;;
        5) di_chuyen ;;
        6) xoa ;;
        7) liet_ke ;;
        8) dieu_huong ;;
        9) phan_quyen ;;
        10) nen ;;
        0) echo "Ket thuc chuong trinh."; break ;;
        *) echo "Lua chon khong hop le." ;;
    esac
done
