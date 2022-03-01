# Assembly Programming AT&T Syntax

### assembly syntax
the at&t assembly syntax (same as gnu assembly syntax) is the default assembly
used in unix systems. The intel syntax is similar to the one used in Windows
operating systems (and its the one i am familiar with).

### stack
> the stack grows toward the lower memory addresses and decreases towards the
lower addresses. So, to allocate stack space, you subtract and to deallocate 
space on the stack you add

the 32-bit stack of a simple program is organised like this;

```
           |------------------------|
          ↑|                        |
          ||   local variables      |
lower     ||                        |-> ebp - 4 
addresses ||------------------------| 
           |  saved EBP             |-> ebp
           |------------------------| 
           |  return address        |-> ebp + 4 
           |------------------------|
           |                        |-> ebp + 8
           |  function parameters   | 
           |                        |
           |------------------------|
 ```

### registers

| 64 bit | 32 bit | 16 bit | 8 bit (high) | 8 bit (lower) |
|--------|--------|--------|--------------|---------------|
| RAX | EAX | AX | AH | AL | 
| RBX | EBX | BX | BH | BL |
| RCX | ECX | CX | CH | CL |
| RDX | EDX | DX | DH | DL |
| RSI | ESI | SI |
| RDI | EDI | DI |
| RSP | ESP | SP |
| RBP | EBP | BP |

#### references
- [AT&T Assembly Syntax](https://flint.cs.yale.edu/cs421/papers/x86-asm/asm.html)
- [AT&T Assembly Programming](https://csiflabs.cs.ucdavis.edu/~ssdavis/50/att-syntax.htm)
- [AT&T assembly syntax and IA-32 instructions](https://gist.github.com/DmitrySoshnikov/c67cbde1cceb0d6a194830b41baa5c8b)
- [x86 Assembly/GNU assembly syntax](https://en.wikibooks.org/wiki/X86_Assembly/GNU_assembly_syntax)
- [AT&T Assembly Language](http://neuraldk.org/document.php?att_asm)
- [Purpose of ESI & EDI registers?](https://stackoverflow.com/questions/1856320/purpose-of-esi-edi-registers)
- [What's the purpose of the LEA instruction?](https://stackoverflow.com/questions/1658294/whats-the-purpose-of-the-lea-instruction?rq=1)
- [8086 ASSEMBLY LANGUAGE](https://www.cs.uregina.ca/Dept/manuals/Manuals/7Language/7_9_8086/7_9Assembly.html)

