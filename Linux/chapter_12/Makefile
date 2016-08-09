CC=gcc
OBJ = happychat
all:
	$(CC)  -c   ./*.c  `pkg-config  --libs --cflags gtk+-2.0`
	$(CC)  -o $(OBJ)  ./*.o -I `pkg-config --libs --cflags gtk+-2.0` -g
	rm -rf *.o

clean:
	-rm -rf *.o
