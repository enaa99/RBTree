#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>

rbtree* new_rbtree(void) {
	rbtree* p = (rbtree*)calloc(1, sizeof(rbtree));
	node_t* nil = (node_t*)calloc(1, sizeof(node_t));

	p->root = nil;
	p->nil = nil;
	nil->color = RBTREE_BLACK;
	// TODO: initialize struct if needed

	return p;
}
void in_order_delete(rbtree* t, node_t* node) {
	if (node != t->nil) {
		if (node->left != t->nil)
			in_order_delete(t, node->left);
		if (node->right != t->nil)
			in_order_delete(t, node->right);
		free(node);
	}
}


void delete_rbtree(rbtree* t) {
	// TODO: reclaim the tree nodes's memory
	node_t* node = t->root;
	in_order_delete(t, node);
	free(t->nil);
	free(t);
}

void left_rotate(rbtree* t, node_t* x) {
	node_t* y = x->right;
	x->right = y->left;
	if (y->left != t->nil) {
		y->left->parent = x;
	}
	y->parent = x->parent;
	if (x->parent == t->nil) {
		t->root = y;
	}
	else if (x == x->parent->left) x->parent->left = y;
	else x->parent->right = y;
	y->left = x;
	x->parent = y;
}
void right_rotate(rbtree* t, node_t* x) {
	node_t* y = x->left;
	x->left = y->right;
	if (y->right != t->nil) y->right->parent = x;
	y->parent = x->parent;
	if (x->parent == t->nil) t->root = y;
	else if (x == x->parent->right) x->parent->right = y;
	else x->parent->left = y;
	y->right = x;
	x->parent = y;
}


void rbtree_insert_fixup(rbtree* t, node_t* node) {

	while (node->parent->color == RBTREE_RED) {
		// 왼쪽 오른쪽 구분
		if (node->parent == node->parent->parent->left) {
			node_t* tmp = node->parent->parent->right;
			if (tmp->color == RBTREE_RED) {
				node->parent->color = RBTREE_BLACK;
				tmp->color = RBTREE_BLACK;
				node->parent->parent->color = RBTREE_RED;
				node = node->parent->parent; // 할아버지부터 다시 시작
			}
			else {
				if (node->parent->right == node) { // 꺾인 상태
					node = node->parent;
					left_rotate(t, node);
					// 좌로 회전-j
				}
				// 펴진 상태 
				node->parent->color = RBTREE_BLACK;
				node->parent->parent->color = RBTREE_RED;
				// 우로 회전
				right_rotate(t, node->parent->parent);
			}
		}
		else {
			node_t* tmp = node->parent->parent->left;
			if (tmp->color == RBTREE_RED) {
				node->parent->color = RBTREE_BLACK;
				tmp->color = RBTREE_BLACK;
				node->parent->parent->color = RBTREE_RED;
				node = node->parent->parent;
			}
			else {
				if (node->parent->left == node) {
					node = node->parent;
					right_rotate(t, node);

					//우로회전
				}
				node->parent->color = RBTREE_BLACK;
				node->parent->parent->color = RBTREE_RED;
				//좌로회전
				left_rotate(t, node->parent->parent);

			}
		}
	}
	t->root->color = RBTREE_BLACK;
}

node_t* rbtree_insert(rbtree* t, const key_t key) {
	// TODO: implement insert
	node_t* x = t->root;
	node_t* y = t->nil;
	node_t* newNode = (node_t*)calloc(1, sizeof(node_t));
	newNode->key = key;
	newNode->color = RBTREE_RED;
	while (x != t->nil) {
		y = x;
		if (newNode->key < x->key) x = x->left;
		else x = x->right;
	}
	newNode->parent = y;
	if (y == t->nil) {
		t->root = newNode;
	}
	else if (newNode->key < y->key) y->left = newNode;
	else y->right = newNode;

	newNode->left = t->nil;
	newNode->right = t->nil;
	rbtree_insert_fixup(t, newNode);

	return newNode;
}

node_t* rbtree_find(const rbtree* t, const key_t key) {
	// TODO: implement find
	node_t* tmp = t->root;
	while (tmp != t->nil) {
		if (tmp->key == key) return tmp;

		if (tmp->key <= key) tmp = tmp->right;
		else tmp = tmp->left;

	}
	return NULL;
}

node_t* rbtree_min(const rbtree* t) {
	// TODO: implement find
	node_t* p = t->root;
	while (p->left != t->nil) {
		p = p->left;
	}

	return p;
}

node_t* rbtree_max(const rbtree* t) {
	// TODO: implement find
	node_t* p = t->root;
	while (p->right != t->nil) {
		p = p->right;
	}

	return p;
}

