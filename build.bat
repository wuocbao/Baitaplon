@echo off
gcc -Wall -Wextra main.c phong.c nhaplieu.c fileio.c timkiem.c nghiepvu.c -o khachsan.exe
if %errorlevel%==0 (echo Bien dich thanh cong: khachsan.exe) else (echo Co loi khi bien dich!)
pause
