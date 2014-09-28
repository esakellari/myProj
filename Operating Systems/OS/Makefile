OBJS 	= main.o OS.o routines.o
SOURCE	= main.c OS.c routines.c
HEADER  = OS.h routines.h 
OUT  	= OS
CC	= gcc
FLAGS   = -g -c 
all: $(OBJS)
	$(CC) -g $(SOURCE) -o $(OUT)
main.o: main.c
	$(CC) $(FLAGS) main.c
OS.o: OS.c
	$(CC) $(FLAGS) OS.c
routines.o: routines.c
	$(CC) $(FLAGS) routines.c
clean:
	rm -f $(OBJS) $(OUT)
count:
	wc $(SOURCE) $(HEADER)


