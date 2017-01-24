/** @file heap.c
 *  @brief Binary heap create, delete; insert and remove ints
 *
 *  This implementation is largely for demonstration purposes. Functions
 *  generally return -1 when the callee gives the function something that its
 *  not expecting. Functions return 0 to indicate false and 1 to indicate
 *  success.
 *
 *  The implementation uses the implicit data structure and general approach
 *  described in:
 *  https://en.wikipedia.org/w/index.php?title=Binary_heap&oldid=759621440
 *
 *  The array of ints is grown and shrunk one at a time using realloc.
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

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "heap.h"

static int left_child(int i)
{
	return 2*i + 1;
}

static int right_child(int i)
{
	return 2*i + 2;
}

static int parent(int i)
{
	return (i - 1)/2;
}

static int property(struct heap *heap, int parent, int child)
{
	if (heap->heap_type == min_heap)
		return heap->vals[parent] <= heap->vals[child];
	else if (heap->heap_type == max_heap)
		return heap->vals[parent] >= heap->vals[child];
	else
		return -1;
}

static void swap(struct heap *heap, int parent, int child)
{
	int tmp = heap->vals[parent];

	heap->vals[parent] = heap->vals[child];
	heap->vals[child] = tmp;
}

static void heap_initialize(struct heap *heap, enum heap_type heap_type)
{
	if (heap == NULL)
		return;

	heap->heap_type = heap_type;
	heap->vals = NULL;
	heap->len = 0;
	heap->initialized = 1;
}

/** @brief Instantiate a new heap
 *
 *  @param[in] heap_type min_heap or max_heap
 *
 *  @retval NULL could not allocate memory for a heap or heap_type is not valid
 *  @retval not NULL new heap, allocated and initialized
 *
 */
struct heap *new_heap(enum heap_type heap_type)
{
	struct heap *heap;

	heap = malloc(sizeof(*heap));
	if (heap == NULL) {
		printf("Could not allocate heap\n");
		return NULL;
	}

	heap_initialize(heap, heap_type);

	return heap;
}

/** @brief Delete a heap instance created using new_heap
 *
 *  @retval -1 heap is NULL, not initialized or not empty
 *  @retval 0 heap has been deleted
 *
 */
int delete_heap(struct heap *heap)
{
	if (heap == NULL) {
		printf("No heap to delete\n");
		return -1;
	}

	if (heap->initialized != 1) {
		printf("Heap not initialized\n");
		return -1;
	}

	if (heap->vals != NULL) {
		printf("Heap not empty");
		return -1;
	}

	free(heap);

	return 0;
}


/** @brief Insert an element (int) into a heap
 *
 *  @param[in] heap The heap to insert a value into
 *  @param[in] val The value to insert
 *
 *  @retval -1 heap is NULL, not init'd, could not realloc or heap type is bad
 *  @retval 1 val has been inserted
 *
 */
int heap_insert(struct heap *heap, int val)
{
	if (heap == NULL)
		return -1;

	if (heap->initialized != 1)
		return -1;

	int *new_vals = NULL;
	new_vals = realloc(heap->vals, (heap->len + 1) * sizeof(*heap->vals));
	if (new_vals == NULL) {
		printf("Could not grow heap by 1\n");
		return -1;
	}
	heap->vals = new_vals;
	heap->len++;

	/* add to the end
	 *  compare to parent
	 *  if less (if max_heap) or greater (if min_heap) than swap
	 */
	heap->vals[heap->len-1] = val;

	int i;
	for (i = heap->len - 1;
		i != 0 && (property(heap, parent(i), i) == 0);
		i = parent(i)) {


		int ret = property(heap, parent(i), i);
		if (ret == -1) {
			printf("Error with property function");
			return -1;
		}

		swap(heap, parent(i), i);
	}

	return 1;
}

static int has_left(int i, struct heap *heap)
{
	return (left_child(i) <= (heap->len - 1));
}

static int has_right(int i, struct heap *heap)
{
	return (right_child(i) <= (heap->len - 1));
}

