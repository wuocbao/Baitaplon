#include "khachsan.h"

/* ===================== DOC / GHI FILE (DANG BANG) ===================== */

static int tachTruong(char *dong, char *truong[], int toiDa) {
    int n = 0;
    char *p = dong;
    while (n < toiDa) {
        truong[n++] = p;
        p = strchr(p, '|');
        if (p == NULL) break;
        *p = '\0';
        p++;
    }
    return n;
}

void ghiFile() {
    FILE *f = fopen(FILE_DATA, "w");
    int i, j;
    if (f == NULL) {
        printf("Khong mo duoc file de ghi!\n");
        return;
    }
    fprintf(f, "%-5s | %-8s | %-10s | %-10s | %-8s | %-20s | %-12s | %-10s | %-6s | %-8s | %-10s\n",
            "Ma", "Loai", "Gia/ngay", "Gia/gio", "T.thai", "Ten khach", "SDT",
            "Ngay nhan", "Tinh", "So luong", "Phu phi");
    fprintf(f, "------------------------------------------------------------------------------------------------------------------------\n");
    for (i = 0; i < HANG; i++) {
        for (j = 0; j < COT; j++) {
            Phong *p = &phong[i][j];
            char ngay[16];
            if (p->khach.nam > 0)
                sprintf(ngay, "%02d/%02d/%04d", p->khach.ngay, p->khach.thang, p->khach.nam);
            else
                strcpy(ngay, "-");
            fprintf(f, "%-5d | %-8s | %-10.0f | %-10.0f | %-8s | %-20s | %-12s | %-10s | %-6s | %-8d | %-10.0f\n",
                    p->maPhong, p->loaiPhong, p->giaNgay, p->giaGio,
                    tenTrangThai(p->trangThai),
                    p->khach.ten[0] ? p->khach.ten : "-",
                    p->khach.sdt[0] ? p->khach.sdt : "-",
                    ngay,
                    p->khach.kieuTinh == THEO_NGAY ? "Ngay" : (p->khach.kieuTinh == THEO_GIO ? "Gio" : "-"),
                    p->khach.soLuong, p->khach.phuPhi);
        }
    }
    fclose(f);
    printf("Da luu du lieu vao file %s\n", FILE_DATA);
}

int docFile() {
    FILE *f = fopen(FILE_DATA, "r");
    char dong[512], *t[11];
    int i, j, loi = 0;
    if (f == NULL) return 0;   /* chua co file -> dung du lieu mac dinh */

    /* bo qua 2 dong tieu de */
    if (fgets(dong, sizeof(dong), f) == NULL || fgets(dong, sizeof(dong), f) == NULL) loi = 1;

    for (i = 0; i < HANG && !loi; i++) {
        for (j = 0; j < COT && !loi; j++) {
            Phong *p = &phong[i][j];
            int k, d = 0, m = 0, y = 0;
            if (fgets(dong, sizeof(dong), f) == NULL || tachTruong(dong, t, 11) != 11) {
                loi = 1;
                break;
            }
            for (k = 0; k < 11; k++) catKhoang(t[k]);

            p->maPhong = atoi(t[0]);
            strncpy(p->loaiPhong, t[1], sizeof(p->loaiPhong) - 1);
            p->loaiPhong[sizeof(p->loaiPhong) - 1] = '\0';
            p->giaNgay = (float)atof(t[2]);
            p->giaGio = (float)atof(t[3]);

            if (strcmp(t[4], "Da dat") == 0) p->trangThai = DA_DAT;
            else if (strcmp(t[4], "Dang o") == 0) p->trangThai = DANG_O;
            else p->trangThai = TRONG;

            xoaKhach(p);
            if (strcmp(t[5], "-") != 0) strncpy(p->khach.ten, t[5], sizeof(p->khach.ten) - 1);
            if (strcmp(t[6], "-") != 0) strncpy(p->khach.sdt, t[6], sizeof(p->khach.sdt) - 1);
            if (sscanf(t[7], "%d/%d/%d", &d, &m, &y) == 3) {
                p->khach.ngay = d; p->khach.thang = m; p->khach.nam = y;
            }
            if (strcmp(t[8], "Ngay") == 0) p->khach.kieuTinh = THEO_NGAY;
            else if (strcmp(t[8], "Gio") == 0) p->khach.kieuTinh = THEO_GIO;
            p->khach.soLuong = atoi(t[9]);
            p->khach.phuPhi = (float)atof(t[10]);
        }
    }
    fclose(f);
    if (loi) {
        printf("File du lieu bi loi, dung du lieu mac dinh.\n");
        khoiTaoPhong();
        return 0;
    }
    printf("Da doc du lieu tu file %s\n", FILE_DATA);
    return 1;
}
