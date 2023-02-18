#include <stdio.h>
#include <stdlib.h>
#include "rbtree.h"


int main(void) {

	rbtree* t = new_rbtree();
	rbtree_insert(t,10);
	rbtree_insert(t, 20);
	rbtree_insert(t, 30);
	rbtree_insert(t, 5);
	rbtree_insert(t, 22);
	rbtree_insert(t, 50);
	rbtree_insert(t, 80);
	rbtree_insert(t, 99);
	int arr[100] = { 0, };
	rbtree_to_array(t, arr, 5);


	return 0;
}