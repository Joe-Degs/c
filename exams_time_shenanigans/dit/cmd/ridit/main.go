package main

import (
	"fmt"
	"io/fs"
	"log"
	"os"
)

func main() {
	f, err := os.Open("lorem.txt")
	if err != nil {
		log.Fatal(err)
	}

	// get the file stats
	fi, err := f.Stat()
	if err != nil {
		log.Fatal(err)
	}

	// everything but directories can be sent over the network
	// and since directories are the only file mode we can depend
	// on to be present on all types of systems, it serves as right
	if fi.Mode().Type() == fs.ModeDir {
		log.Fatal("cannot send directories")
	}

	fmt.Println("size", fi.Size())
}
