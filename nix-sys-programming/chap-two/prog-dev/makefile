all: first jmp ass

first:
	gcc -o exe t1.c t2.c

jmp:
	gcc -o newexe longjump.c

ass:
	gcc -S t.c

clean:
	rm exe newexe t.s
