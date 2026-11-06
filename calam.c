#include "khachsan.h"

/* ===================== CHECK-IN / CHECK-OUT CA LAM =====================
 * File calam.txt ghi tung su kien, moi dong:
 *   VAO|ten_dang_nhap|epoch|dd/mm/yyyy hh:mm
 *   RA |ten_dang_nhap|epoch|dd/mm/yyyy hh:mm
 * Nhan vien dang trong ca neu su kien cuoi cung cua ho la VAO.
 */

static void ghiSuKien(const char *loai, time_t t) {
    FILE *f = fopen(FILE_CALAM, "a");
    struct tm *tm = localtime(&t);
    if (f == NULL) {
        printf("Khong ghi duoc file ca lam!\n");
        return;
    }
    fprintf(f, "%s|%s|%lld|%02d/%02d/%04d %02d:%02d\n", loai, nguoiDung->user, (long long)t,
            tm->tm_mday, tm->tm_mon + 1, tm->tm_year + 1900, tm->tm_hour, tm->tm_min);
    fclose(f);
}

/* Tra ve thoi diem vao ca neu dang trong ca, nguoc lai -1 */
static long long thoiDiemVaoCa(const char *user) {
    FILE *f = fopen(FILE_CALAM, "r");
    char dong[128];
    long long vao = -1;
    if (f == NULL) return -1;
    while (fgets(dong, sizeof(dong), f) != NULL) {
        char loai[8], u[20], txt[32];
        long long t;
        if (sscanf(dong, "%7[^|]|%19[^|]|%lld|%31[^\n]", loai, u, &t, txt) != 4) continue;
        if (strcmp(u, user) != 0) continue;
        if (strcmp(loai, "VAO") == 0) vao = t;
        else if (strcmp(loai, "RA") == 0) vao = -1;
    }
    fclose(f);
    return vao;
}

static void inThoiLuong(long long giay) {
    printf("%lldh%02dp", giay / 3600, (int)((giay % 3600) / 60));
}

int daVaoCa(void) {
    return nguoiDung != NULL && thoiDiemVaoCa(nguoiDung->user) != -1;
}

void checkInCa(void) {
    if (daVaoCa()) {
        printf("Ban dang trong ca lam roi!\n");
        return;
    }
    ghiSuKien("VAO", time(NULL));
    printf("Check-in ca lam thanh cong. Chuc %s lam viec tot!\n", nguoiDung->hoTen);
}

void checkOutCa(void) {
    long long vao = thoiDiemVaoCa(nguoiDung->user);
    time_t now;
    if (vao == -1) {
        printf("Ban chua check-in ca lam!\n");
        return;
    }
    now = time(NULL);
    ghiSuKien("RA", now);
    printf("Check-out ca lam thanh cong. Thoi gian lam viec: ");
    inThoiLuong((long long)(now - (time_t)vao));
    printf("\n");
}

/* Chi quan ly moi co muc nay trong menu */
void xemLichSuCa(void) {
    FILE *f = fopen(FILE_CALAM, "r");
    char dong[128];
    long long vaoT[MAX_TK];
    char vaoTxt[MAX_TK][32], tenTK[MAX_TK][20];
    int i, co = 0;

    if (f == NULL) {
        printf("Chua co du lieu ca lam!\n");
        return;
    }
    for (i = 0; i < MAX_TK; i++) vaoT[i] = -1;

    printf("\n===== LICH SU CA LAM =====\n");
    printf("%-12s %-17s %-17s %s\n", "Tai khoan", "Vao ca", "Ra ca", "Thoi gian");
    printf("-------------------------------------------------------------\n");
    while (fgets(dong, sizeof(dong), f) != NULL) {
        char loai[8], u[20], txt[32];
        long long t;
        int idx;
        if (sscanf(dong, "%7[^|]|%19[^|]|%lld|%31[^\n]", loai, u, &t, txt) != 4) continue;
        idx = timTaiKhoan(u);
        if (idx < 0) continue;
        if (strcmp(loai, "VAO") == 0) {
            vaoT[idx] = t;
            snprintf(vaoTxt[idx], sizeof(vaoTxt[idx]), "%s", txt);
            snprintf(tenTK[idx], sizeof(tenTK[idx]), "%s", u);
        } else if (strcmp(loai, "RA") == 0 && vaoT[idx] != -1) {
            printf("%-12s %-17s %-17s ", u, vaoTxt[idx], txt);
            inThoiLuong(t - vaoT[idx]);
            printf("\n");
            vaoT[idx] = -1;
            co = 1;
        }
    }
    fclose(f);
    for (i = 0; i < MAX_TK; i++)
        if (vaoT[i] != -1) {
            printf("%-12s %-17s %-17s (dang lam)\n", tenTK[i], vaoTxt[i], "-");
            co = 1;
        }
    if (!co) printf("Chua co ca lam nao!\n");
}
