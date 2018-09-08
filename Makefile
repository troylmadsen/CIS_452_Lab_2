filename = simple_shell
objects = simple_shell.o

simple_shell: $(objects)
	clang -o ${filename} -Wall $(objects)

simple_shell.o: simple_shell.c
	clang -c -o simple_shell.o -Wall simple_shell.c

.PHONY: clean run debug
clean:
	-rm $(filename) $(objects)

run: $(filename)
	./$(filename)

debug:
	clang -c -o simple_shell.o -Wall -g simple_shell.c
	clang -o $(filename) -Wall -g simple_shell.o
