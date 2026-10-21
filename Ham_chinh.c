#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HANG 5
#define COT 5
#define TONG (HANG * COT)
#define FILE_DATA "phong.txt"

/* Trang thai phong */
#define TRONG 0
#define DA_DAT 1
#define DANG_O 2

/* Cach tinh tien */
#define THEO_NGAY 1
#define THEO_GIO 2

typedef struct {
    char ten[50];
    char sdt[15];
    int ngay, thang, nam;   /* ngay nhan phong du kien */
    int kieuTinh;           /* 1: theo ngay, 2: theo gio */
    int soLuong;            /* so ngay hoac so gio */
    float phuPhi;           /* tien dich vu */
} KhachHang;

typedef struct {
    int maPhong;
    char loaiPhong[20];
    float giaNgay;
    float giaGio;
    int trangThai;
    KhachHang khach;
} Phong;

Phong phong[HANG][COT];

/* ===================== HAM TIEN ICH NHAP LIEU ===================== */

void boXuongDong(char *s) {
    s[strcspn(s, "\r\n")] = '\0';
}

int nhapSoNguyen(const char *loiNhac) {
    char buf[64];
    int x;
    while (1) {
        printf("%s", loiNhac);
        if (fgets(buf, sizeof(buf), stdin) == NULL) return -1;
        if (sscanf(buf, "%d", &x) == 1) return x;
        printf("Vui long nhap so nguyen!\n");
    }
}

float nhapSoThuc(const char *loiNhac) {
    char buf[64];
    float x;
    while (1) {
        printf("%s", loiNhac);
        if (fgets(buf, sizeof(buf), stdin) == NULL) return 0;
        if (sscanf(buf, "%f", &x) == 1 && x >= 0) return x;
        printf("Vui long nhap so hop le!\n");
    }
}

void nhapChuoi(const char *loiNhac, char *s, int size) {
    do {
        printf("%s", loiNhac);
        if (fgets(s, size, stdin) == NULL) { s[0] = '\0'; return; }
        boXuongDong(s);
        if (s[0] == '\0') printf("Khong duoc de trong!\n");
    } while (s[0] == '\0');
}

/* ===================== TIEN ICH PHONG ===================== */

const char *tenTrangThai(int tt) {
    switch (tt) {
        case TRONG:  return "Trong";
        case DA_DAT: return "Da dat";
        case DANG_O: return "Dang o";
    }
    return "?";
}

Phong *timPhong(int ma) {
    int i, j;
    for (i = 0; i < HANG; i++)
        for (j = 0; j < COT; j++)
            if (phong[i][j].maPhong == ma) return &phong[i][j];
    return NULL;
}

void xoaKhach(Phong *p) {
    memset(&p->khach, 0, sizeof(KhachHang));
}

void inTieuDe() {
    printf("%-6s %-8s %-10s %-10s %-8s %-18s\n",
           "Ma", "Loai", "Gia/ngay", "Gia/gio", "T.thai", "Khach");
    printf("-----------------------------------------------------------------\n");
}

void inMotPhong(const Phong *p) {
    printf("%-6d %-8s %-10.0f %-10.0f %-8s %-18s\n",
           p->maPhong, p->loaiPhong, p->giaNgay, p->giaGio,
           tenTrangThai(p->trangThai),
           p->trangThai == TRONG ? "-" : p->khach.ten);
}

/* ===================== KHOI TAO ===================== */

void khoiTaoPhong() {
    int i, j;
    for (i = 0; i < HANG; i++) {
        for (j = 0; j < COT; j++) {
            Phong *p = &phong[i][j];
            p->maPhong = i * COT + j + 1;
            if (i < 3) {                  /* hang 0-2: phong thuong */
                strcpy(p->loaiPhong, "Thuong");
                p->giaNgay = 300000;
                p->giaGio = 60000;
            } else {                      /* hang 3-4: phong VIP */
                strcpy(p->loaiPhong, "VIP");
                p->giaNgay = 600000;
                p->giaGio = 120000;
            }
            p->trangThai = TRONG;
            xoaKhach(p);
        }
    }
}

/* ===================== DOC / GHI FILE ===================== */
/* Moi dong: ma|loai|giaNgay|giaGio|trangThai|ten|sdt|ngay|thang|nam|kieuTinh|soLuong|phuPhi */

