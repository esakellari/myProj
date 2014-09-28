CC = gcc
FLAGS = -Wall

OBJS = HDF_Lib.o FMM_Lib.o BMM_Lib.o BF_Lib.o RMM_Lib.o RecBitmap.o main.o
HDRS = ./*.h
SRCS = ./*.c
PROG = prog


all: $(OBJS)
	$(CC) $(OBJS) -o $(PROG)

RMM_Lib.o: $(HDRS) RMM_Lib.c
	$(CC) $(FLAGS) -c RMM_Lib.c
RecBitmap.o: $(HDRS) RecBitmap.c
	$(CC) $(FLAGS) -c RecBitmap.c
main.o: $(HDRS) main.c
	$(CC) $(FLAGS) -c main.c

clean:
	rm -f RMM_Lib.o RecBitmap.o main.o
del:
	rm -f EVENTS ATHLETES PARTICIPATIONS

