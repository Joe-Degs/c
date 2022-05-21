package main

import (
	"log"

	"github.com/Joe-Degs/dit"
)

func main() {
	conn, err := dit.Dial("udp6", "localhost:696", "localhost:69")
	if err != nil {
		log.Fatal(err)
	}
	pk := &dit.ReadWriteRequest{
		Opcode:   dit.Wrq,
		Filename: "file",
		Mode:     "netascii",
	}
	conn.SnoopWithPacket(pk)
}
