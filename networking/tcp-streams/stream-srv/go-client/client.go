package main

import (
	"bytes"
	"context"
	"log"
	"net"
	"os"
	"os/signal"
	"sync"
	"syscall"
	"time"
)

// pool is a buffer pool for getting buffers to use for the things i want to do
// what i need is a []byte but i'll have to do the reset manually
// the go team that wrote bytes.Buffer definitely have done a better job than
// i'll ever do when it comes to reseting bytes slices in a hot loop so, i'll
// just use what they've already done
var pool = &sync.Pool{
	New: func() interface{} { return new(bytes.Buffer) },
}

// get a buffer you can put bytes in
func buffer(size int) *bytes.Buffer {
	buf := pool.Get().(*bytes.Buffer)
	if buf.Len() < size {
		diff := size - buf.Len()
		buf.Write(make([]byte, diff))
	}
	return buf
}

func putBuffer(buf *bytes.Buffer) { pool.Put(buf) }

func main() {

	go func(cancel context.CancelFunc) {
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
				log.Println("recieved signal", sig.String(), "shutting down now...")
				cancel()
				time.Sleep(2 * time.Second)
				os.Exit(0)
			default:
				// don't know what type of signal this is
				log.Printf("recieved unknown signal: '%s'", sig.String())
			}
		}
	}(Main())

	select {}
}

func Main() context.CancelFunc {
	ctx, cancel := context.WithCancel(context.Background())

	openConn := func(network, addr string) net.Conn {
		conn, err := net.Dial(network, addr)
		if err != nil {
			log.Printf("[error] openConn: %s", err.Error())
			return nil
		}
		return conn
	}

	readHello := func(f func(string, string) net.Conn, b []byte, network, addr string) int {
		conn := f(network, addr)
		defer conn.Close()
		if conn == nil {
			return 0
		}

		if n, err := conn.Read(b); err != nil {
			log.Printf("[error] readHello: %s", err.Error())
			return 0
		} else {
			return n
		}
	}

	go func(ctx context.Context) {
		var num int
		for {
			num++
			select {
			case <-ctx.Done():
				return
			case <-time.After(time.Millisecond):
				go func(idx int) {
					bytesBuf := buffer(200)
					defer putBuffer(bytesBuf)
					buf := bytesBuf.Bytes()
					n := readHello(openConn, buf, "tcp", "localhost:3490")
					if n == 0 {
						return
					}
					log.Printf("[%d] client read: '%s'\n", idx, buf[:n])
				}(num)
			}
		}
	}(ctx)

	return cancel
}
