# http://nuclear.mutantstargoat.com/articles/make/

CC = gcc
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

CFLAGS = 
LDFLAGS = -lpthread -Wl,./FS-AI_API/fs-ai_api.a -lm

main: $(OBJ) $(CFLAGS)
	make -C ./FS-AI_API/
	$(CC) -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(OBJ) main