void rbtree_transplant(rbtree* t, node_t *node, node_t* nodeChild) {
	if (node->parent == t->nil) t->root = nodeChild;
	else if (node == node->parent->left) node->parent->left = nodeChild;
	else node->parent->right = nodeChild;
	nodeChild->parent = node->parent;

}
node_t* rbtree_minimum(rbtree* t, node_t* node) {
	while (node->left != t->nil) {
		node = node->left;
	}
	return node;
}


void rbtree_delete_fixup(rbtree* t, node_t* x)
{
	while (x != t->root && x->color == RBTREE_BLACK)
	{
		if (x == x->parent->left)
		{
			// w is sibling
			node_t* w = x->parent->right;
			if (w->color == RBTREE_RED)
			{
				// case1, When the brother on the right side of the double black was red
				w->color = RBTREE_BLACK;
				x->parent->color = RBTREE_RED;
				left_rotate(t, x->parent);
				w = x->parent->right;
			}
			if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK)
			{
				// case2 When the brother of the double black is black
				// and both of the brothers’ children are black,
				w->color = RBTREE_RED;
				x = x->parent;
			}
			else
			{
				if (w->right->color == RBTREE_BLACK)
				{
					// case3
					//  When the brother on the right side of the double black is black
					// and the brother’s left child is RED
					//  and the brother’s right child is BLACK
					w->left->color = RBTREE_BLACK;
					w->color = RBTREE_RED;
					right_rotate(t, w);
					w = x->parent->right;
				}
				w->color = x->parent->color;
				x->parent->color = RBTREE_BLACK;
				w->right->color = RBTREE_BLACK;
				left_rotate(t, x->parent);
				x = t->root;
			}
		}
		else
		{
			node_t* w = x->parent->left;
			if (w->color == RBTREE_RED) {
				//case1
				w->color = RBTREE_BLACK;
				x->parent->color = RBTREE_RED;
				right_rotate(t, x->parent);
				w = x->parent->left;
			}
			if (w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK) {
				//case2
				w->color = RBTREE_RED;
				x = x->parent;
			}
			else {
				if (w->left->color == RBTREE_BLACK) {
					//case3
					w->right->color = RBTREE_BLACK;
					w->color = RBTREE_RED;
					left_rotate(t, w);
					w = x->parent->left;
				}
				//case4
				w->color = x->parent->color;
				x->parent->color = RBTREE_BLACK;
				w->left->color = RBTREE_BLACK;
				right_rotate(t, x->parent);
				x = t->root;
			}
		}
	}
	x->color = RBTREE_BLACK;
}
int rbtree_erase(rbtree* t, node_t* z)
{
	if(rbtree_find(t,z->key) == NULL) return 0;
	//   TODO: implement erase
	// RB tree 내부의 ptr로 지정된 node를 삭제하고 메모리 반환
	node_t* y = z;
	color_t y_original_color = y->color;
	node_t* x = NULL;
	// Check if the number of children is 0 or 1
	if (z->left == t->nil)
	{
		x = z->right;
		rbtree_transplant(t, z, z->right);
	}
	else if (z->right == t->nil)
	{
		x = z->left;
		rbtree_transplant(t, z, z->left);
	}
	else
	{
		y = rbtree_minimum(t, z->right);
		y_original_color = y->color;
		x = y->right;
		if (y->parent == z)
			x->parent = y;
		else
		{
			rbtree_transplant(t, y, y->right);
			y->right = z->right;
			y->right->parent = y;
		}
		rbtree_transplant(t, z, y);
		y->left = z->left;
		y->left->parent = y;
		y->color = z->color;
	}

	if (y_original_color == RBTREE_BLACK)
		rbtree_delete_fixup(t, x);

	free(z);

	return 1;
}


int rbtree_to_array(const rbtree* t, key_t* arr, const size_t n) {

	//RB tree의 내용을 key 순서대로 주어진 array로 변환
	//	array의 크기는 n으로 주어지며 tree의 크기가 n 보다 큰 경우에는 순서대로 n개 까지만 변환
	//	array의 메모리 공간은 이 함수를 부르는 쪽에서 준비하고 그 크기를 n으로 알려줍니다.
	node_t* p = t->root;
	if (p == t->nil) return 0;

	in_order(t, p, arr, arr, n);
	
	/*for (int i = 0; i < 10; i++)
		printf("%d\n", *(arr++));*/

	return 1;
}

int* in_order(const rbtree* t, node_t* node, int* arr, int* pa, const size_t n) {

	if (node != t->nil) {
		if (node->left != t->nil)
			arr = in_order(t, node->left, arr, pa, n);
		if (arr - pa >= n) return arr;
		*arr = node->key;
		arr++;
		if (node->right != t->nil)
			arr = in_order(t, node->right, arr, pa, n);
	}
	return arr;
}
