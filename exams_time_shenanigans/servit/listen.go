package servit

import "net"

type ServitListener struct {
	l *net.TCPListener
}

// Accept waits for and returns the next connection to the listener.
func (s *ServitListener) Accept() (net.Conn, error) {
	conn, err := s.l.AcceptTCP()
	if err != nil {
		return nil, err
	}
	return &Conn{conn}, nil
}

// Close closes the listener.
// Any blocked Accept operations will be unblocked and return errors.
func (s *ServitListener) Close() error {
	return s.l.Close()
}

// Addr returns the listener's network address.
func (s *ServitListener) Addr() net.Addr {
	return s.l.Addr()
}
