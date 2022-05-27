package main

import (
	"log"

	"github.com/Joe-Degs/dit"
)

func main() {
	conn, err := dit.Dial("udp6", "localhost:69")
	if err != nil {
		log.Fatal(err)
	}
	pk := &dit.ReadWriteRequest{
		Opcode:   dit.Rrq,
		Filename: "lorem.txt",
		Mode:     "netascii",
	}
	conn.SnoopWithPacket(pk)
}
