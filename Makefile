CC = gcc
CFLAGS = -c
FFLAGS = -pthread -lm
prog: header.o main.o
	$(CC) -o prog header.o main.o $(FFLAGS)
header.o : header.c
	$(CC) $(CFLAGS) header.c $(FFLAGS)
main.o: main.c
	$(CC) $(CFLAGS) main.c $(FFLAGS)
clean:
	rm -f a_* b_* prog *.o
