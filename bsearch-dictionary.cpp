
// Implementation of a dictionary using an array and binary search
// It will inherit from the ArrayDictionary

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "bsearch-dictionary.h"

// Constructor
BinarySearchDictionary::BinarySearchDictionary()
{
	sorted = false;
}

bool
BinarySearchDictionary::addRecord( KeyType key, DataType record) {
	sorted = false;
	return ArrayDictionary::addRecord(key, record);
}

// Find a key in the dictionary and return corresponding record or NULL
DataType
BinarySearchDictionary::findRecord( KeyType key)
{
	if (!sorted) {
		sort();
		sorted = true;
	}
	
	// Use binary search

	// Add your implementation here
	
	int low = 0;
	int high = currentNumber - 1;
	while (high >= low) {
		int mid = (low + high) / 2;
		int result = strcmp(array[mid].key, key);
		if (result == 0) {
			// the element is found
			return (DataType)array[mid].data;
		}
		else if (result > 0) high = mid - 1;
		else low = mid + 1;
	}
	return NULL;
}


// Sort array using heap sort
void
BinarySearchDictionary::sort()
{
	//printf("Before\n");
	//for (int i = 0; i < currentNumber; i++) {
	//	printf("%s\n", array[i].key);
	//}

        // Add your code here

	//printf("After\n");
	//for (int i = 0; i < currentNumber; i++) {
	//	printf("%s\n", array[i].key);
	//}
	//
	ArrayDictionaryNode temp;
	
	for (int i = currentNumber / 2 - 1 ; i >= 0; i--) {
		shift(currentNumber, i);
	}

	for (int i = currentNumber - 1; i >= 0; i--) {
		temp = array[0];
		array[0] = array[i];
		array[i] = temp;
		shift(i, 0);
	}

}

void
BinarySearchDictionary::shift(int n, int i) {
	int parent = i;
	int left = 2 * i + 1;
	int right = 2 * i + 2;
	
	if (left < n && strcmp(array[left].key, array[parent].key) > 0) {
		parent = left;
	}
	
	if (right < n && strcmp(array[right].key, array[parent].key) > 0) {
		parent = right;
	}
	
	if (parent != i) {
		ArrayDictionaryNode temp = array[i];
		array[i] = array[parent];
		array[parent] = temp;

		shift(n, parent);
	}
}




