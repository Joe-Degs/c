CC = gcc
CFLAGS = -Wall
OBJS = t.o mysum.o
INCLUDE = -Ipath

all: myt install

myt: type.h $(OBJS)
	$(CC) $(CFLAGS) -o myt $(OBJS) $(INCLUDE)

t.o: type.h t.c
	gcc -c t.c
mysum: type.h mysum.c
	gcc -c mysum.c

install: myt
	echo installing myt to /usr/local/bin
	sudo mv myt /usr/local/bin/

run: install
	echo run executable image
	myt || /bin/true

clean:
	rm -f *.o 2>/dev/null
	sudo rm -f /usr/local/bin/myt
