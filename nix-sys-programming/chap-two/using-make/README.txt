A make file is made of "targets", "dependencies" and "rules".

> target: file to be created/updated, may be a directive or a label
          it depends on source files, object files or other target files
> dependencies: anything depended on to create the resulting files
> rules: set of commands for the make program to execute

automatic variable of make
----------------------------
$@ -> name of current target
$< -> name of first dependency
$^ -> names of all dependencies
$* -> name of current depedency without extension
$? -> 

> recompile files if dependencies change
DEPS = type.h		# list all dependencies
%.o: %.c $(DEPS)	# for all .o files if its .c or .h file has changed
	$(CC) -c -o $@  # compile corresponding .c file again

# makefile directives
.s.o:	# for each fname.o, assemble fname.s into fname.o
	$(AS) -o $< -o $@	# -o $@ is required for .c files

.c.o:	# for each fname.o, assemble fname.c into fname.o
	$(AS) -o $< -o $@	# -o $@ is optional for .c files

%.o: %.c $(DEPS)	# for all .o files if its .c or .h file has changed
	$(CC) -c -o $@ $<  # compile corresponding .c file again

target: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^