void ghiFile() {
    FILE *f = fopen(FILE_DATA, "w");
    int i, j;
    if (f == NULL) {
        printf("Khong mo duoc file de ghi!\n");
        return;
    }
    for (i = 0; i < HANG; i++) {
        for (j = 0; j < COT; j++) {
            Phong *p = &phong[i][j];
            fprintf(f, "%d|%s|%.0f|%.0f|%d|%s|%s|%d|%d|%d|%d|%d|%.0f\n",
                    p->maPhong, p->loaiPhong, p->giaNgay, p->giaGio, p->trangThai,
                    p->khach.ten[0] ? p->khach.ten : "-",
                    p->khach.sdt[0] ? p->khach.sdt : "-",
                    p->khach.ngay, p->khach.thang, p->khach.nam,
                    p->khach.kieuTinh, p->khach.soLuong, p->khach.phuPhi);
        }
    }
    fclose(f);
    printf("Da luu du lieu vao file %s\n", FILE_DATA);
}

int docFile() {
    FILE *f = fopen(FILE_DATA, "r");
    int i, j;
    if (f == NULL) return 0;   /* chua co file -> dung du lieu mac dinh */

    for (i = 0; i < HANG; i++) {
        for (j = 0; j < COT; j++) {
            Phong *p = &phong[i][j];
            int n = fscanf(f,
                "%d|%19[^|]|%f|%f|%d|%49[^|]|%14[^|]|%d|%d|%d|%d|%d|%f\n",
                &p->maPhong, p->loaiPhong, &p->giaNgay, &p->giaGio, &p->trangThai,
                p->khach.ten, p->khach.sdt,
                &p->khach.ngay, &p->khach.thang, &p->khach.nam,
                &p->khach.kieuTinh, &p->khach.soLuong, &p->khach.phuPhi);
            if (n != 13) {
                fclose(f);
                printf("File du lieu bi loi, dung du lieu mac dinh.\n");
                khoiTaoPhong();
                return 0;
            }
            if (strcmp(p->khach.ten, "-") == 0) p->khach.ten[0] = '\0';
            if (strcmp(p->khach.sdt, "-") == 0) p->khach.sdt[0] = '\0';
        }
    }
    fclose(f);
    printf("Da doc du lieu tu file %s\n", FILE_DATA);
    return 1;
}

/* ===================== HIEN THI / TIM KIEM ===================== */

void hienThiPhong() {
    int i, j;
    printf("\n===== DANH SACH PHONG =====\n");
    inTieuDe();
    for (i = 0; i < HANG; i++)
        for (j = 0; j < COT; j++)
            inMotPhong(&phong[i][j]);
}

void hienThiPhongTrong() {
    int i, j, co = 0;
    printf("\n===== DANH SACH PHONG TRONG =====\n");
    inTieuDe();
    for (i = 0; i < HANG; i++)
        for (j = 0; j < COT; j++)
            if (phong[i][j].trangThai == TRONG) {
                inMotPhong(&phong[i][j]);
                co = 1;
            }
    if (!co) printf("Khong co phong trong!\n");
}

void timPhongTheoLoai() {
    char loai[20];
    int i, j, co = 0;
    nhapChuoi("Nhap loai phong (Thuong/VIP): ", loai, sizeof(loai));
    printf("\n===== PHONG TRONG LOAI %s =====\n", loai);
    inTieuDe();
    for (i = 0; i < HANG; i++)
        for (j = 0; j < COT; j++) {
            Phong *p = &phong[i][j];
            if (p->trangThai == TRONG && strcmp(p->loaiPhong, loai) == 0) {
                inMotPhong(p);
                co = 1;
            }
        }
    if (!co) printf("Khong tim thay phong trong loai nay!\n");
}

void timPhongTheoGia() {
    float giaMax = nhapSoThuc("Nhap gia toi da moi ngay (VND): ");
    int i, j, co = 0;
    printf("\n===== PHONG TRONG GIA <= %.0f =====\n", giaMax);
    inTieuDe();
    for (i = 0; i < HANG; i++)
        for (j = 0; j < COT; j++) {
            Phong *p = &phong[i][j];
            if (p->trangThai == TRONG && p->giaNgay <= giaMax) {
                inMotPhong(p);
                co = 1;
            }
        }
    if (!co) printf("Khong co phong trong nao trong muc gia nay!\n");
}

/* ===================== SAP XEP ===================== */

int khoaNgay(const Phong *p) {
    return p->khach.nam * 10000 + p->khach.thang * 100 + p->khach.ngay;
}

int soSanhNgay(const void *a, const void *b) {
    const Phong *p = *(const Phong **)a;
    const Phong *q = *(const Phong **)b;
    return khoaNgay(p) - khoaNgay(q);
}

int soSanhGia(const void *a, const void *b) {
    const Phong *p = *(const Phong **)a;
    const Phong *q = *(const Phong **)b;
    if (p->giaNgay < q->giaNgay) return -1;
    if (p->giaNgay > q->giaNgay) return 1;
    return p->maPhong - q->maPhong;
}

