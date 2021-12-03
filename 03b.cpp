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

	Node* setLeft(int v) {
		if (left == nullptr) {
			left = new Node(v);
		}
		left->count++;
		return left;
	}

	Node* setRight(int v) {
		if (right == nullptr) {
			right = new Node(v);
		}
		right->count++;
		return right;
	}
};

int calculateRating(Node* currentNode, int numBits, bool useMostCommonBit) {
	int rating = 0;
	int bitPos = 0;

	for (int i = 0; i < numBits; i++) {
		bitPos = numBits - 1 - i;
		// Calculate rating by walking the tree
		// If the node only has one child, just follow that
		if (currentNode->left == nullptr) {
			currentNode = currentNode->right;
		} else if (currentNode->right == nullptr) {
			currentNode = currentNode->left;
		} else if (useMostCommonBit) {
			// Both children are present, choose the most common
			currentNode = currentNode->right->count >= currentNode->left->count ? currentNode->right : currentNode->left;
		} else {
			// Both children are present, choose the least common
			currentNode = currentNode->right->count < currentNode->left->count ? currentNode->right : currentNode->left;
		}
		rating = rating | (currentNode->value << bitPos);
	}

	return rating;
}

int main() {
	fstream file("03.txt", ios::in);

	// Read the first number so we know how many bits it has.
	string input;
	file >> input;

	Node* tree = new Node(-1);
	int numBits = input.length();

	do {
		Node* currentNode = tree;

		int i = 0;
		for (const char& c : input) {
			int bit = static_cast<int>(c - '0');

			// Advance down the tree and add nodes as needed
			currentNode = bit == 0 ? currentNode->setLeft(bit) : currentNode->setRight(bit);
		}
	} while (file >> input);

	int o2Rating = calculateRating(tree, numBits, true);
	int co2Rating = calculateRating(tree, numBits, false);

	cout << o2Rating << ", " << co2Rating << ". Power consumption: " << (o2Rating * co2Rating) << endl;
}
