trees are data structures composed of multi-levels of lists. It is a node that
contains references to other nodes. it can be defined recursively as;
    
    node: value [&node[1], ..., &node[k]]

-> the first node in a tree is called the root node
-> nodes contain references to other list called to children nodes and the node
containing the references is the parent node.
-> each node has a unique parent but each node may have variable children nodes
including none at all.
-> trees in computer science are mostly drawn upside down as opposed to
conventional diagrams of trees. ex

        [root node]
        /    |     \
    [ k ]  [ k ]  [ k ]
             |      /   \
            [k]    [k]   [ k ]
                    |
                    [k]
                    / \
                   [k] [k] 
