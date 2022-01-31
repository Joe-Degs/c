package main

import (
	"testing"
	"time"
)

func BenchmarkClient(b *testing.B) {
	for n := 0; n < b.N; n++ {
		cancel := Main()
		time.AfterFunc(time.Minute, func() { cancel() })
	}
}
