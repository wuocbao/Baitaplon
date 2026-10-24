#include "khachsan.h"

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
