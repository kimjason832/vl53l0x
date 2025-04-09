CC=gcc
CFLAGS=-c -Wall -O2
LDFLAGS=-L/usr/local/lib -ltof

all: libtof.a tof_run

libtof.a: tof.o
	ar -rc libtof.a tof.o
	sudo cp libtof.a /usr/local/lib
	sudo cp tof.h /usr/local/include

tof.o: tof.c
	$(CC) $(CFLAGS) tof.c -o tof.o

tof_run: main.o
	$(CC) main.o -o tof_run $(LDFLAGS) -lwiringPi

main.o: main.c
	$(CC) $(CFLAGS) main.c -o main.o

clean:
	rm -f *.o libtof.a tof_run