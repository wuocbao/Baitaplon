#include "khachsan.h"

Phong phong[HANG][COT];

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
