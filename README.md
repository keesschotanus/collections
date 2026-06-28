# Collections

Basic implementation of some collections in the C programming language.
We all need lists, maps and trees and it would be wise to use a standard library for this,
but why use a standard library in minutes when you can experience the agony of writing some of these yourself?

## Design goals

- Keep it simple
- Hide implementation details
- Make collections generic
	- Making collections support multiple types
	- Not using Macros
- Allow using multiple collections simultaneously

## Current implementations

- Lists
	- [list](include/list/list.h) (a simple dynamic list)
	- [doubly linked list](include/list/doubly_linked_list.h)
- Maps
	- [hash map](include/map/hash_map.h)
- Trees
	- [binary tree](include/tree/binary_tree.h)
	- [binary search tree](include/tree/binary_search_tree.h)

## Using the Makefile

```bash
# Build the collections and run the unit tests
make
```

```bash
# Build the apidoc
make apidoc
```
The command above requires [Doxygen](https://www.doxygen.nl/index.html)
and Doxygen in turns requires [Graphviz](https://graphviz.org/).

```bash
# Build the collections as a (shared) library
make libs
```


