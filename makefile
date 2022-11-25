all: shell main

clean:
	rm -f vagon-sh.out shell.o

shell: shell.c
	clang -D_GNU_SOURCE -D_STRICT_ANSI -fno-stack-protector -std=c18 -include shell.h -c shell.c

main: main.c shell.o
	clang -D_GNU_SOURCE -std=c18 -g shell.o main.c -o vagon-sh.out
