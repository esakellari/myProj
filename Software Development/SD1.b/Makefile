OBJS 	= main4.o HDF_Lib.o FMM_Lib.o bitmap.o
SOURCE	= main4.c HDF_Lib.c FMM_Lib.c bitmap.c
HEADER  = HDF_Lib.h FMM_Lib.h bitmap.h BF_Defines.h
OUT  	= sd
CC	= gcc
FLAGS   = -c -g

all: $(OBJS)
	$(CC) $(OBJS) -o $(OUT)   

# create/compile the individual files >>separately<< 
main4.o: main4.c
	$(CC) $(FLAGS) main4.c

HDF_Lib.o : HDF_Lib.c 
	$(CC) $(FLAGS) HDF_Lib.c 

FMM_Lib.o: FMM_Lib.c
	$(CC) $(FLAGS) FMM_Lib.c

bitmap.o: bitmap.c
	$(CC) $(FLAGS) bitmap.c

clean:
	rm -f $(OBJS) $(OUT)

count:
	wc $(SOURCE) $(HEADER)

