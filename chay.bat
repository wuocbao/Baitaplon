@echo off
cd /d "%~dp0"
"D:\msys64\ucrt64\bin\gcc.exe" -Wall -Wextra main.c phong.c nhaplieu.c fileio.c timkiem.c nghiepvu.c taikhoan.c calam.c doanhthu.c -o khachsan.exe
if errorlevel 1 (
    pause
    exit /b 1
)
.\khachsan.exe
pause
