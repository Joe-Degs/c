CC = gcc
DEFLAGS = -MP -MD
CFLAGS = -Wextra -Wall -g $(DEFLAGS) -O0

srv_bin = srv
srv_code = . ./server
srv_files = $(foreach dir, $(srv_code), $(wildcard $(dir)/*.c))
srv_obj = $(patsubst %.c, %.o, $(srv_files))

cl_bin = cl
cl_code = . ./client
cl_files = $(foreach dir, $(cl_code), $(wildcard $(dir)/*.c))
cl_obj = $(patsubst %.c, %.o, $(cl_files))

depfiles = $(foreach dir, . ./server ./client, $(wildcard $(dir)/*.c))
deps = $(patsubst %.c, %.d, $(depfiles))

all: $(cl_bin) $(srv_bin)

echo:
	@echo server
	@echo $(srv_file)
	@echo $(srv_obj)
	@echo $(deps)
	@echo client

$(srv_bin): $(srv_obj)
	$(CC) $(CFLAGS) -o $@ $^

$(cl_bin): $(srv_obj)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

clean:
	rm -rf $(deps) $(cl_obj) $(srv_obj) $(srv_bin) $(cl_bin)
