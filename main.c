#include "khachsan.h"

/* ===================== MENU TRANG QUAN TRI (CON TRO HAM) ===================== */

typedef void (*HamMenu)(void);

typedef struct {
    const char *ten;
    HamMenu ham;
    int quyen;      /* bit VAITRO_NV / VAITRO_QL duoc phep thay muc nay */
    int canCa;      /* 1: nhan vien phai dang trong ca moi duoc dung */
} MucMenu;

#define CHUNG (VAITRO_NV | VAITRO_QL)

static MucMenu menu[] = {
    {"Hien thi tat ca phong",          hienThiPhong,       CHUNG,     0},
    {"Tim phong trong",                hienThiPhongTrong,  CHUNG,     0},
    {"Tim phong trong theo loai",      timPhongTheoLoai,   CHUNG,     0},
    {"Tim phong trong theo gia",       timPhongTheoGia,    CHUNG,     0},
    {"Sap xep (theo ngay / theo gia)", sapXep,             CHUNG,     0},
    {"Dat phong",                      datPhong,           CHUNG,     1},
    {"Check-in",                       checkIn,            CHUNG,     1},
    {"Them dich vu (phu phi)",         themDichVu,         CHUNG,     1},
    {"Check-out & in hoa don",         checkOut,           CHUNG,     1},
    {"Luu du lieu ra file",            ghiFile,            CHUNG,     0},
    /* --- rieng nhan vien --- */
    {"Check-in ca lam",                checkInCa,          VAITRO_NV, 0},
    {"Check-out ca lam",               checkOutCa,         VAITRO_NV, 0},
    /* --- rieng quan ly --- */
    {"Xem doanh thu",                  xemDoanhThu,        VAITRO_QL, 0},
    {"Xem lich su ca lam",             xemLichSuCa,        VAITRO_QL, 0},
    {"Them tai khoan nhan vien",       themNhanVien,       VAITRO_QL, 0}
};

#define SO_MUC (int)(sizeof(menu) / sizeof(menu[0]))

/* In menu theo vai tro, ds[] luu chi so that trong menu[]. Tra ve so muc hien thi */
static int hienMenu(int ds[]) {
    int i, n = 0;
    printf("\n===== TRANG QUAN TRI - MY HOTEL =====\n");
    printf("Tai khoan: %s (%s)", nguoiDung->hoTen, tenVaiTro(nguoiDung->vaiTro));
    if (nguoiDung->vaiTro == VAITRO_NV)
        printf(" - %s", daVaoCa() ? "Dang trong ca" : "Chua vao ca");
    printf("\n");
    for (i = 0; i < SO_MUC; i++) {
        if (menu[i].quyen & nguoiDung->vaiTro) {
            ds[n++] = i;
            printf("%d. %s\n", n, menu[i].ten);
        }
    }
    printf("0. Dang xuat (tu dong luu)\n");
    return n;
}

/* Dang nhap roi chay menu quan ly / nhan vien. Thoat ra la quay lai trang khach. */
static void trangQuanTri(void) {
    int ds[SO_MUC], n, chon;

    if (dangNhap() != 1) return;      /* 0: sai qua 3 lan, -1: nguoi dung huy */

    do {
        n = hienMenu(ds);
        chon = nhapSoNguyen("Nhap lua chon: ");

        if (chon == 0) {
            if (nguoiDung->vaiTro == VAITRO_NV && daVaoCa() &&
                nhapSoNguyen("Ban chua check-out ca lam. Check-out ngay? (1: co, 0: khong): ") == 1)
                checkOutCa();
            ghiFile();
            printf("Da dang xuat.\n");
            break;
        }
        if (chon >= 1 && chon <= n) {
            const MucMenu *m = &menu[ds[chon - 1]];
            if (m->canCa && nguoiDung->vaiTro == VAITRO_NV && !daVaoCa())
                printf("Ban phai check-in ca lam truoc khi thuc hien chuc nang nay!\n");
            else
                m->ham();
        } else {
            printf("Lua chon khong hop le!\n");
        }
    } while (1);

    nguoiDung = NULL;
}

/* ===================== TRANG KHACH HANG (MAN HINH DAU TIEN) ===================== */

static void hienMenuKhach(void) {
    printf("\n==========================================\n");
    printf("                MY HOTEL\n");
    printf("     Chao mung quy khach den voi khach san\n");
    printf("==========================================\n");
    printf("1. Xem tat ca phong\n");
    printf("2. Xem phong trong\n");
    printf("3. Tim phong trong theo loai\n");
    printf("4. Tim phong trong theo gia\n");
    printf("5. Dat phong\n");
    printf("6. Trang quan tri (Nhan vien / Quan ly)\n");
    printf("0. Thoat\n");
}

int main() {
    int chon;

    khoiTaoPhong();
    docFile();
    docTaiKhoan();

    do {
        hienMenuKhach();
        chon = nhapLuaChon("Nhap lua chon: ", 6);
        switch (chon) {
            case 1: hienThiPhongKhach();   break;
            case 2: hienThiPhongTrong();   break;
            case 3: timPhongTheoLoai();    break;
            case 4: timPhongTheoGia();     break;
            case 5: datPhong(); ghiFile(); break;   /* luu ngay de khong mat don dat */
            case 6: trangQuanTri();        break;
        }
    } while (chon != 0);

    ghiFile();
    printf("Cam on quy khach. Hen gap lai!\n");
    return 0;
}