static int no_children(int i, struct heap *heap)
{
	return (!has_left(i, heap) && !has_right(i, heap));
}

static int left_no_right(int i, struct heap *heap)
{
	return (has_left(i, heap) && !has_right(i, heap));
}

static int smaller_child(int i, struct heap *heap)
{
	if (heap->vals[left_child(i)] < heap->vals[right_child(i)])
		return left_child(i);
	else
		return right_child(i);
}

static int larger_child(int i, struct heap *heap)
{
	if (heap->vals[left_child(i)] > heap->vals[right_child(i)])
		return left_child(i);
	else
		return right_child(i);
}

/** @brief Extract element (an int) from a heap
 *
 *  @param[in] heap The heap to extract an element from
 *  @param[out] val A pointer to an int to return the extracted value
 *
 *  @retval -1 heap or val is NULL,
 *  @retval 1 val has been inserted
 *
 */
int heap_extract(struct heap *heap, int *val)
{
	if (heap == NULL)
		return -1;

	if (val == NULL)
		return -1;

	if (heap->initialized != 1)
		return -1;

	if (heap->vals == NULL)
		return -1;

	if (heap->len == 0)
		return 0;

	*val = heap->vals[0];

	if (heap->len == 1) {
		free(heap->vals);
		heap->vals = NULL;
		heap->len = 0;
		return 1;
	}

	/* at this point I have at least 2 elms */

	/* Replace the root of the heap with the last elm on the last level
	 * Compare with child, if okay stop
	 * Else, swap
	 */

	/* replace the root */
	heap->vals[0] = heap->vals[heap->len - 1];

	int *new_vals = NULL;
	/* shrink by 1 */
	new_vals = realloc(heap->vals, (heap->len - 1) * sizeof(*heap->vals));
	if (new_vals == NULL) {
		printf("Could not shrink heap by 1\n");
		return -1;
	}
	heap->vals = new_vals;
	heap->len--;

	int i = 0;

	while (1) {
		if (no_children(i, heap)) {
			break;
		} else if (left_no_right(i, heap)) {
			int ret = property(heap, i, left_child(i));
			if (ret == -1) {
				printf("Error with property function");
				return -1;
			}

			if (ret == 0)
				swap(heap, i, left_child(i));
			break;
		} else {
			if (heap->heap_type == min_heap) {
				int the_smaller_child = smaller_child(i, heap);

				int ret = property(heap, i, the_smaller_child);
				if (ret == -1) {
					printf("Error with property function");
					return -1;
				}

				if (ret == 0)
					swap(heap, i, the_smaller_child);
				i = the_smaller_child;
			} else if (heap->heap_type == max_heap) {
				int the_larger_child = larger_child(i, heap);

				int ret = property(heap, i, the_larger_child);
				if (ret == -1) {
					printf("Error with property function");
					return -1;
				}

				if (ret == 0)
					swap(heap, i, the_larger_child);
				i = the_larger_child;
			} else {
				printf("Unknown heap type\n");
			}
		}
	}

	return 1;
}

/** @brief Given a heap type, return a string describing the heap type
 *
 *  @param[in] heap_type The heap type you'd like to string representation of
 *  @param[out] *str The string will be, min_heap or max_heap
 *
 *  @retval -1 heap or val is NULL,
 *  @retval 1 val has been inserted
 *
 *  @remark
 *  This function is not core to the implementation. I wrote it to demonstrate
 *  a way to associate a string with an enum while not assuming anything about
 *  the enum values.
 *
 */
int heap_type_to_str(enum heap_type heap_type, char **str)
{
	static struct heap_type_and_str {
		enum heap_type heap_type;
		char *heap_type_str;
	} heap_types_and_strings[] = {
		{ min_heap, "min_heap" },
		{ max_heap, "max_heap" }
	};

	if (*str == NULL)
		return -1;

	int i;
	for (i = 0; i < ARRAY_SIZE(heap_types_and_strings); i++) {
		if (heap_type == heap_types_and_strings[i].heap_type) {
			*str = heap_types_and_strings[i].heap_type_str;
			return 1;
		}
	}

	return 0;
}
