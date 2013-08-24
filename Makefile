CC = gcc
CFLAGS = -Wall -ansi -std=c89 -g
LDFLAGS = -lmikmod
EXEC = renmod
RM = rm -rfv

.PHONY: all clean

all: $(EXEC)

clean:
	$(RM) *.o $(EXEC)

$(EXEC): main.c
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@
