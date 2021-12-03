#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

/*
The input could contain digits of any number of bits, can't assume we know the number of bits is each number.
They do seem to all be the same within a file though.

Keep track of:
* How many numbers are in the file
* How many of each bit were ON

Since bits can only be ON or OFF, if there are enough ON bits for more than half the numbers, that's the most common.
 */
class Node {
public:
	Node* left = nullptr;
	Node* right = nullptr;
	int count = 0;
	int value = 0;

	Node() {}
	Node(int v) : value(v) {}
};

int main() {
	fstream file("03.txt", ios::in);

	// Read the first number so we know how many bits it has.
	string input;
	file >> input;

	Node* tree = new Node(-1);
	int numBits = input.length();

	do {
		Node* currentNode = tree;
		Node* nextNode = nullptr;

		int i = 0;
		for (const char& c : input) {
			int bit = static_cast<int>(c - '0');

			// Advance down the tree and add nodes as needed
			nextNode = bit == 0 ? currentNode->left : currentNode->right;
			if (nextNode == nullptr) {
				nextNode = new Node(bit);
				if (bit == 0) {
					currentNode->left = nextNode;
				} else {
					currentNode->right = nextNode;
				}
			}
			currentNode = nextNode;
			currentNode->count++;
		}
	} while (file >> input);

	int o2Rating = 0;
	int co2Rating = 0;
	int bitPos = 0;
	Node* currentNode = tree;
	for (int i = 0; i < numBits; i++) {
		bitPos = numBits - 1 - i;
		// Calculate o2Rating by walking the tree
		// If the node only has one child, just follow that
		if (currentNode->left == nullptr) {
			currentNode = currentNode->right;
		} else if (currentNode->right == nullptr) {
			currentNode = currentNode->left;
		} else{
			// Both children are present, choose the most common
			currentNode = currentNode->right->count >= currentNode->left->count ? currentNode->right : currentNode->left;
		}
		o2Rating = o2Rating | (currentNode->value << bitPos);
	}

	currentNode = tree;
	bitPos = 0;
	for (int i = 0; i < numBits; i++) {
		bitPos = numBits - 1 - i;
		// Calculate co2Rating by walking the tree
		// If the node only has one child, just follow that
		if (currentNode->left == nullptr) {
			currentNode = currentNode->right;
		} else if (currentNode->right == nullptr) {
			currentNode = currentNode->left;
		} else{
			// Both children are present, choose the most common
			currentNode = currentNode->right->count < currentNode->left->count ? currentNode->right : currentNode->left;
		}
		co2Rating = co2Rating | (currentNode->value << bitPos);
	}


	cout << o2Rating << ", " << co2Rating << ". Power consumption: " << (o2Rating * co2Rating) << endl;

	// Test the tree
	/* Node* node = tree; */
	/* char dir; */
	/* while(true) { */
	/* 	cout << "Direction: "; */
	/* 	cin >> dir; */
	/* 	if (dir == 'x') { */
	/* 		node = tree; */
	/* 		cout << endl; */
	/* 	} else { */
	/* 		node = dir == '0' ? node->left : node->right; */
	/* 		cout << node->value << " " << node->count << endl; */
	/* 	} */
	/* } */
}
