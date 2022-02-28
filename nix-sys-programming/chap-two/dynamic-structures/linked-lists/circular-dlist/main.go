package main

import "fmt"

// node represents a single element in a list
type node struct {
	next, prev *node
	value      interface{}
}

func (n node) Next() *node { return n.next }
func (n node) Prev() *node { return n.prev }

// list represents the whole list
type list struct {
	head *node
}

// head returns the list head
func (l list) Head() *node { return l.head }

// return the first element in the list
func (l *list) front() *node {
	return l.head.next
}

// return the last element in the list
func (l *list) back() *node {
	return l.head.prev
}

// print the list
func (l list) print() {
	for tmp := l.front(); tmp != l.Head(); tmp = tmp.Next() {
		fmt.Printf("[%v]->", tmp.value)
	}
	fmt.Println("NULL")
}

// print the list from the last element to the first
func (l list) printBackward() {
	for tmp := l.back(); tmp != l.Head(); tmp = tmp.Prev() {
		fmt.Printf("[%v]->", tmp.value)
	}
	fmt.Println("NULL")
}

// append adds a new node to end of list
func (l *list) append(n *node) {
	tmp := l.back()
	n.next = tmp.next
	tmp.next.prev = n
	n.prev = tmp
	tmp.next = n
}

// pushFront pushes a new element to the front of the list
func (l *list) pushFront(n *node) {
	tmp := l.front()
	n.prev = tmp.prev
	tmp.prev.next = n
	tmp.prev = n
	n.next = tmp
}

// search for element with value `v` in list
func (l *list) search(v interface{}) *node {
	for tmp := l.front(); tmp != l.Head(); tmp = tmp.Next() {
		if tmp.value == v {
			return tmp
		}
	}
	return nil
}

// remove element specified element from list and return it
// make sure element is a list element before you pass it to this method
func (l *list) remove(del *node) *node {
	if del == l.head || del == nil {
		return nil
	}
	del.prev.next = del.next
	del.next.prev = del.prev
	return del
}

// pop removes the last element in the list and returns it.
func (l *list) pop() *node {
	return l.remove(l.back())
}

// remove element from front of list and return it
func (l *list) popFront() *node {
	return l.remove(l.front())
}

// delete removes element with value `v` and returns it
func (l *list) delete(v interface{}) *node {
	return l.remove(l.search(v))
}

// New returns a new empty list.
func New() *list {
	n := new(list)
	n.head = new(node)
	n.head.next, n.head.prev = n.head, n.head
	return n
}

// A fifo queue can be built on this very easily, because the list already
// contains all the necessary functions to implement a fifo queue.
type queue struct {
	head *list
}

// enqueue takes a nodes and joins it to the queue
func (q *queue) enqueue(n *node) {
	return q.head.append(n)
}

// dequeue returns the next person in the queue
func (q *queue) dequeue() *node {
	return q.head.popFront()

}

func NewQueue() *queue {
	return &queue{head: New()}
}

func main() {
	listhead := New()

	fmt.Println("Appending to list")
	for i := 1; i <= 3; i++ {
		newNode := new(node)
		newNode.value = i
		listhead.append(newNode)
	}
	listhead.print()

	fmt.Println("Pushing to Front of list")
	for i := 0; i < 1; i++ {
		newNode := new(node)
		newNode.value = i
		listhead.pushFront(newNode)
	}
	listhead.print()
	listhead.printBackward()

	fmt.Println("Deleting element with key '0' from list")
	listhead.delete(0)
	listhead.print()

	fmt.Println("Pop from the back of the list")
	listhead.pop()
	listhead.print()

	fmt.Println("Pop from the front of the list")
	listhead.popFront()
	listhead.print()
}
