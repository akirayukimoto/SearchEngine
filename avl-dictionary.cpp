
// Implementation of a dictionary using an AVL tree
// AVL Trees are balanced binary trees 

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "avl-dictionary.h"

bool debug = false;

// Constructor
AVLDictionary::AVLDictionary()
{
	root = NULL;
	nElements = 0;
}

// Add a record to the dictionary. Returns false if key already exists
bool
AVLDictionary::addRecord( KeyType key, DataType record)
{
	if ( debug) {
		printf("------------------------------------\n");
		printf("addRecord(\"%s\",%ld)\n",  key, (long) record);
		printf("---------- Before -----------------\n");
		printNode("", root, 0);
	}
	
	// Add your implementation here
	//Find node to insert into 
	//Node does not exist. Create it.
	//Height might not be valid anymore.
	//We need to restructure .
	//
	
	// find if the node exist
	
	AVLNode *n = new AVLNode();
	n->key = key;
	n->data = record;
	n->left = NULL;
	n->right = NULL;
	n->parent = NULL;
	n->height = 1;

	if (root == NULL) {
		root = n;
		nElements++;
		return true;
	}

	AVLNode *curr = root;
	AVLNode *prev = NULL;
	while (curr != NULL) {
		prev = curr;
		if (!strcmp(key, curr->key)) {
			curr->data = record;
			return false;
		}
		else if (strcmp(key, curr->key) < 0) curr = curr->left;
		else curr = curr->right;
	}
	
	if (strcmp(key, prev->key) < 0) prev->left = n;
	else prev->right = n;

	n->parent = prev;

	AVLNode *m = n->parent;
	while (m != NULL) {
		int maxheight = 0;
		if (m->left != NULL) maxheight = m->left->height;
		if (m->right != NULL && m->right->height > maxheight)
			maxheight = m->right->height;
		m->height = maxheight + 1;
		m = m->parent;
	}

	if ( debug) {
		printf("---------- Before Restructure -----------------\n");
		printNode("", root, 0);
	}
	
	// Call restructure
	restructure(n);

	if (debug) {
		checkRecursive(root);
		
		printf("---------- After Restructure -----------------\n");
		printNode("", root, 0);
	}
	nElements++;
		
	return true;
}

void
AVLDictionary::restructure(AVLNode * n) {
	//Balance the tree starting at n 

	//Go upwards until root is found

        // See class notes

	// Add your implementation here
	AVLNode *z = n->parent;
	while (z != NULL) {
		int hleft = 0;
		if (z->left != NULL) {
			hleft = z->left->height;
		}
		int hright = 0;
		if (z->right != NULL) hright = z->right->height;
		int hdiff = hleft - hright;
		if (hdiff < 0) hdiff = -hdiff;
		if (hleft > hright) z->height = hleft + 1;
		else z->height = hright + 1;
		if (hdiff <= 1) {
			z = z->parent;
			continue;
		}

		AVLNode *y = NULL;
		int maxh = 0;
		if (z->left != NULL) {
			y = z->left;
			maxh = y->height;
		}
		if (z->right != NULL && z->right->height > maxh) {
			y = z->right;
			maxh = y->height;
		}
		assert(y != NULL);

		AVLNode *x = NULL;
		maxh = 0;
		if (y->left != NULL) {
			x = y->left;
			maxh = y->left->height;
		}
		if (y->right != NULL && y->right->height > maxh) {
			x = y->right;
			maxh = x->height;
		}
		assert(x != NULL);

		AVLNode *a, *b, *c, *t0, *t1, *t2, *t3;
		if (z->right == y) {
			if (y->right == x) {
				a = z;
				b = y;
				c = x;
				t0 = z->left;
				t1 = y->left;
				t2 = x->left;
				t3 = x->right;
			}
			else {
				a = z;
				b = x;
				c = y;
				t0 = z->left;
				t1 = x->left;
				t2 = x->right;
				t3 = y->right;
			}
		}
		else {
			if (y->left == x) {
				a = x;
				b = y;
				c = z;
				t0 = x->left;
				t1 = x->right;
				t2 = y->right;
				t3 = z->right;
			}
			else {
				a = y;
				b = x;
				c = z;
				t0 = y->left;
				t1 = x->left;
				t2 = x->right;
				t3 = z->right;
			}
		}

		AVLNode *p = z->parent;
		if (p != NULL) {
			if (p->left == z) p->left = b;
			else p->right = b;
		}
		else root = b;

		b->parent = p;
		b->left = a;
		b->right = c;

		a->parent = b;
		a->left = t0;
		a->right = t1;

		c->parent = b;
		c->left = t2;
		c->right = t3;

		if (t0 != NULL) t0->parent = a;
		if (t1 != NULL) t1->parent = a;
		if (t2 != NULL) t2->parent = c;
		if (t3 != NULL) t3->parent = c;

		maxh = 0;
		if (a->left != NULL) maxh = a->left->height;
		if (a->right != NULL && a->right->height > maxh)
			maxh = a->right->height;
		a->height = maxh + 1;

		maxh = 0;
		if (c->left != NULL) maxh = c->left->height;
		if (c->right != NULL && c->right->height > maxh)
			maxh = c->right->height;
		c->height = maxh + 1;

		maxh = 0;
		if (b->left != NULL) maxh = b->left->height;
		if (b->right != NULL && b->right->height > maxh)
			maxh = b->right->height;
		b->height = maxh + 1;
		
		z = p;
	}
}

