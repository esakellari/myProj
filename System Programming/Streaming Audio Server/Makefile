#
# In order to execute this "Makefile" just type "make"
#	
#

OBJS 	= main.o functions.o url_codec.o thread_source.o
SOURCE	= main.c functions.c url_codec.c thread_source.c
HEADER  = functions.h thread_source.h url_codec.h
OUT  	= musicstreamer
CC	= gcc
FLAGS   = -c 
# -g option enables debugging mode 
# -c flag generates object code for separate files

all: $(OBJS)
	$(CC) $(OBJS) -o $(OUT) -lpthread  

# create/compile the individual files >>separately<< 
main.o: main.c
	$(CC) $(FLAGS) main.c

functions.o: functions.c
	$(CC) $(FLAGS) functions.c

url_codec.o: url_codec.c
	$(CC) $(FLAGS) url_codec.c

thread_source.o: thread_source.c
	$(CC) $(FLAGS) thread_source.c


# clean house
clean:
	rm -f $(OBJS) $(OUT)

# do a bit of accounting
count:
	wc $(SOURCE) $(HEADER)