void sapXep() {
    Phong *ds[TONG];
    int n = 0, i, j, chon;

    printf("\n1. Sap xep phong dat/dang o theo ngay nhan\n");
    printf("2. Sap xep tat ca phong theo gia\n");
    chon = nhapSoNguyen("Chon: ");

    if (chon == 1) {
        for (i = 0; i < HANG; i++)
            for (j = 0; j < COT; j++)
                if (phong[i][j].trangThai != TRONG) ds[n++] = &phong[i][j];
        if (n == 0) {
            printf("Chua co phong nao duoc dat!\n");
            return;
        }
        qsort(ds, n, sizeof(Phong *), soSanhNgay);
        printf("\n===== SAP XEP THEO NGAY NHAN =====\n");
        printf("%-6s %-18s %-12s %-8s\n", "Ma", "Khach", "Ngay nhan", "T.thai");
        for (i = 0; i < n; i++)
            printf("%-6d %-18s %02d/%02d/%04d   %-8s\n",
                   ds[i]->maPhong, ds[i]->khach.ten,
                   ds[i]->khach.ngay, ds[i]->khach.thang, ds[i]->khach.nam,
                   tenTrangThai(ds[i]->trangThai));
    } else if (chon == 2) {
        for (i = 0; i < HANG; i++)
            for (j = 0; j < COT; j++)
                ds[n++] = &phong[i][j];
        qsort(ds, n, sizeof(Phong *), soSanhGia);
        printf("\n===== SAP XEP THEO GIA =====\n");
        inTieuDe();
        for (i = 0; i < n; i++) inMotPhong(ds[i]);
    } else {
        printf("Lua chon khong hop le!\n");
    }
}

/* ===================== DAT PHONG / CHECK-IN / CHECK-OUT ===================== */

int ngayHopLe(int d, int m, int y) {
    int ngayThang[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (y < 2000 || m < 1 || m > 12 || d < 1) return 0;
    if (m == 2 && ((y % 4 == 0 && y % 100 != 0) || y % 400 == 0))
        return d <= 29;
    return d <= ngayThang[m - 1];
}

void datPhong() {
    int ma = nhapSoNguyen("Nhap ma phong muon dat: ");
    Phong *p = timPhong(ma);
    KhachHang *k;
    int cachTinh;

    if (p == NULL) {
        printf("Ma phong khong ton tai!\n");
        return;
    }
    if (p->trangThai != TRONG) {
        printf("Phong nay da duoc dat hoac dang co khach!\n");
        return;
    }

    k = &p->khach;
    nhapChuoi("Ten khach hang: ", k->ten, sizeof(k->ten));
    nhapChuoi("So dien thoai: ", k->sdt, sizeof(k->sdt));

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
    printf("Dat phong %d thanh cong!\n", ma);
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

float tinhTienPhong(const Phong *p, int soLuongThuc) {
    if (p->khach.kieuTinh == THEO_NGAY)
        return soLuongThuc * p->giaNgay;
    return soLuongThuc * p->giaGio;
}

void inHoaDon(const Phong *p, int soLuongThuc) {
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

    p->trangThai = TRONG;
    xoaKhach(p);
    printf("Check-out phong %d thanh cong! Phong hien da trong.\n", ma);
}

/* ===================== MENU (CON TRO HAM) ===================== */

typedef void (*HamMenu)(void);

typedef struct {
    const char *ten;
    HamMenu ham;
} MucMenu;

MucMenu menu[] = {
    {"Hien thi tat ca phong",          hienThiPhong},
    {"Tim phong trong",                hienThiPhongTrong},
    {"Tim phong trong theo loai",      timPhongTheoLoai},
    {"Tim phong trong theo gia",       timPhongTheoGia},
    {"Sap xep (theo ngay / theo gia)", sapXep},
    {"Dat phong",                      datPhong},
    {"Check-in",                       checkIn},
    {"Them dich vu (phu phi)",         themDichVu},
    {"Check-out & in hoa don",         checkOut},
    {"Luu du lieu ra file",            ghiFile}
};

#define SO_MUC (int)(sizeof(menu) / sizeof(menu[0]))

void hienMenu() {
    int i;
    printf("\n===== QUAN LY KHACH SAN =====\n");
    for (i = 0; i < SO_MUC; i++)
        printf("%d. %s\n", i + 1, menu[i].ten);
    printf("0. Thoat (tu dong luu)\n");
}

int main() {
    int chon;

    khoiTaoPhong();
    docFile();

    do {
        hienMenu();
        chon = nhapSoNguyen("Nhap lua chon: ");

        if (chon == -1 || chon == 0) {
            ghiFile();
            printf("Ket thuc chuong trinh\n");
            break;
        }
        if (chon >= 1 && chon <= SO_MUC)
            menu[chon - 1].ham();
        else
            printf("Lua chon khong hop le!\n");
    } while (1);

    return 0;
}
