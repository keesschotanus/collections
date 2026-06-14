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
	- [list](include/list.h) (a simple dynamic list)
	- [doubly linked list](include/doubly_linked_list.h)
- Maps
	- [hash map](include/hash_map.h)

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


