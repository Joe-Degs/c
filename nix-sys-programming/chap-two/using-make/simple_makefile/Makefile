CC = gcc
INCLUDES = -I$(shell pwd)
OPT = -O0
CFLAGS = -Wextra -Wall -g $(INCLUDES) $(OPT)

objs = x.o y.o

all: bin

bin: $(objs)
	gcc $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

clean:
	rm *.o bin 2> /dev/null
