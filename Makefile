CC = gcc
CFLAGS = -std=c99 -Wall -Wextra  -pedantic -g -D_GNU_SOURCE
INCS=-I/usr/include -I.
LIBS = -L/usr/lib -lc


repl: repl.c map.c
	@$(CC) $(CFLAGS) repl.c map.c -o repl $(INCS) $(LIBS) -lreadline
