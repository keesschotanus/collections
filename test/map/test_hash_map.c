/*
 * test_hash_map.c
 *
 * Unit tests for the hash map implementation defined in hash_map.c.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "hash_map.h"

void test_hash_map(void)
{
        hash_map hmap = hash_map_create(10, sizeof(int), sizeof(int));
        assert(hmap != NULL);
        hash_map_put(hmap, &(int){1}, &(int){100});
        hash_map_put(hmap, &(int){2}, &(int){200});
        
        assert(*(int *)hash_map_get(hmap, &(int){1}) == 100);
        assert(*(int *)hash_map_get(hmap, &(int){2}) == 200);

       	printf("All hash_map tests passed!\n");
}