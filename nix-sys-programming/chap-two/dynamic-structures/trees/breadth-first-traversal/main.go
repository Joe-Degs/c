package main

import "fmt"

// a node in a binary tree
type node struct {
	left, right *node
	value       int
}

// a binary tree
type tree struct {
	root *node
}

func insert(to *node, n *node) *node {
	if to == nil {
		return n
	}
	if n.value < to.value {
		to.left = insert(to.left, n)
	} else if n.value > to.value {
		to.right = insert(to.right, n)
	}
	return n
}

func (t *tree) append(n *node) *node {
	return insert(t.root, n)
}

// return a new empty binary tree
func New() *tree {
	return new(tree)
}

func main() {
	t := New()
	// t.root = &node{value: 13}
	values := []int{5, 3, 2, 4, 7, 6, 8}

	for i := 0; i < len(values); i++ {
		n := new(node)
		n.value = values[i]
		t.append(n)
	}

	fmt.Println(t, t.root)
}
