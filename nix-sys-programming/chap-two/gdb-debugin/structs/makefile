cc = riscv64-linux-gnu-gcc
cflags = -static -ggdb -g -O0

exe: $(patsubst %.c, %.o, $(wildcard *.c))
	$(cc) $(cflags) -o $@ $^

%.o: %.c
	$(cc) $(cflags) -c -o $@ $^

debug: exe
	@qemu-riscv64 -g 1234 $^ &
	@gdb-multiarch $< -iex "target remote :1234"

clean:
	rm *.o exe
