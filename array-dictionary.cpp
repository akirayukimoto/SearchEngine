
// Implementation of a dictionary using an array and sequential search
// The array will be dynamically resized if necessary

#include <stdlib.h>
#include <string.h>

#include "array-dictionary.h"

// Constructor
ArrayDictionary::ArrayDictionary()
{
  // Add your code here
	maxNumber = 500;
	currentNumber = 0;
	array = new ArrayDictionaryNode[maxNumber];
}

// Add a record to the dictionary. Returns false if key already exists
bool
ArrayDictionary::addRecord( KeyType key, DataType record)
{
        // Add your code here
	// find if the element exists
	for (int i = 0; i < currentNumber; i++) {
		if (!strcmp(array[i].key, key)) {
			array[i].data = (DataType *)record;
			return false;
		}
	}
	// increase the size when the array is full
	if (currentNumber == maxNumber) {
		//int oldSize = maxNumber;
		maxNumber = maxNumber * 2;
		ArrayDictionaryNode *newArray = new ArrayDictionaryNode[maxNumber];
		for (int i = 0; i < currentNumber; i++) {
			newArray[i].key = array[i].key;
			newArray[i].data = new DataType;
			newArray[i].data = (DataType *)array[i].data;
		}
		delete []array;
		array = newArray;
	}
	array[currentNumber].key = strdup(key);
	array[currentNumber].data = new DataType;
	array[currentNumber].data = (DataType *)record;
	currentNumber++;
	return true;
}

// Find a key in the dictionary and return corresponding record or NULL
DataType
ArrayDictionary::findRecord( KeyType key)
{
        // add your code here
	
	return NULL;
}

// Removes one element from the table
bool
ArrayDictionary::removeElement(KeyType key)
{
        // Add your code here

	return true;
	
}

// Returns all the elements in the table as an array of strings.
// *n is the size of the table and it is returned by reference
KeyType *
ArrayDictionary::keys(int * n)
{
        // Add yoru code here
	return NULL;
}
