CC:=gcc
CFLAGS+= -c -Wall -g

tetris_game: input.o tetris.o main.o
	$(CC) $^ -o $@

%.o: %.c
	$(CC) $^ $(CFLAGS) -o $@

clean:
	rm tetris_game *.o -rf
