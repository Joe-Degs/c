package main

import (
	"context"
	"log"
	"net"
	"syscall"

	"github.com/Joe-Degs/dit"
	"github.com/davecgh/go-spew/spew"
	"golang.org/x/sys/unix"
)

func main() {
	config := &net.ListenConfig{
		Control: func(net, addr string, c syscall.RawConn) error {
			return c.Control(func(fd uintptr) {
				// set socket option to let multiple processes to
				// listen on the same port
				unix.SetsockoptInt(int(fd), unix.SOL_SOCKET,
					syscall.SO_REUSEADDR, 1)

				// set the priority of the socket high to recieve the
				// fucking packets becuase no packets are coming
				// socket priority [low - high] => [1 - 7]
				unix.SetsockoptInt(int(fd), unix.SOL_SOCKET,
					syscall.SO_PRIORITY, 7)
			})
		},
	}

	listener, err := dit.ListenConfigConn(context.Background(), config,
		"udp6", "localhost:69")
	if err != nil {
		log.Fatal(err)
	}

	log.Printf("TFTP Server listening %s", listener.Addr())

	for {
		conn, err := listener.Accept()
		if err != nil {
			log.Fatal(err)
		}

		// log.Printf("recieved request from %s", conn.DestinationTID())
		spew.Dump(conn)
		conn.Close()
	}
}
