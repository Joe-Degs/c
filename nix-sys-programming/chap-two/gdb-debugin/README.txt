intro to gdb
-------------
> use print to evaluate a c expression
    
    $ print 1 + 4
    $ print (int) 556544434

> get the value of variables

    $ print <var_name>

> get address of variable

    $ print &<var_name>

> get size of variable in memory

    $ print sizeof(<var_name>)
    
    - you can also get the size of a type just like in c

        $ print sizeof(long)

examining memory
-----------------
use the "x" command to examine memory. execute "help x" at the gdb prompt for
format options for the x

> look at memory at address

    $ x/xb <address>

> set variable to value

    $ set var <var_name> = <value>

> check the type of a variable

    $ ptype <var_name>

arrays and pointers
--------------------
- arrays are pretty much just pointers in c. they point to the start of some
  memory location that has a bunch of bytes in it.
- you can do pointer arithmetic on them, just like pointers but there are two
  things to keep in mind.
    1. if you pass an array to "sizeof" it returns the actual size of the array
       not the pointer
    2. if you take a pointer to an array you still get a pointer pointing to the
       first element 
        $ arr == &arr
NB: Arrays are weird; Beware.

> show contents my_ptr points to
    
    $ p *my_ptr

> show array that has decayed into pointer or 
> make any pointer into array of size "len"

    $ p *my_ptr@len

> get the type of variable


structs
---------
> see the layout of structs

    $ pt <type_name>

- do "set print pretty" to get better output
