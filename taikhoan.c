#include "khachsan.h"

/* ===================== TAI KHOAN / DANG NHAP ===================== */

static TaiKhoan dsTK[MAX_TK];
static int soTK = 0;
TaiKhoan *nguoiDung = NULL;

const char *tenVaiTro(int vaiTro) {
    return vaiTro == VAITRO_QL ? "Quan ly" : "Nhan vien";
}

int timTaiKhoan(const char *user) {
    int i;
    for (i = 0; i < soTK; i++)
        if (strcmp(dsTK[i].user, user) == 0) return i;
    return -1;
}

static void ghiTaiKhoan(void) {
    FILE *f = fopen(FILE_TAIKHOAN, "w");
    int i;
    if (f == NULL) {
        printf("Khong ghi duoc file tai khoan!\n");
        return;
    }
    for (i = 0; i < soTK; i++)
        fprintf(f, "%s|%s|%d|%s\n", dsTK[i].user, dsTK[i].pass, dsTK[i].vaiTro, dsTK[i].hoTen);
    fclose(f);
}

static void themTK(const char *user, const char *pass, int vaiTro, const char *hoTen) {
    TaiKhoan *t = &dsTK[soTK++];
    snprintf(t->user, sizeof(t->user), "%s", user);
    snprintf(t->pass, sizeof(t->pass), "%s", pass);
    snprintf(t->hoTen, sizeof(t->hoTen), "%s", hoTen);
    t->vaiTro = vaiTro;
}

static void taoTaiKhoanMacDinh(void) {
    soTK = 0;
    themTK("admin", "admin123", VAITRO_QL, "Quan ly");
    themTK("nhanvien1", "123456", VAITRO_NV, "Nhan vien 1");
    ghiTaiKhoan();
    printf("Chua co file tai khoan, da tao tai khoan mac dinh:\n");
    printf("  Quan ly  : admin / admin123\n");
    printf("  Nhan vien: nhanvien1 / 123456\n");
}

void docTaiKhoan(void) {
    FILE *f = fopen(FILE_TAIKHOAN, "r");
    char dong[256];
    soTK = 0;
    if (f == NULL) {
        taoTaiKhoanMacDinh();
        return;
    }
    while (soTK < MAX_TK && fgets(dong, sizeof(dong), f) != NULL) {
        char u[20], p[20], ten[40];
        int vt;
        if (sscanf(dong, "%19[^|]|%19[^|]|%d|%39[^\n]", u, p, &vt, ten) != 4) continue;
        if (vt != VAITRO_NV && vt != VAITRO_QL) continue;
        themTK(u, p, vt, ten);
    }
    fclose(f);
    if (soTK == 0) taoTaiKhoanMacDinh();
}

int dangNhap(void) {
    char user[64], pass[64];
    int lan, i;

    printf("\n===== DANG NHAP =====\n");
    for (lan = 1; lan <= 3; lan++) {
        printf("Ten dang nhap (nhap 0 de quay lai): ");
        if (fgets(user, sizeof(user), stdin) == NULL) return -1;
        boXuongDong(user);
        if (strcmp(user, "0") == 0) return -1;

        printf("Mat khau: ");
        if (fgets(pass, sizeof(pass), stdin) == NULL) return -1;
        boXuongDong(pass);

        for (i = 0; i < soTK; i++) {
            if (strcmp(dsTK[i].user, user) == 0 && strcmp(dsTK[i].pass, pass) == 0) {
                nguoiDung = &dsTK[i];
                printf("Dang nhap thanh cong! Xin chao %s (%s)\n",
                       nguoiDung->hoTen, tenVaiTro(nguoiDung->vaiTro));
                return 1;
            }
        }
        printf("Sai ten dang nhap hoac mat khau! (%d/3)\n", lan);
    }
    printf("Nhap sai qua 3 lan!\n");
    return 0;
}

/* Chi quan ly moi co muc nay trong menu */
void themNhanVien(void) {
    char user[20], pass[20], ten[40];

    if (soTK >= MAX_TK) {
        printf("Da dat so tai khoan toi da!\n");
        return;
    }
    if (!nhapChuoi("Ten dang nhap moi (nhap 0 de quay lai): ", user, sizeof(user))) return;
    if (strchr(user, '|') != NULL) {
        printf("Ten dang nhap khong duoc chua ky tu '|'!\n");
        return;
    }
    if (timTaiKhoan(user) != -1) {
        printf("Ten dang nhap da ton tai!\n");
        return;
    }
    if (!nhapChuoi("Mat khau (nhap 0 de quay lai): ", pass, sizeof(pass))) return;
    if (!nhapChuoi("Ho ten nhan vien (nhap 0 de quay lai): ", ten, sizeof(ten))) return;
    if (strchr(pass, '|') != NULL || strchr(ten, '|') != NULL) {
        printf("Khong duoc dung ky tu '|'!\n");
        return;
    }
    themTK(user, pass, VAITRO_NV, ten);
    ghiTaiKhoan();
    printf("Da tao tai khoan nhan vien %s\n", user);
}
