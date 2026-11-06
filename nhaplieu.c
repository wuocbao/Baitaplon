#include "khachsan.h"

void boXuongDong(char *s) {
    s[strcspn(s, "\r\n")] = '\0';
}

/* Cat khoang trang thua o hai dau chuoi */
void catKhoang(char *s) {
    int len = (int)strlen(s), i = 0;
    while (len > 0 && (s[len - 1] == ' ' || s[len - 1] == '\t' || s[len - 1] == '\r' || s[len - 1] == '\n'))
        s[--len] = '\0';
    while (s[i] == ' ' || s[i] == '\t') i++;
    if (i > 0) memmove(s, s + i, strlen(s + i) + 1);
}

/* Doc mot dong. Tra ve: 1 = ok, 0 = het du lieu (EOF), -1 = dong qua dai */
static int docDong(const char *loiNhac, char *buf, int size) {
    int ch;
    printf("%s", loiNhac);
    fflush(stdout);
    if (fgets(buf, size, stdin) == NULL) return 0;
    if (strchr(buf, '\n') == NULL && !feof(stdin)) {   /* nhap qua dai: bo phan thua */
        while ((ch = getchar()) != '\n' && ch != EOF) { }
        return -1;
    }
    boXuongDong(buf);
    catKhoang(buf);
    return 1;
}

/* Nhap so nguyen khong am. Nhap so am, so thuc, chu, ky tu dac biet -> bao loi va bat nhap lai.
   Het du lieu (EOF) tra ve 0 de chuong trinh luon thoat duoc. */
int nhapSoNguyen(const char *loiNhac) {
    char buf[64];
    int r, i, hopLe;
    while (1) {
        r = docDong(loiNhac, buf, sizeof(buf));
        if (r == 0) return 0;
        if (r < 0) { printf("Khong hop le! Gia tri nhap qua dai.\n"); continue; }
        if (buf[0] == '\0') { printf("Khong hop le! Khong duoc de trong.\n"); continue; }
        if (buf[0] == '-') { printf("Khong hop le! Khong duoc nhap so am.\n"); continue; }
        hopLe = 1;
        for (i = 0; buf[i]; i++)
            if (!isdigit((unsigned char)buf[i])) { hopLe = 0; break; }
        if (!hopLe) {
            printf("Khong hop le! Chi duoc nhap so nguyen (khong nhap so thuc, chu hoac ky tu dac biet).\n");
            continue;
        }
        if (strlen(buf) > 9) { printf("Khong hop le! So qua lon.\n"); continue; }
        return atoi(buf);
    }
}

/* Nhap so thuc khong am (vi du tien dich vu, gia). Dung dinh dang: chu so, toi da mot dau cham. */
float nhapSoThuc(const char *loiNhac) {
    char buf[64];
    int r, i, cham, so;
    while (1) {
        r = docDong(loiNhac, buf, sizeof(buf));
        if (r == 0) return 0;
        if (r < 0) { printf("Khong hop le! Gia tri nhap qua dai.\n"); continue; }
        if (buf[0] == '\0') { printf("Khong hop le! Khong duoc de trong.\n"); continue; }
        if (buf[0] == '-') { printf("Khong hop le! Khong duoc nhap so am.\n"); continue; }
        cham = so = 0;
        for (i = 0; buf[i]; i++) {
            if (buf[i] == '.') cham++;
            else if (isdigit((unsigned char)buf[i])) so++;
            else { cham = 99; break; }
        }
        if (cham > 1 || so == 0) {
            printf("Khong hop le! Vui long nhap mot so (vi du: 50000).\n");
            continue;
        }
        if (strlen(buf) > 12) { printf("Khong hop le! So qua lon.\n"); continue; }
        return (float)atof(buf);
    }
}

/* Nhap lua chon tu 0 den toiDa. Ngoai khoang -> bao khong hop le va bat nhap lai. */
int nhapLuaChon(const char *loiNhac, int toiDa) {
    int x;
    while (1) {
        x = nhapSoNguyen(loiNhac);
        if (x <= toiDa) return x;
        printf("Khong hop le! Chi duoc chon tu 0 den %d.\n", toiDa);
    }
}

/* Nhap chuoi khong rong. Nhap "0" de quay lai -> tra ve 0. */
int nhapChuoi(const char *loiNhac, char *s, int size) {
    int r;
    while (1) {
        r = docDong(loiNhac, s, size);
        if (r == 0) { s[0] = '\0'; return 0; }
        if (r < 0) { printf("Khong hop le! Toi da %d ky tu.\n", size - 1); continue; }
        if (strcmp(s, "0") == 0) return 0;
        if (s[0] == '\0') { printf("Khong hop le! Khong duoc de trong.\n"); continue; }
        return 1;
    }
}

/* Dung man hinh cho den khi nguoi dung nhap 0 */
void choQuayLai(void) {
    while (nhapSoNguyen("\nNhap 0 de quay lai menu: ") != 0)
        printf("Khong hop le! Chi nhap 0 de quay lai.\n");
}
