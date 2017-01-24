#ifndef HEAP_H_
#define HEAP_H_

/** @file heap.h
 *  @brief Heap header
 *
 *  @copyright
 *  MIT License
 *
 *  Copyright (c) 2017 Zach Pfeffer
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to
 *  deal in the Software without restriction, including without limitation the
 *  rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 *  sell copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 *  IN THE SOFTWARE.
 */

/* This should go in a "util", put here for eleminate a util.h */
#define ARRAY_SIZE(a) (sizeof(a)/sizeof(*a))

enum heap_type {
	min_heap,
	max_heap
};

struct heap {
	int initialized;
	enum heap_type heap_type;
	int *vals;
	unsigned int len;
};

struct heap *new_heap(enum heap_type heap_type);
int delete_heap(struct heap *heap);
int heap_insert(struct heap *heap, int val);
int heap_extract(struct heap *heap, int *val);
int heap_type_to_str(enum heap_type heap_type, char **str);

#endif /* HEAP_H_ */
