myshell: mysh.o lib.o
	gcc -g mysh.o lib.o  -o myshell
mysh.o: mysh.c mysh.h
	gcc -g -c mysh.c
lib.o: lib.c lib.h
	gcc -g -c lib.c
clean:
	$(RM) *.o
