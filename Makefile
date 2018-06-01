CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Werror -pedantic -D_GNU_SOURCE
INCS=-I/usr/include -I.
LIBS = -L/usr/lib -lc


repl: repl.c
	@$(CC) $(CFLAGS) repl.c -o repl $(INCS) $(LIBS) -lreadline
