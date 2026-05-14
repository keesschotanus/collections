#include <stdio.h>
#include <stdlib.h>

void test_list(void);
void test_doubly_linked_list(void);
void test_hash_map(void);

int main(void)
{
	test_list();
	test_doubly_linked_list();
	
	test_hash_map();
	
	return 0;
}