// Find a key in the dictionary and return corresponding record or NULL
DataType
AVLDictionary::findRecord( KeyType key)
{
        // Add your implementation here
	AVLNode *curr = root;
	while (curr != NULL) {
		if (!strcmp(curr->key, key)) return (DataType)curr->data;
		else if (strcmp(curr->key, key) > 0) curr = curr->left;
		else curr = curr->right;
	}
	return NULL;
}

// Removes one element from the dictionary
bool
AVLDictionary::removeElement(KeyType key)
{
	if (debug) {
		printf("------------------------------------\n");
		printf("removeElement(\"%s\")\n",  key);
		printf("---------- Before -----------------\n");
		printNode("", root, 0);
	}


	// Add your implementation here

	AVLNode *curr = root;
	while (curr != NULL) {
		if (!strcmp(curr->key, key)) break;
		else if (strcmp(curr->key, key) > 0) curr = curr->left;
		else curr = curr->right;
	}
	if (curr == NULL) return false;

	//AVLNode *parent = curr->parent;
	AVLNode *parent = NULL;
	if (curr != root) parent = curr->parent;
	else {
	//	parent = curr->left;
	//	curr->right->parent = parent;
	//	curr->left->right = curr->right;
	//	parent = curr->left;
	//	curr->left = NULL;
	//	curr->right = NULL;
		curr->left = curr->right->parent;
	}

	if (curr->left == NULL && curr->right == NULL) {
		if (curr == parent->right) parent->right = NULL;
		else parent->left = NULL;
		delete curr;
		curr = NULL;
		restructure(parent);
	}
	else if (curr->left == NULL) {
		if (curr == parent->right) {
			parent->right = NULL;
			parent->right = curr->right;
			parent->right->height = curr->right->height;
			//delete curr;
			//curr = NULL;
			restructure(parent);
			delete curr;
			//curr = NULL;
		}
		else {
			parent->left = NULL;
			parent->left = curr->left;
			parent->left->height = curr->left->height;
			//delete curr;
			//curr = NULL;
			restructure(parent);
			delete curr;
			curr = NULL;
		}
	}
	else if (curr->right == NULL) {
		if (curr == parent->right) {
			parent->right = NULL;
			parent->right = curr->right;
			parent->right->height = curr->right->height;
			//delete curr;
			//curr = NULL;
			restructure(parent);
			delete curr;
			curr = NULL;
		}
		else {
			parent->left = NULL;
			parent->left = curr->left;
			parent->left->height = curr->left->height;
			//delete curr;
			//curr = NULL;
			restructure(parent);
			delete curr;
			curr = NULL;
		}
	}
	else {
		AVLNode *prev = curr->left;
		while (prev->right != NULL) {
			prev = prev->right;
		}
		curr->key = prev->key;
		curr->data = prev->data;

		curr = prev;

		if (curr->left == NULL) {
			if (curr->parent->left == curr) {
				curr->parent->right = curr->right;
			}
			else {
				curr->parent->left = curr->right;
			}
		}
		else {
			if (curr->parent->right == curr)
				curr->parent->right = curr->left;
			else curr->parent->left = curr->left;
		}
		restructure(curr->parent);
		delete curr;
	}
	
	
	if (debug) {
		printf("---------- After -----------------\n");
		printNode("", root, 0);

		checkRecursive(root);
	}
	
	return true;
}

// Returns all the elements in the table as an array of strings.
// *n is the size of the table and it is returned by reference
KeyType *
AVLDictionary::keys(int * n)
{
	KeyType * a = (KeyType *) malloc(nElements * sizeof(KeyType));
	*n = 0;
	addKeysRecursive(root, a, n);

	return a;
}

// Add keys recursively
void
AVLDictionary::addKeysRecursive(AVLNode * node, KeyType * a, int * n) {
	if (node==NULL) {
		return;
	}
	
	a[*n] = node->key;
	(*n)++;
	addKeysRecursive(node->left, a, n);
	addKeysRecursive(node->right, a, n);
}

