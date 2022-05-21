// this file contains types and functions to encode/decode packets as defined
// in rfc 1350
package dit

import (
	"encoding"
	"encoding/binary"
	"fmt"
	"unicode/utf8"
)

// Packet is a TFTP protocol packet
type Packet interface {
	encoding.BinaryUnmarshaler
}

// An Opcode is a TFTP protocol opcode representing the type of message
// a packet contains.
type Opcode uint16

const (
	// A Read Request Type
	Rrq Opcode = iota + 1

	// A Write Request Type
	Wrq

	// Data Type
	Data

	// Acknowledgement Type
	Ack

	// Error Type
	Error
)

// ReadWriteRequest is a TFTP read/write request packet as described in RFC1350,
// apendix I
type ReadWriteRequest struct {
	Opcode   Opcode
	Filename string
	Mode     string
}

func (p *ReadWriteRequest) UnmarshalBinary(b []byte) error {
	p.Opcode = Opcode(binary.BigEndian.Uint16(b[0:2]))

	// strings are null terminated bytes
	strs := b[2:]
	for i, s := range strs {

		// find the first null byte
		if s == 0 {
			// decode filename null terminated string
			if bytes := strs[:i]; len(bytes) >= 1 {
				if !utf8.Valid(bytes) {
					return fmt.Errorf("dit: filename contains illegal utf8 values, %s", bytes)
				}
				p.Filename = string(bytes)
			}

			// get the rest null terminated string as mode
			if bytes := strs[i+1 : len(strs)-1]; len(bytes) >= 1 {
				if !utf8.Valid(bytes) {
					return fmt.Errorf("dit: mode does not contain valid utf8, %s", bytes)
				}
				p.Mode = string(bytes)
			}
			break
		}

	}
	return nil
}

func (p *ReadWriteRequest) MarshalBinary() ([]byte, error) {
	data := make([]byte, 2)
	binary.BigEndian.PutUint16(data, uint16(p.Opcode))
	data = append(data, append([]byte(p.Filename), 0)...)
	data = append(data, append([]byte(p.Mode), 0)...)
	if len(data) != 2+len(p.Filename)+len(p.Mode)+2 {
		return nil, fmt.Errorf("dit: packet length not compatible with items")
	}
	return data, nil
}

// DataPacket is a TFTP data packet as described in RFC1350, apendix I
type DataPacket struct {
	Opcode      Opcode
	BlockNumber uint16
	Data        []byte
}

func (p *DataPacket) UnmarshalBinary(b []byte) error {
	p.Opcode = Opcode(binary.BigEndian.Uint16(b[0:2]))
	p.BlockNumber = binary.BigEndian.Uint16(b[2:4])
	p.Data = b[4:]
	return nil
}

// AckPacket is a TFTP acknowledgement packet as described in RFC1350,apendix I
type AckPacket struct {
	Opcode      Opcode
	BlockNumber uint16
}

func (p *AckPacket) UnmarshalBinary(b []byte) error {
	p.Opcode = Opcode(binary.BigEndian.Uint16(b[0:2]))
	p.BlockNumber = binary.BigEndian.Uint16(b[2:4])
	return nil
}

// ErrorCode represents a TFTP error code as specified in RFC1350, apendix I
type ErrorCode uint16

// TFTP error code constants as specified in RFC1350, apendix I
const (
	NotDefined ErrorCode = iota
	FileNotFound
	AccessViolation
	DiskFull
	IllegalOperation
	UnknownTID
	FileAlreadyExists
	NoSuchUser
)

// ErrorPacket is a TFTP error packet as described in RFC1350,apendix I
type ErrorPacket struct {
	Opcode    Opcode
	ErrorCode ErrorCode
	ErrMsg    string
}

func (p *ErrorPacket) UnmarshalBinary(b []byte) error {
	p.Opcode = Opcode(binary.BigEndian.Uint16(b[0:2]))
	p.ErrorCode = ErrorCode(binary.BigEndian.Uint16(b[2:4]))
	if bytes := b[4 : len(b)-1]; len(bytes) >= 1 {
		if !utf8.Valid(bytes) {
			return fmt.Errorf("dit: error messages bytes contains illegal bytes, %s", bytes)
		}
		p.ErrMsg = string(bytes)
	}
	return nil
}

// TODO(Joe-Degs): change decode packet to MarshalPacket or something and
// remove the depency on encoding. U don't really need that.
func DecodePacket(b []byte) (Packet, error) {
	opcode := Opcode(binary.BigEndian.Uint16(b[0:2]))
	switch opcode {
	case Rrq, Wrq:
		p := &ReadWriteRequest{}
		err := p.UnmarshalBinary(b)
		return p, err
	case Data:
		p := &DataPacket{}
		err := p.UnmarshalBinary(b)
		return p, err
	case Ack:
		p := &AckPacket{}
		err := p.UnmarshalBinary(b)
		return p, err
	case Error:
		p := &ErrorPacket{}
		err := p.UnmarshalBinary(b)
		return p, err
	default:
		return nil, fmt.Errorf("dit: unrecognized opcode in packet, %d", opcode)
	}
}
