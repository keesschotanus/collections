#include <stdio.h>
#include <stdlib.h>

void test_list(void);
void test_doubly_linked_list(void);
void test_hash_map(void);
void test_binary_tree(void);
void test_binary_search_tree(void);

int main(void)
{
	test_list();
	test_doubly_linked_list();
	
	test_hash_map();

	test_binary_tree();
	test_binary_search_tree();
	
	return 0;
}

