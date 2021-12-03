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
int main() {
	fstream file("03.txt", ios::in);

	// Read the first number so we know how many bits it has.
	string input;
	file >> input;

	vector<int> onCount(input.length(), 0); // Initialize the vector to all zeros
	int numCount = 0;

	do {
		numCount++;

		int i = 0;
		for (const char& c : input) {
			int bit = static_cast<int>(c - '0');
			onCount[i++] += bit;
		}
	} while (file >> input);

	int gamma = 0;
	int epsilon = 0;
	int bitValue = 0;
	int bitPos = 0;
	for (int i = 0; i < onCount.size(); i++) {
		if (onCount[i] == numCount/2) {
			cout << "EDGE CASE!" << endl;
			exit(EXIT_FAILURE);
		}

		bitValue = (onCount[i] > numCount/2) ? 1 : 0;
		bitPos = onCount.size() - 1 - i;
		gamma = gamma | (bitValue << bitPos);
		epsilon = epsilon | (!bitValue << bitPos);
	}

	cout << gamma << ", " << epsilon << ". Power consumption: " << (gamma * epsilon) << endl;
}
