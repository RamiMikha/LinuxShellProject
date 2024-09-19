myshell: mysh.o
	gcc -g mysh.o -o myshell
mysh.o: mysh.c mysh.h
	gcc -g -c mysh.c
clean:
	$(RM) *.o
