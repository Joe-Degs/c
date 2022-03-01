# Assembly Programming AT&T Syntax

#### assembly syntax
the at&t assembly syntax (same as gnu assembly syntax) is the default assembly
used in unix systems. The intel syntax is similar to the one used in Windows
operating systems (and its the one i am familiar with).

#### stack and heap
> the stack grows toward the lower memory addresses and decreases towards the
lower addresses. So, to allocate stack space, you subtract and to deallocate 
space on the stack you add

the stack is mostly organised like this;

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

#### registers
at&t syntax is more like the risc-v assembly language.

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

