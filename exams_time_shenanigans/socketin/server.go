package main

import (
	"bufio"
	"flag"
	"fmt"
	"log"
	"net"
	"os"
)

func main() {
	var file string
	flag.StringVar(&file, "f", "file to serve", "-f /path/to/file")
	flag.Parse()

	sock, err := net.Listen("tcp", "localhost:6969")
	if err != nil {
		log.Fatal(err)
	}
	defer sock.Close()
	log.Printf("[Server] (addr) %s (serving file) %s\n ", sock.Addr(), file)

	for {
		client, err := sock.Accept()
		if err != nil {
			log.Printf("[Error] %v\n", err)
			continue
		}
		fmt.Println("")
		log.Printf("[Remote Client] (addr) %s\n", client.RemoteAddr())

		go func(client net.Conn) {
			defer client.Close()

			f, err := os.Open(file)
			if err != nil {
				log.Fatal(err)
			}

			buf := bufio.NewReader(f)
			log.Printf("[BufIO Reader] (buffer size) %dKB\n", buf.Size()/1024)

			sent, err := buf.WriteTo(client)
			if err != nil {
				log.Fatal(err)
			}
			log.Printf("[Total Bytes Sent] (size) %dKB", sent/1024)
		}(client)
	}
}
