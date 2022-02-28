	.text
	.global mysum, printf  # import mysum, import printf
mysum:
	# int mysum(int x, int y)
	#
	# establish stack frame
	pushl %ebp
	movl %esp, %ebp

	# compute x + y in AX register
	movl 8(%esp), %eax  # AX = x
	addl 12(%ebp), %eax # AX += y

	# deallocate stack and return
	movl %ebp, %esp
	pop %ebp
	ret
