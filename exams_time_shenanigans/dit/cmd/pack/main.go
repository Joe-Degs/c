package main

import (
	"fmt"
	"log"

	"github.com/Joe-Degs/dit"
	"github.com/davecgh/go-spew/spew"
)

func main() {
	packet := &dit.ReadWriteRequest{
		Opcode:   dit.Rrq,
		Filename: "path/to/file",
		Mode:     "netascii",
	}
	// spew.Dump(packet)
	bytes, err := packet.MarshalBinary()
	if err != nil {
		log.Fatal(err)
	}
	spew.Dump(bytes)

	p := &dit.ReadWriteRequest{}
	if err := p.UnmarshalBinary(bytes); err != nil {
		log.Fatal(err)
	}

	fmt.Println("")
	spew.Dump(p)
}
