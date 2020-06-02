CC=gcc
CFLAGS=-Wall

all: sudo

pile: pile.o sudo.o
	$(CC) -o pile pile.o sudo.o

pile.o: pile.c
	$(CC) -o pile.o -c pile.c $(CFLAGS)

lec-fichier-bon.o: lec-fichier-bon.c
	$(CC) -o lec-fichier-bon.o -c lec-fichier-bon.c $(CFLAGS)

sudo.o: sudo.c pile.h
	$(CC) -o sudo.o -c sudo.c $(CFLAGS)

clean:
		rm -rf *.o

mrproper: clean
		rm -rf sudo
