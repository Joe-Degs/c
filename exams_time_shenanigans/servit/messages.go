package servit

import (
	"encoding"
	"encoding/binary"
	"fmt"
)

// Message an abstraction for bytes recieved from the connection
type Message interface {
	encoding.BinaryMarshaler
	encoding.BinaryUnmarshaler
}

type PacketType uint8

const (
	Hello PacketType = iota
	Command
)

// Header is a servit packet header
// the packet structure of header will look something like this
//
// |0       |1     |23      |45    | -> index
// ------------------------------------
// |0       |0     |00      |00    | -> number of bytes
// -----------------------------------
// |version | type | length | files| -> fields
//
type Header struct {
	// Version of protocol used in communication
	Version uint8

	// Type of message. informs how we to decode bytes
	Type PacketType

	// Length of the payload in bytes
	Length uint16

	// Number of files in the message payload. zero if there are no files
	Files uint16
}

func getHeader(b []byte) (Header, error) {
	if l := len(b); l < 6 {
		fmt.Errorf("servit: packet should be greater of equal to header %d", l)
	}
	header := Header{
		Version: binary.BigEndian.Uint8(b[0]),
		Type:    binary.BigEndian.Uint8(b[1]),
		Length:  binary.BigEndian.Uint8(b[2:3]),
		Files:   binary.BigEndian.Uint8(b[4:5]),
	}
}

// HelloPacket structure
type HelloPacket struct {
	Header  Header
	Payload []byte
}

func (h *HelloPacket) UnmarshalBinary(b []byte) error {
	header, err := getHeader(b)
	if err != nil {
		return err
	}

	if header.Type != Hello {
		fmt.Errorf("servit: expected a hello packet type %d", header.Type)
	}

	return nil
}

func (h *HelloPacket) MarshalBinary() ([]byte, error) {
	//header := make([]byte, )
	return nil, nil
}
