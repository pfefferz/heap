Heap
====

Binary heap code and test bench

Copyright 2017 Zach Pfeffer

pfefferz@gmail.com

Overview
--------

Functions to create and delete binary heaps of ints and insert and extract int
elems from heaps and a test bench to test the implementation. The functions are
built to take heaps and elements and return status so that NULLs and invariant
violations can be caught, e.g. the heap has been initialized.

ARRAY_SIZE is also implemented, demonstrated and unit tested.

I wrote this to demonstrate good-ish implementation practices. If something
could be better, please let me know.  For instance, I've used Doxygen to
document the functions as opposed to
http://www.sphinx-doc.org/en/stable/domains.html. What are your thoughts?

Usage
-----

To build the list test bench type:
`make`

To run the list test bench type:
`./test_heap`

See test_heap.c to see how to use the functions.

References
----------

I used the general approach found in
https://en.wikipedia.org/w/index.php?title=Binary_heap&oldid=759621440

Referenced this when creating this file:
https://raw.githubusercontent.com/google/protobuf/master/README.md
I <3 protocol buffers!

I found the MIT license text at http://choosealicense.com/licenses/mit/ which I
found from https://github.com/blog/1530-choosing-an-open-source-license

The _ after the HEAP_H_ came from:
https://google.github.io/styleguide/cppguide.html#The__define_Guard

The use of starting the first Doxygen Special Command after /** and using 2
spaces indent from * came from:
https://www.cs.cmu.edu/~409/doc/doxygen.html

I found the basic Makefile from:
An Introduction to GCC by Brian Gough ISBN 0-9541617-9-3

Most of this work was done so that I could become proficient with Vim. During
most of it I read (and reread) Vi IMproved-- Vim by Steve Oualline ISBN
0-7357-1001-5. I would recommend this book to learn Vim.
