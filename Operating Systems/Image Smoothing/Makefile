SOURCE =  R_procedure.c  smooth.c  M_procedure.c
CC = gcc 
OUT = smooth R M
all:
	$(CC) -g $ R_procedure.c -o $ R
	$(CC) -g $ M_procedure.c -o $ M
	$(CC) -g $ smooth.c -o $ smooth
clean:
	rm -f $(OUT)
count:
	wc $(SOURCE) $(HEADER)

