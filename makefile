CC=gcc
LDFLAGS=-lX11
CFLAGS=-Wall -Wextra -Wpedantic -Werror

all: xgrab

xgrab.o: xgrab.c

OBJECTS=xgrab.o
xgrab: xgrab.o
	$(CC) $(CFLAGS) $(OBJECTS) -o xgrab $(LDFLAGS)

.PHONY:
clean:
	rm -f xgrab
