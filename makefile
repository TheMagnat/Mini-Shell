CC = gcc
OBJ = moncd.o decouper.o main.o mon_if.o pipe.o lanceur.o metachar.o

shell: $(OBJ) head.h
	$(CC) -o shell $(OBJ)

%.o:	%.c
	$(CC) -c $<

clean:
	rm *.o

cleanall:
	rm *.o shell