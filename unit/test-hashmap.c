/*
 *
 *  Embedded Linux library
 *
 *  Copyright (C) 2011-2012  Intel Corporation. All rights reserved.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <assert.h>

#include <ell/ell.h>

static void test_ptr(const void *test_data)
{
	struct l_hashmap *hashmap;
	unsigned int n, i;

	hashmap = l_hashmap_new();
	assert(hashmap);

	for (n = 0; n < 1024; n++) {
		for (i = 1; i < n + 2; i++) {
			if (!l_hashmap_insert(hashmap, L_UINT_TO_PTR(i),
							L_UINT_TO_PTR(i)))
				printf("[%u] insert error: %u\n", n, i);
		}

		if (l_hashmap_size(hashmap) != n + 1) {
			printf("[%u] size mismatch: %u\n", n,
						l_hashmap_size(hashmap));
		}

		for (i = 1; i < n + 2; i++) {
			void *ptr = l_hashmap_lookup(hashmap, L_UINT_TO_PTR(i));

			if (!ptr) {
				printf("[%u] lookup empty: %u\n", n, i);
				continue;
			}

			if (i != L_PTR_TO_UINT(ptr)) {
				printf("[%u] lookup misatch: %u != %u\n",
						n, i, L_PTR_TO_UINT(ptr));
			}
		}

		for (i = 1; i < n + 2; i++) {
			void *ptr = l_hashmap_remove(hashmap, L_UINT_TO_PTR(i));

			if (!ptr) {
				printf("[%u] remove empty: %u\n", n, i);
				continue;
			}

			if (i != L_PTR_TO_UINT(ptr)) {
				printf("[%u] remove misatch: %u != %u\n",
						n, i, L_PTR_TO_UINT(ptr));
			}
		}

		if (!l_hashmap_isempty(hashmap)) {
			printf("[%u] not empty: %u\n", n,
						l_hashmap_size(hashmap));
		}
	}

	l_hashmap_destroy(hashmap, NULL);
}

static void test_str(const void *test_data)
{
	struct l_hashmap *hashmap;
	const void *ptr;
	const char **itr, *strings[] = {
		"hello",
		"world",
		"a",
		"a long key here, bla bla bla bla bla hey! enough?",
		NULL
	};

	hashmap = l_hashmap_string_new();
	assert(hashmap);

	/* basics */
	for (itr = strings; *itr != NULL; itr++) {
		assert(l_hashmap_insert(hashmap, *itr, itr));
		ptr = l_hashmap_lookup(hashmap, *itr);
		assert(ptr == itr);
	}

	assert(l_hashmap_lookup(hashmap, "not in hash") == NULL);

	/* remove and look post */
	for (itr = strings; *itr != NULL; itr++) {
		const char **subitr;
		assert(l_hashmap_remove(hashmap, *itr));
		ptr = l_hashmap_lookup(hashmap, *itr);
		assert(ptr == NULL);

		for (subitr = itr + 1; *subitr != NULL; subitr++) {
			ptr = l_hashmap_lookup(hashmap, *subitr);
			assert(ptr == subitr);
		}
	}
	assert(l_hashmap_size(hashmap) == 0);

	/* add again, to remove backwards */
	for (itr = strings; *itr != NULL; itr++) {
		assert(l_hashmap_insert(hashmap, *itr, itr));
		ptr = l_hashmap_lookup(hashmap, *itr);
		assert(ptr == itr);
	}

	/* remove and look previous */
	for (itr--; itr >= strings; itr--) {
		const char **subitr;
		assert(l_hashmap_remove(hashmap, *itr));
		ptr = l_hashmap_lookup(hashmap, *itr);
		assert(ptr == NULL);

		for (subitr = strings; subitr < itr; subitr++) {
			ptr = l_hashmap_lookup(hashmap, *subitr);
			assert(ptr == subitr);
		}
	}
	assert(l_hashmap_size(hashmap) == 0);

	/* force different insert and lookup pointers */
	for (itr = strings; *itr != NULL; itr++) {
		char *str = l_strdup(*itr);
		assert(l_hashmap_insert(hashmap, *itr, itr));
		ptr = l_hashmap_lookup(hashmap, str);
		l_free(str);
		assert(ptr == itr);
	}

	assert(l_hashmap_lookup(hashmap, "not in hash") == NULL);



	l_hashmap_destroy(hashmap, NULL);
}

int main(int argc, char *argv[])
{
	l_test_init(&argc, &argv);

	l_test_add("Pointer Test", test_ptr, NULL);
	l_test_add("String Test", test_str, NULL);
	return l_test_run();
}
