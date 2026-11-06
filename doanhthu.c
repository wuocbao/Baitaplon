#include "khachsan.h"

/* ===================== DOANH THU =====================
 * Moi lan check-out ghi them 1 dong vao doanhthu.txt:
 *   dd/mm/yyyy hh:mm|ma phong|loai|ten khach|tien phong|phu phi|tong|nhan vien
 */

void ghiDoanhThu(const Phong *p, float tienPhong) {
    FILE *f = fopen(FILE_DOANHTHU, "a");
    time_t now = time(NULL);
    struct tm *tm = localtime(&now);
    char ten[50];
    char *c;

    if (f == NULL) {
        printf("Canh bao: khong ghi duoc file doanh thu!\n");
        return;
    }
    snprintf(ten, sizeof(ten), "%s", p->khach.ten);
    while ((c = strchr(ten, '|')) != NULL) *c = ' ';

    fprintf(f, "%02d/%02d/%04d %02d:%02d|%d|%s|%s|%.0f|%.0f|%.0f|%s\n",
            tm->tm_mday, tm->tm_mon + 1, tm->tm_year + 1900, tm->tm_hour, tm->tm_min,
            p->maPhong, p->loaiPhong, ten,
            tienPhong, p->khach.phuPhi, tienPhong + p->khach.phuPhi,
            nguoiDung ? nguoiDung->user : "-");
    fclose(f);
}

/* Chi quan ly moi co muc nay trong menu */
void xemDoanhThu(void) {
    FILE *f;
    char dong[256];
    int chon, d = 0, m = 0, y = 0, n = 0;
    double tongPhong = 0, tongPhu = 0, tong = 0;

    printf("\n1. Doanh thu theo ngay\n2. Doanh thu theo thang\n");
    printf("3. Doanh thu theo nam\n4. Tong doanh thu\n");
    chon = nhapSoNguyen("Chon: ");
    if (chon < 1 || chon > 4) {
        printf("Lua chon khong hop le!\n");
        return;
    }
    if (chon == 1) d = nhapSoNguyen("Ngay: ");
    if (chon == 1 || chon == 2) m = nhapSoNguyen("Thang: ");
    if (chon <= 3) y = nhapSoNguyen("Nam: ");

    f = fopen(FILE_DOANHTHU, "r");
    if (f == NULL) {
        printf("Chua co du lieu doanh thu!\n");
        return;
    }

    while (fgets(dong, sizeof(dong), f) != NULL) {
        int dd, mm, yy, hh, mi, ma;
        char loai[20], ten[50], nv[20];
        double tp, pp, t;
        if (sscanf(dong, "%d/%d/%d %d:%d|%d|%19[^|]|%49[^|]|%lf|%lf|%lf|%19[^\n]",
                   &dd, &mm, &yy, &hh, &mi, &ma, loai, ten, &tp, &pp, &t, nv) != 12) continue;
        if (chon == 1 && (dd != d || mm != m || yy != y)) continue;
        if (chon == 2 && (mm != m || yy != y)) continue;
        if (chon == 3 && yy != y) continue;

        if (n == 0) {
            printf("\n===== HOA DON DA THANH TOAN =====\n");
            printf("%-17s %-5s %-8s %-18s %12s %10s %12s  %s\n",
                   "Thoi gian", "Phong", "Loai", "Khach", "Tien phong", "Phu phi", "Tong", "NV");
            printf("---------------------------------------------------------------------------------------------\n");
        }
        printf("%02d/%02d/%04d %02d:%02d %-5d %-8s %-18s %12.0f %10.0f %12.0f  %s\n",
               dd, mm, yy, hh, mi, ma, loai, ten, tp, pp, t, nv);
        tongPhong += tp;
        tongPhu += pp;
        tong += t;
        n++;
    }
    fclose(f);

    if (n == 0) {
        printf("Khong co hoa don nao trong khoang thoi gian nay!\n");
        return;
    }
    printf("---------------------------------------------------------------------------------------------\n");
    printf("So hoa don      : %d\n", n);
    printf("Tien phong      : %.0f VND\n", tongPhong);
    printf("Phu phi dich vu : %.0f VND\n", tongPhu);
    printf("TONG DOANH THU  : %.0f VND\n", tong);
}
