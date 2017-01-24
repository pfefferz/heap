/** @file test_heap.c
 *  @brief Test heap
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
#include <stdio.h>
#include <stdlib.h>
#include "heap.h"

enum heap_type heap_types[] = {min_heap, max_heap};

static char test_desc_0[] = "Create, insert 1, extrct and del heap, both types";
int test_0(void)
{
	struct heap *heap;

	int i;
	for (i = 0; i < ARRAY_SIZE(heap_types); i++) {
		heap = new_heap(heap_types[i]);
		if (heap == NULL) {
			printf("Could not alloc a heap\n");
			return -1;
		}

		int ret;
		ret = heap_insert(heap, 0);
		if (ret != 1) {
			printf("Could not insert into heap\n");
			return -1;
		}

		int val;
		if (heap_extract(heap, &val) == -1) {
			printf("Could not extract a value from the heap\n");
			return -1;
		}

		if (val != 0) {
			printf("Did not extract the exp value, got %i\n", val);
			return 0;
		}

		if (delete_heap(heap) != 0)
			printf("Could not delete heap\n");
	}

	return 1;
}

static char test_desc_1[] = "Create, insert 4, extrct and del heap, both types";
int test_1(void)
{
	struct heap *heap;

	int in_vals[][4] = {
		{10, 9, 8, 0},  /* for min_heap */
		{0, 8, 9, 10}   /* for max_heap */
	};

	int out_vals[][4] = {
		{0, 8, 9, 10},
		{10, 9, 8, 0}
	};

	int i;
	for (i = 0; i < ARRAY_SIZE(heap_types); i++) {
		heap = new_heap(heap_types[i]);
		if (heap == NULL) {
			printf("Could not alloc a heap\n");
			return -1;
		}

		int j;
		for (j = 0; j < ARRAY_SIZE(in_vals[i]); j++) {
			int ret = heap_insert(heap, in_vals[i][j]);
			if (ret != 1) {
				printf("Could not insert into heap\n");
				return -1;
			}
		}

		int val;
		for (j = 0; j < ARRAY_SIZE(out_vals[i]); j++) {
			int ret = heap_extract(heap, &val);

			if (ret == -1) {
				printf("Err ext a val from the heap\n");
				return -1;
			}

			if (ret == 0) {
				printf("No item to extract from heap\n");
				return -1;
			}

			if (val != out_vals[i][j]) {
				printf("Did not ext cor val. Got %i exp %i\n",
					val, out_vals[i][j]);
				return -1;
			}
		}

		if (delete_heap(heap) != 0)
			printf("Could not delete heap\n");
	}
	return 1;
}

#define RAN_ELMS 10000

static char test_desc_2[] = "Cret&del, ins 10k rnd at once, ext all, bth type";
int test_2(void)
{
	struct heap *heap;

	int i;
	for (i = 0; i < ARRAY_SIZE(heap_types); i++) {
		heap = new_heap(heap_types[i]);
		if (heap == NULL) {
			printf("Could not alloc a heap\n");
			return -1;
		}

		int j;
		for (j = 0; j < RAN_ELMS; j++) {
			int ret = heap_insert(heap, rand());
			if (ret != 1) {
				printf("Could not insert into heap\n");
				return -1;
			}
		}

		int val;
		int last_val;
		for (j = 0; j < RAN_ELMS; j++) {
			int ret = heap_extract(heap, &val);

			if (ret == -1) {
				printf("Err ext a val from the heap\n");
				return -1;
			}

			if (ret == 0) {
				printf("No item to extract from heap\n");
				return -1;
			}

			if (j > 0 && val < last_val &&
					heap->heap_type == min_heap) {
				printf("Got larg val: %i, exp a smal val: %i\n",
						val, last_val);
				return -1;
			} else if (j > 0 &&
					val > last_val &&
					heap->heap_type == max_heap) {
				printf("Got smal val: %i, exp a larg val: %i\n",
						val, last_val);
			}

			last_val = val;
		}

		if (delete_heap(heap) != 0)
			printf("Could not delete heap\n");
	}
	return 1;
}

static char test_desc_3[] = "Testing heap_type_to_str";
int test_3(void)
{
	int i;
	for (i = 0; i < ARRAY_SIZE(heap_types); i++) {
		char *heap_type_str;
		int ret = heap_type_to_str(heap_types[i], &heap_type_str);
		if (ret != 1) {
			printf("heap_type_to_str had an error %i\n", ret);
			return 0;
		}
	}

	return 1;
}



struct test {
	int (*test_func)(void);
	char *test_desc;
} tests[] = {
	{test_0, test_desc_0},
	{test_1, test_desc_1},
	{test_2, test_desc_2},
	{test_3, test_desc_3}
};

int main(int argc, char *argv[])
{
	int ret, i;

	for (i = 0; i < ARRAY_SIZE(tests); i++) {
		ret = tests[i].test_func();
		switch (ret) {
		case -1:
			printf("%s: ", "SYSERR");
			break;
		case 0:
			printf("%s: ", "FAILED");
			break;
		case 1:
			printf("%s: ", "PASSED");
			break;
		default:
			printf("%s: ", "UNEXPE");
			break;
		}
		printf("%s", tests[i].test_desc);
		printf("\n");
	}

	return 0;
}
