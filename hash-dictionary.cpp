
// Implementation of a dictionary using a hash table

#include <stdlib.h>
#include <string.h>
#include "hash-dictionary.h"

// Constructor
HashDictionary::HashDictionary()
{
  // Add your code here
	//nElements = 0;
	for (int i = 0; i < MaxBuckets; i++) {
		buckets[i] = NULL;
	}
}

int
HashDictionary::hash(KeyType key) {
  // Add your code here
  int sum = 0;
  int len = strlen(key);
  for (int i = 0; i < len; i++) {
		sum += i*key[i];
  }
  return sum%MaxBuckets;
}

// Add a record to the dictionary. Returns false if key already exists
bool
HashDictionary::addRecord( KeyType key, DataType record)
{
  // Add your code here
	int h = hash(key);
	HashNode *entry = buckets[h];
	while (entry != NULL) {
		if (!strcmp(key, entry->key)) {
			entry->data = record;
			return false;
		}
		entry = entry->next;
	}
	entry = new HashNode();
	entry->key = strdup(key);
	entry->data = record;
	entry->next = buckets[h];
	buckets[h] = entry;
	nElements++;
	return true;
}

// Find a key in the dictionary and return corresponding record or NULL
DataType
HashDictionary::findRecord( KeyType key)
{
	int h = hash(key);
	HashNode *curr = buckets[h];
	while (curr != NULL) {
		if (!strcmp(key, curr->key)) {
			return (DataType)curr->data;
		}
		curr = curr->next;
	}
	return NULL;
}

// Removes one element from the table
bool
HashDictionary::removeElement(KeyType key)
{
  // Add your code here
	int h = hash(key);
	HashNode *curr = buckets[h];
	HashNode *prev = NULL;
	while (curr != NULL) {
		if (!strcmp(key, curr->key)) {
			if (prev != NULL) prev = curr->next;
			else buckets[h] = curr->next;
			nElements--;
			delete curr;
			return true;
		}
		prev = curr;
		curr = curr->next;
	}
	return false;
}

// Returns all the elements in the table as an array of strings.
// *n is the size of the table and it is returned by reference
KeyType *
HashDictionary::keys(int * n)
{
	KeyType * a = (KeyType *) malloc(nElements * sizeof(KeyType));

	*n = nElements;
	int i = 0;
	for (int h = 0; h < MaxBuckets; h++) {
		HashNode * n = buckets[h];
		while (n!=NULL) {
			a[i] = n->key;
			i++;
			n = n->next;
		}
	}
	*n = nElements;
	return a;
}

