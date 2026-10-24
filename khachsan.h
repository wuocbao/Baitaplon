#ifndef KHACHSAN_H
#define KHACHSAN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HANG 5
#define COT 5
#define TONG (HANG * COT)
#define FILE_DATA "phong.txt"

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

/* Mang phong dung chung (dinh nghia trong phong.c) */
extern Phong phong[HANG][COT];

/* ---- nhaplieu.c ---- */
void boXuongDong(char *s);
int nhapSoNguyen(const char *loiNhac);
float nhapSoThuc(const char *loiNhac);
void nhapChuoi(const char *loiNhac, char *s, int size);

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
void hienThiPhongTrong(void);
void timPhongTheoLoai(void);
void timPhongTheoGia(void);
void sapXep(void);

/* ---- nghiepvu.c ---- */
void datPhong(void);
void checkIn(void);
void themDichVu(void);
void checkOut(void);

#endif
