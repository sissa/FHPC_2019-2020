CC = gcc
CFLAGS = -Wall -O3 -march=native #-D__i686__

mountain: mountain.c fcyc2.c clock.c
	$(CC) $(CFLAGS) -o mountain mountain.c fcyc2.c clock.c 

clean:
	rm -f mountain *.o *~


