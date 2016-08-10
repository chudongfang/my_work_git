CC=gcc
OBJ = happychat_server
all:
	$(CC)  -c   ./*.c  `pkg-config  --libs --cflags gtk+-2.0` -pthread
	$(CC)  -o $(OBJ)  ./*.o -I `pkg-config --libs --cflags gtk+-2.0` -pthread -g
	rm -rf *.o

clean:
	-rm -rf *.o
