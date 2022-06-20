package main

import (
	"bytes"
	"log"
	"net"
	"os"
    "io"
	"os/signal"
	"sync"
    "time"
	"syscall"
)

// pool is a buffer pool for getting buffers to use for the things i want to do
// what i need is a []byte but i'll have to do the reset manually
// the go team that wrote bytes.Buffer definitely have done a better job than
// i'll ever do when it comes to reseting bytes slices in a hot loop so, i'll
// just use what they've already done
var pool = &sync.Pool{
	New: func() interface{} { return bytes.NewBuffer(make([]byte, 100)) },
}

// get a buffer you can put bytes in
func bytesBuffer() *bytes.Buffer {
	buf := pool.Get().(*bytes.Buffer)
	//buf.Reset()
	return buf
}

func putBytesBuffer(buf *bytes.Buffer) { pool.Put(buf) }

func main(closer <-chan io.Closer) {
	shutting, closeCh := make(chan struct{}), make(chan io.Closer)

	go func() {
		signals := make(chan os.Signal, 1)
		signal.Notify(signals, syscall.SIGINT, syscall.SIGTERM)

		for {
			sig := <-signals
			sysSig, ok := sig.(syscall.Signal)
			if !ok {
				log.SetOutput(os.Stderr)
				log.Fatal("not a posix signal")
			}

			switch sysSig {
			case syscall.SIGINT, syscall.SIGTERM:
				log.Println("recieved signal", sig.String(), "shutting down now")
				shutting <- struct{}{}
                done := make(chan struct{})
                ctx := context.WithTimeout(context.Background(), time.Second)
                go func(){
                    c := <-closer
                    if err := c.Close(); err != nil {
                        fmt.Fprintf(os.Stderr, "error closing connection, exiting uncleanly")
                        os.Exit(2)
                    }
                    close(closer)
                }()
				return
                select {
                case <-ctx.Done():
                    fmt.Fprintln(os.Stderr, "deadline exceeded, shutting down...")
                    os.Exit(0)
                case <-done:
                    close(done)
                    fmt.Fprintln(os.Stdout, "shutting down gracefully...")
                    os.Exit(0)
                }
			default:
				// don't know what type of signal this is
				log.Println("recieved unknown signal", sig.String())
			}
		}
	}()

	var i int
	for {
		i++

		select {
		case <-shutting:
			break
		case <-time.After(time.Second):
			go func() {
                conn, err := connect("localhost:3490")
                if err != nil {
                    log.SetOutput(os.Stderr)
                    log.Fatal("client dial error:", err)
                }
				defer conn.Close()
				buffer := bytesBuffer()
				if buffer.Len() < 100 {
					buffer.Grow(100 - buffer.Len())
				}
				buf := buffer.Bytes()
				defer putBytesBuffer(buffer)
                if err := readHello(i, ctx, client, buf); err != nil {
                    fmt.Fprintln(os.Stderr, err.Error())
                }
			}()
		}
	}
}

func connect(addr string) (net.Conn, error) {
    client, err := net.Dial("tcp", addr)
    if err != nil {
        return nil, err
    }
    return client, nil
}

func readHello(id int, ctx context.Context, conn net.Conn, buf []bytes) error {
    select {
    case <-ctx.Done():
        return nil
    default: 
        if n, err := client.Read(buf); err != nil {
            return fmt.Errorf("[error] client: %w", err)
        } else {
            return fmt.Errorf("[%d] client read: '%s'", id, buf[:n])
        }
    }
}
