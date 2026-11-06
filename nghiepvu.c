#include "khachsan.h"

/* ===================== DAT PHONG / CHECK-IN / CHECK-OUT ===================== */

static int ngayHopLe(int d, int m, int y) {
    int ngayThang[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (y < 2000 || m < 1 || m > 12 || d < 1) return 0;
    if (m == 2 && ((y % 4 == 0 && y % 100 != 0) || y % 400 == 0))
        return d <= 29;
    return d <= ngayThang[m - 1];
}

void datPhong() {
    int ma = nhapSoNguyen("Nhap ma phong muon dat (nhap 0 de quay lai): ");
    Phong *p;
    KhachHang *k;
    int cachTinh;

    if (ma == 0) return;
    p = timPhong(ma);

    if (p == NULL) {
        printf("Ma phong khong ton tai!\n");
        return;
    }
    if (p->trangThai != TRONG) {
        printf("Phong nay da duoc dat hoac dang co khach!\n");
        return;
    }

    k = &p->khach;
    if (!nhapChuoi("Ten khach hang (nhap 0 de quay lai): ", k->ten, sizeof(k->ten)) ||
        !nhapChuoi("So dien thoai (nhap 0 de quay lai): ", k->sdt, sizeof(k->sdt))) {
        xoaKhach(p);
        return;
    }

    do {
        k->ngay = nhapSoNguyen("Ngay nhan phong (ngay): ");
        k->thang = nhapSoNguyen("Ngay nhan phong (thang): ");
        k->nam = nhapSoNguyen("Ngay nhan phong (nam): ");
        if (!ngayHopLe(k->ngay, k->thang, k->nam))
            printf("Ngay khong hop le, nhap lai!\n");
    } while (!ngayHopLe(k->ngay, k->thang, k->nam));

    do {
        cachTinh = nhapSoNguyen("Tinh tien theo (1: ngay, 2: gio): ");
    } while (cachTinh != THEO_NGAY && cachTinh != THEO_GIO);
    k->kieuTinh = cachTinh;

    do {
        k->soLuong = nhapSoNguyen(cachTinh == THEO_NGAY
                                  ? "So ngay du kien o: "
                                  : "So gio du kien o: ");
    } while (k->soLuong <= 0);

    k->phuPhi = 0;
    p->trangThai = DA_DAT;
    printf("Dat phong %d thanh cong! Vui long den quay le tan de nhan phong.\n", ma);
}

void checkIn() {
    int ma = nhapSoNguyen("Nhap ma phong can check-in: ");
    Phong *p = timPhong(ma);

    if (p == NULL) {
        printf("Ma phong khong ton tai!\n");
        return;
    }
    if (p->trangThai == DA_DAT) {
        p->trangThai = DANG_O;
        printf("Check-in phong %d thanh cong! Khach: %s\n", ma, p->khach.ten);
    } else if (p->trangThai == TRONG) {
        printf("Phong chua duoc dat!\n");
    } else {
        printf("Phong dang co khach!\n");
    }
}

void themDichVu() {
    int ma = nhapSoNguyen("Nhap ma phong su dung dich vu: ");
    Phong *p = timPhong(ma);
    float tien;

    if (p == NULL) {
        printf("Ma phong khong ton tai!\n");
        return;
    }
    if (p->trangThai != DANG_O) {
        printf("Chi them dich vu cho phong dang co khach!\n");
        return;
    }
    tien = nhapSoThuc("Nhap tien dich vu (an uong, giat ui...): ");
    p->khach.phuPhi += tien;
    printf("Da them. Tong phu phi hien tai: %.0f VND\n", p->khach.phuPhi);
}

static float tinhTienPhong(const Phong *p, int soLuongThuc) {
    if (p->khach.kieuTinh == THEO_NGAY)
        return soLuongThuc * p->giaNgay;
    return soLuongThuc * p->giaGio;
}

static void inHoaDon(const Phong *p, int soLuongThuc) {
    float tienPhong = tinhTienPhong(p, soLuongThuc);
    const char *donVi = p->khach.kieuTinh == THEO_NGAY ? "ngay" : "gio";
    float donGia = p->khach.kieuTinh == THEO_NGAY ? p->giaNgay : p->giaGio;

    printf("\n==============================\n");
    printf("        HOA DON THANH TOAN\n");
    printf("==============================\n");
    printf("Khach hang : %s\n", p->khach.ten);
    printf("SDT        : %s\n", p->khach.sdt);
    printf("Phong      : %d (%s)\n", p->maPhong, p->loaiPhong);
    printf("Ngay nhan  : %02d/%02d/%04d\n", p->khach.ngay, p->khach.thang, p->khach.nam);
    printf("------------------------------\n");
    printf("Tien phong : %d %s x %.0f = %.0f\n", soLuongThuc, donVi, donGia, tienPhong);
    printf("Phu phi DV : %.0f\n", p->khach.phuPhi);
    printf("------------------------------\n");
    printf("TONG CONG  : %.0f VND\n", tienPhong + p->khach.phuPhi);
    printf("==============================\n");
}

void checkOut() {
    int ma = nhapSoNguyen("Nhap ma phong can check-out: ");
    Phong *p = timPhong(ma);
    int soLuongThuc;

    if (p == NULL) {
        printf("Ma phong khong ton tai!\n");
        return;
    }
    if (p->trangThai == DA_DAT) {
        printf("Phong da dat nhung chua check-in!\n");
        return;
    }
    if (p->trangThai == TRONG) {
        printf("Phong dang trong, khong the check-out!\n");
        return;
    }

    do {
        soLuongThuc = nhapSoNguyen(p->khach.kieuTinh == THEO_NGAY
                                   ? "So ngay thuc te da o: "
                                   : "So gio thuc te da o: ");
    } while (soLuongThuc <= 0);

    inHoaDon(p, soLuongThuc);
    ghiDoanhThu(p, tinhTienPhong(p, soLuongThuc));

    p->trangThai = TRONG;
    xoaKhach(p);
    printf("Check-out phong %d thanh cong! Phong hien da trong.\n", ma);
}
