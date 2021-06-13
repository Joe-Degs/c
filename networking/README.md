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

There are tons of structures involved when dealing with this things. I've
continued taking notes in my notebook so i won't be writing anything here
anymore.

This tutorial mostly deals with IPv4 and IPv6 sockets so most of the structures
are related to those sockets.

`struct addrinfo` is a linked list and it contains `struct sockaddr` that
contains info about the address type the socket relates to. But it is somewhat
generic so there are specific structures, `struct sockaddr_in` representing an
IPv4 socket address structure and `struct sockaddr_in6` representing the an IPv6
address structure. You mostly work with a specific internet socket address type and then
when sending over the wire time comes you cast to a `struct sockaddr` and then you send.

If you want to be IP address type agnostic or don't know the type of socket address type you are
recieving over the wire, theres a socket addr type `struct sockaddr_storage` that
can contain either address types and can be cast safely to any of the specific
ip address structures.

Always convert all types to network byte order using the provided functions
before sending over the wire. Very very important!.
