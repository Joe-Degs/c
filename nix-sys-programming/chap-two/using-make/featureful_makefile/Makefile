BINARY = bin

code_dirs = . lib
inc_dirs = . ./include/

# compiler constants 
CC = gcc
DEPFLAGS = -MP -MD  # gcc support for makefiles
OPT = -O0
# recurse through all values in inc_dirs and add -I
CFLAGS = -Wextra -Wall -g $(foreach dir, $(inc_dirs), -I$(dir)) $(DEPFLAGS) $(OPT)

# recurse the directories and glob files with the c extension
CFILES = $(foreach dir, $(code_dirs), $(wildcard $(dir)/*.c))
# pattern substitution for files ending .c to .o
OBJECTS = $(patsubst %.c, %.o, $(CFILES))
DEPFILES = $(patsubst %.c, %.d, $(CFILES))

all: $(BINARY)

$(BINARY): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

clean:
	rm -rf *.tgz $(OBJECTS) $(DEPFILES) $(BINARY) 2> /dev/null

dist: clean
	tar zcvf $@.tgz *

# @ silences the printing of the command before executing
# makefile info command prints
diff:
	$(info The status of the repo, and the volume of per-file changes:)
	@git status $(shell pwd)
	@git diff --stat $(shell pwd)
