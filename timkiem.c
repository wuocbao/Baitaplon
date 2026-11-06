#include "khachsan.h"

/* ===================== HIEN THI / TIM KIEM ===================== */

void hienThiPhong() {
    int i, j;
    printf("\n===== DANH SACH PHONG =====\n");
    inTieuDe();
    for (i = 0; i < HANG; i++)
        for (j = 0; j < COT; j++)
            inMotPhong(&phong[i][j]);
    choQuayLai();
}

/* Danh sach phong cho khach xem: KHONG hien ten khach khac (bao mat rieng tu) */
void hienThiPhongKhach() {
    int i, j;
    printf("\n===== DANH SACH PHONG =====\n");
    printf("%-6s %-8s %-10s %-10s %-8s\n", "Ma", "Loai", "Gia/ngay", "Gia/gio", "T.thai");
    printf("-----------------------------------------------\n");
    for (i = 0; i < HANG; i++)
        for (j = 0; j < COT; j++) {
            const Phong *p = &phong[i][j];
            printf("%-6d %-8s %-10.0f %-10.0f %-8s\n",
                   p->maPhong, p->loaiPhong, p->giaNgay, p->giaGio, tenTrangThai(p->trangThai));
        }
    choQuayLai();
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
    choQuayLai();
}

void timPhongTheoLoai() {
    int chon, i, j, co = 0;
    const char *loai;

    printf("\n===== TIM PHONG TRONG THEO LOAI =====\n");
    printf("1. Phong Thuong\n2. Phong VIP\n0. Quay lai\n");
    chon = nhapLuaChon("Chon loai phong: ", 2);   /* nhap sai -> bao khong hop le, nhap lai */
    if (chon == 0) return;
    loai = (chon == 1) ? "Thuong" : "VIP";

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
    choQuayLai();
}

void timPhongTheoGia() {
    float giaMax;
    int i, j, co = 0;

    printf("\n===== TIM PHONG TRONG THEO GIA =====\n");
    giaMax = nhapSoThuc("Nhap gia toi da moi ngay (VND), nhap 0 de quay lai: ");
    if (giaMax == 0) return;

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
    choQuayLai();
}

/* ===================== SAP XEP ===================== */

static int khoaNgay(const Phong *p) {
    return p->khach.nam * 10000 + p->khach.thang * 100 + p->khach.ngay;
}

static int soSanhNgay(const void *a, const void *b) {
    const Phong *p = *(const Phong **)a;
    const Phong *q = *(const Phong **)b;
    return khoaNgay(p) - khoaNgay(q);
}

static int soSanhGia(const void *a, const void *b) {
    const Phong *p = *(const Phong **)a;
    const Phong *q = *(const Phong **)b;
    if (p->giaNgay < q->giaNgay) return -1;
    if (p->giaNgay > q->giaNgay) return 1;
    return p->maPhong - q->maPhong;
}

void sapXep() {
    Phong *ds[TONG];
    int n = 0, i, j, chon;

    printf("\n===== SAP XEP =====\n");
    printf("1. Phong dat/dang o theo ngay nhan\n");
    printf("2. Tat ca phong theo gia\n");
    printf("0. Quay lai\n");
    chon = nhapLuaChon("Chon: ", 2);
    if (chon == 0) return;

    if (chon == 1) {
        for (i = 0; i < HANG; i++)
            for (j = 0; j < COT; j++)
                if (phong[i][j].trangThai != TRONG) ds[n++] = &phong[i][j];
        if (n == 0) {
            printf("Chua co phong nao duoc dat!\n");
        } else {
            qsort(ds, n, sizeof(Phong *), soSanhNgay);
            printf("\n===== SAP XEP THEO NGAY NHAN =====\n");
            printf("%-6s %-18s %-12s %-8s\n", "Ma", "Khach", "Ngay nhan", "T.thai");
            for (i = 0; i < n; i++)
                printf("%-6d %-18s %02d/%02d/%04d   %-8s\n",
                       ds[i]->maPhong, ds[i]->khach.ten,
                       ds[i]->khach.ngay, ds[i]->khach.thang, ds[i]->khach.nam,
                       tenTrangThai(ds[i]->trangThai));
        }
    } else {
        for (i = 0; i < HANG; i++)
            for (j = 0; j < COT; j++)
                ds[n++] = &phong[i][j];
        qsort(ds, n, sizeof(Phong *), soSanhGia);
        printf("\n===== SAP XEP THEO GIA =====\n");
        inTieuDe();
        for (i = 0; i < n; i++) inMotPhong(ds[i]);
    }
    choQuayLai();
}
