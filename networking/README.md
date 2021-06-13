### A bit of Network Programming in C
Trying hard to learn a bit about networking programming the classic way(in c). And im using beej's guide to network programming, its been fun so far but let's get into actual programming and see what happens.

I do not really know how to write c code yet. My hope is to learn how to write c code from this too. And if i succeed hurray, if i don't, we try again next time. Go is a little bit closer to the machine and c than other high level langugages i know so i'm going to use the knowledge from here to see how things are done in golang also.

This feels a lot like learning to use a framework without learning to use the language in which the framework is in first, but who cares right?

Enough of the small talk, let's dive into some networking technicalities.

#### Meeting the Structs
-------------
There are lots of structs used to work with sockets, why? Because thats just c rolls, it stores things in structs for later use.

The first struct we look at is the `addrinfo` struct. It is used to do some initial work to get socket address for later use. The name says it all, it provides info about the network address. It contains
```c
struct addrinfo {
	int ai_flags; /* */
	int ai_family; /* */
	int ai_socktype; /* */
	int ai_protocol; /* */
	size_t ai_addrlen; /* */
	struct sockaddr *ai_addr; /* */
	char *ai_canonname

	struct addrinfo *ai_next; /* */
}
```
From `struct addrinfo *ai_next`, this is a linked list pointing to the next `addrinfo`.
