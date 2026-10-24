#include "khachsan.h"

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
