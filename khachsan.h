#ifndef KHACHSAN_H
#define KHACHSAN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define HANG 5
#define COT 5
#define TONG (HANG * COT)
#define FILE_DATA "phong.txt"
#define FILE_TAIKHOAN "taikhoan.txt"
#define FILE_CALAM "calam.txt"
#define FILE_DOANHTHU "doanhthu.txt"

/* Vai tro (dung nhu bit de loc menu) */
#define VAITRO_NV 1
#define VAITRO_QL 2
#define MAX_TK 50

/* Trang thai phong */
#define TRONG 0
#define DA_DAT 1
#define DANG_O 2

/* Cach tinh tien */
#define THEO_NGAY 1
#define THEO_GIO 2

typedef struct {
    char ten[50];
    char sdt[15];
    int ngay, thang, nam;   /* ngay nhan phong du kien */
    int kieuTinh;           /* 1: theo ngay, 2: theo gio */
    int soLuong;            /* so ngay hoac so gio */
    float phuPhi;           /* tien dich vu */
} KhachHang;

typedef struct {
    int maPhong;
    char loaiPhong[20];
    float giaNgay;
    float giaGio;
    int trangThai;
    KhachHang khach;
} Phong;

typedef struct {
    char user[20];
    char pass[20];
    int vaiTro;             /* VAITRO_NV hoac VAITRO_QL */
    char hoTen[40];
} TaiKhoan;

/* Mang phong dung chung (dinh nghia trong phong.c) */
extern Phong phong[HANG][COT];

/* Tai khoan dang dang nhap (NULL neu chua dang nhap) */
extern TaiKhoan *nguoiDung;

/* ---- nhaplieu.c ---- */
void boXuongDong(char *s);
void catKhoang(char *s);
int nhapSoNguyen(const char *loiNhac);
float nhapSoThuc(const char *loiNhac);
int nhapLuaChon(const char *loiNhac, int toiDa);
int nhapChuoi(const char *loiNhac, char *s, int size);   /* tra ve 0 neu nhap "0" (quay lai) hoac het du lieu */
void choQuayLai(void);

/* ---- phong.c ---- */
const char *tenTrangThai(int tt);
Phong *timPhong(int ma);
void xoaKhach(Phong *p);
void inTieuDe(void);
void inMotPhong(const Phong *p);
void khoiTaoPhong(void);

/* ---- fileio.c ---- */
void ghiFile(void);
int docFile(void);

/* ---- timkiem.c ---- */
void hienThiPhong(void);
void hienThiPhongKhach(void);
void hienThiPhongTrong(void);
void timPhongTheoLoai(void);
void timPhongTheoGia(void);
void sapXep(void);

/* ---- nghiepvu.c ---- */
void datPhong(void);
void checkIn(void);
void themDichVu(void);
void checkOut(void);

/* ---- taikhoan.c ---- */
void docTaiKhoan(void);
int dangNhap(void);               /* 1: thanh cong, 0: sai qua 3 lan, -1: thoat */
int timTaiKhoan(const char *user);
const char *tenVaiTro(int vaiTro);
void themNhanVien(void);

/* ---- calam.c ---- */
int daVaoCa(void);
void checkInCa(void);
void checkOutCa(void);
void xemLichSuCa(void);

/* ---- doanhthu.c ---- */
void ghiDoanhThu(const Phong *p, float tienPhong);
void xemDoanhThu(void);

#endif
