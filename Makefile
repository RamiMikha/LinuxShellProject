myshell: mysh.o lib.o messages.o my_strings.o commands.o
	gcc -g mysh.o lib.o messages.o my_strings.o commands.o -o myshell
mysh.o: mysh.c mysh.h
	gcc -g -c mysh.c
lib.o: lib.c lib.h
	gcc -g -c lib.c
messages.o: messages.c messages.h
	gcc -g -c messages.c
my_strings.o: my_strings.c my_strings.h
	gcc -g -c my_strings.c
commands.o: commands.c commands.h
	gcc -g -c commands.c
clean:
	$(RM) *.o
