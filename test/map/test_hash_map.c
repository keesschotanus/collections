/*
 * test_hash_map.c
 *
 * Unit tests for the hash map implementation defined in hash_map.c.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "map/hash_map.h"

struct person {
    int id;
    char name[50];
    char email[100];
};

static void test_hash_map_int_to_int(void);
static void test_hash_map_int_to_struct(void);

void test_hash_map(void)
{
       	printf("Test hash map...");

        test_hash_map_int_to_int();
        test_hash_map_int_to_struct();

	puts(" ✅");
}

static void test_hash_map_int_to_int(void) {
        hash_map_t hmap = hash_map_create(1, sizeof(int), sizeof(int));
        assert(hmap != NULL);

        for (int i = 0; i < 100; i++) {
                assert(hash_map_put(hmap, &i, &(int){i * 100}));
        }

        assert(hash_map_size(hmap) == 100);
        for (int i = 0; i < 100; i++) {
                int const *value = (int *)hash_map_get(hmap, &i);
                assert(*value == i * 100);
        }       

        hash_map_remove(hmap, &(int){1});
        assert(hash_map_get(hmap, &(int){1}) == NULL);
        assert(hash_map_size(hmap) == 99);


        hash_map_free(hmap);
}

static void test_hash_map_int_to_struct(void)
{
        hash_map_t hmap = hash_map_create(10, sizeof(int), sizeof(struct person));
        assert(hmap != NULL);

        struct person p1 = {1, "Alice", "alice@example.com"};
        struct person p2 = {2, "Bob", "bob@example.com"};

        hash_map_put(hmap, &(int){1}, &p1);
        hash_map_put(hmap, &(int){2}, &p2);

        assert(strcmp(((struct person *)hash_map_get(hmap, &(int){1}))->name, "Alice") == 0);
        assert(strcmp(((struct person *)hash_map_get(hmap, &(int){2}))->name, "Bob") == 0);

        hash_map_remove(hmap, &(int){1});
        assert(hash_map_get(hmap, &(int){1}) == NULL);

        hash_map_remove(hmap, &(int){2});
        assert(hash_map_get(hmap, &(int){2}) == NULL);

        hash_map_free(hmap);
}