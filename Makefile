CFLAGS = -ansi -pedantic -Wall -Werror -g
SOURCES = src/main.c src/lexer.c src/mempool.c

plxc: $(SOURCES)
	$(CC) $(CFLAGS) -o $@ $^
