package dit

import (
	"bufio"
	"io"
	"os"
)

//
type FileBuffer struct {
	// request is the new request with which this file
	// buffer is created
	request *ReadWriteRequest

	// buf is a buffered reader/writer. the underlying type
	// is a bufio.Reader/Writer depending on the request
	buf bufferedObject
}

type bufferedObject interface {
	io.ReadWriteCloser
	Type() tipe
}

type tipe uint8

const (
	Reader tipe = iota
	Writer
	ReadWriter
)

type bufWrapper struct {
	t tipe
	r io.Reader
	w io.Writer
}

func wrap(t tipe, buf interface{}) *bufWrapper {
	bufer := &bufWrapper{t: t}
	if t == Reader {
		bufer.r = buf.(io.Reader)
	} else if t == Writer {
		bufer.w = buf.(io.Writer)
	}
	return bufer
}

// return the type of object in the buffer
func (b *bufWrapper) Type() tipe { return b.t }

// return the underlying buffered io object in this case
// a buffered writer. always check the type before calling
// this method
func (b *bufWrapper) BufferedWriter() *bufio.Writer {
	return b.w.(*bufio.Writer)
}

// return the underlying buffered io object in this case
// a buffered reader. always check the type before calling
// this object
func (b *bufWrapper) BufferedReader() *bufio.Reader {
	return b.r.(*bufio.Reader)
}

// read bytes from the underlying buffered io object.
// abort reads for writer objects as they do not need it.
func (b *bufWrapper) Read(p []byte) (int, error) {
	if b.t == Writer {
		return 0, nil
	}
	return io.ReadFull(b.r, p)
}

// write bytes to buffered io object. abort writes for
// reader objects.
func (b *bufWrapper) Write(p []byte) (int, error) {
	if b.t == Reader {
		return 0, nil
	}
	return b.w.Write(p)
}

// close the bufWrapper bufio objects. bufWrapper contains
// either one of writer/reader at a time so this totally
// checks out
func (b *bufWrapper) Close() error {
	if b.t == Writer {
		return b.w.(*bufio.Writer).Flush()
	}
	return nil
}

func NewFileBuffer(req *ReadWriteRequest) (*FileBuffer, error) {
	var (
		f   *os.File
		err error
		buf bufferedObject
		// fb  = &FileBuffer{req}
	)

	if op := req.opcode(); op == Rrq {
		// open file for reading
		if f, err = os.Open(req.Filename); err != nil {
			return nil, err
		}
		buf = wrap(Reader, bufio.NewReader(f))
	} else if op == Wrq {
		// open and truncate file for writing
		if f, err = os.OpenFile(req.Filename,
			os.O_WRONLY|os.O_TRUNC|os.O_APPEND|os.O_CREATE, 0666); err != nil {
			return nil, err
		}
		buf = wrap(Writer, bufio.NewWriter(f))
	}

	return &FileBuffer{req, buf}, nil
}

// Read tries to write exactly len(b) bytes into byte slice b.
// If the bytes left in the file at the end of read is not up
// to len(b) then it returns the io.EOF error to signal end of
// file. Any other error means a failure to read
func (f *FileBuffer) Read(b []byte) (int, error) {
	return f.buf.Read(b)
}

// Write
func (f *FileBuffer) Write(b []byte) (int, error) {
	return f.buf.Write(b)
}
