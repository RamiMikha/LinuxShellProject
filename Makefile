myshell: mysh.o lib.o messages.o
	gcc -g mysh.o lib.o messages.o  -o myshell
mysh.o: mysh.c mysh.h
	gcc -g -c mysh.c
lib.o: lib.c lib.h
	gcc -g -c lib.c
messages.o: messages.c messages.h
	gcc -g -c messages.c
clean:
	$(RM) *.o
