CC = gcc
CFLAGS = -Wall -Wextra
OBJ = main.o phong.o nhaplieu.o fileio.o timkiem.o nghiepvu.o

khachsan: $(OBJ)
	$(CC) $(OBJ) -o khachsan

%.o: %.c khachsan.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o khachsan
