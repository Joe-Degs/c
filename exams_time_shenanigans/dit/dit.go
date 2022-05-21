// dit implements the tftp protocol as specified in rfc 1350 at
// https://datatracker.ietf.org/doc/html/rfc1350
package dit

import (
	"fmt"
	"log"
	"net"
	"net/netip"
	"strings"
	"time"

	"github.com/davecgh/go-spew/spew"
)

// Conn can send and recieve files using the tftp protocol.
// It keeps the address of the TFTP server connected to maintain
// a kind of connection between server and client.
type Conn struct {
	c     *net.UDPConn
	saddr netip.AddrPort
}

// Dial connects to TFTP server and returns a `Conn` for sending/recieving
// files. It creates a udp multicast listener on an interface listening
// for all packets sent to that host.
//
// The `localAddr` can be an empty string, in which case an ephemeral port
// is requested from the system to use a the client address.
// `remoteAddr` on the other hand can never be empty or nil, it represents
// the address of TFTP server to connect to.
func Dial(network, localAddr, remoteAddr string) (*Conn, error) {
	if !strings.Contains(network, "udp") {
		return nil, fmt.Errorf("dit: protocol runs only over udp, %s", network)
	}

	if localAddr == "" {
		localAddr = "localhost:0"
	}
	laddr, err := net.ResolveUDPAddr(network, localAddr)
	if err != nil {
		return nil, err
	}

	raddr, err := net.ResolveUDPAddr(network, remoteAddr)
	if err != nil {
		return nil, err
	}

	c, err := net.ListenUDP(network, laddr)
	if err != nil {
		return nil, err
	}

	return &Conn{c, raddr.AddrPort()}, nil
}

// Write writes `len(p)` bytes to the TFTP server
func (c *Conn) Write(b []byte) (int, error) {
	if !c.saddr.IsValid() {
		return 0, fmt.Errorf("dit: invalid server address, %s", c.saddr)
	}
	return c.c.WriteToUDPAddrPort(b, c.saddr)
}

// Read waits and reads `len(p)` from the TFTP server connected to.
func (c *Conn) Read(b []byte) (int, error) {
	if !c.saddr.IsValid() {
		return 0, fmt.Errorf("dit: invalid server address, %s", c.saddr)
	}
	n, addr, err := c.c.ReadFromUDPAddrPort(b)
	fmt.Println("%s\n", addr)
	return n, err
}

// SetReadDeadline sets a deadline on reads from the TFTP server.
func (c *Conn) SetReadDeadline(n time.Duration) error {
	return c.c.SetReadDeadline(time.Now().Add(n))
}

// SetWriteDeadline sets a deadline on writes to the TFTP server.
func (c *Conn) SetWriteDeadline(n time.Duration) error {
	return c.c.SetWriteDeadline(time.Now().Add(n))
}

// ReadFrom
func (c *Conn) ReadFrom() ([]byte, error) {
	return nil, nil
}

// Snoop is a test method to send packets to a TFTP server and wait
// for responses from the server.
func (c *Conn) Snoop() {
	c.SnoopWithPacket(nil)
}

// SnoopWithPacket is `Snoop` but accepts a packet to send to server
func (c *Conn) SnoopWithPacket(pk Packet) {
	if pk == nil {
		pk = &ReadWriteRequest{
			Opcode:   Rrq,
			Filename: "path/to/file",
			Mode:     "netascii",
		}
	}

	bytes, err := pk.(*ReadWriteRequest).MarshalBinary()
	if err != nil {
		log.Fatal(err)
	}

	data := make([]byte, 256)
	for {
		// write dummy packet
		if _, err := c.Write(bytes); err != nil {
			log.Fatal(err)
		}
		log.Printf("wrote dummy packet to server; %x\n", bytes)

		// set read deadline on the connection
		c.SetReadDeadline(5 * time.Second)
		n, addr, err := c.c.ReadFromUDPAddrPort(data)
		if err != nil {
			log.Println(err.Error())
			continue
		}

		fmt.Printf("Server Addr: %s\n", addr)
		pack, err := DecodePacket(data[:n])
		if err != nil {
			log.Println(err.Error())
			continue
		}
		spew.Dump(pack)

		time.Sleep(10 * time.Second)
	}
}
