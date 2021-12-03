/*
The submarine has been making some odd creaking noises, so you ask it to produce a diagnostic report just in case.

The diagnostic report (your puzzle input) consists of a list of binary numbers which, when decoded properly, can tell you many useful things about the conditions of the submarine. The first parameter to check is the power consumption.

You need to use the binary numbers in the diagnostic report to generate two new binary numbers (called the gamma rate and the epsilon rate). The power consumption can then be found by multiplying the gamma rate by the epsilon rate.

Each bit in the gamma rate can be determined by finding the most common bit in the corresponding position of all numbers in the diagnostic report. For example, given the following diagnostic report:

00100
11110
10110
10111
10101
01111
00111
11100
10000
11001
00010
01010
Considering only the first bit of each number, there are five 0 bits and seven 1 bits. Since the most common bit is 1, the first bit of the gamma rate is 1.

The most common second bit of the numbers in the diagnostic report is 0, so the second bit of the gamma rate is 0.

The most common value of the third, fourth, and fifth bits are 1, 1, and 0, respectively, and so the final three bits of the gamma rate are 110.

So, the gamma rate is the binary number 10110, or 22 in decimal.

The epsilon rate is calculated in a similar way; rather than use the most common bit, the least common bit from each position is used. So, the epsilon rate is 01001, or 9 in decimal. Multiplying the gamma rate (22) by the epsilon rate (9) produces the power consumption, 198.

Use the binary numbers in your diagnostic report to calculate the gamma rate and epsilon rate, then multiply them together. What is the power consumption of the submarine? (Be sure to represent your answer in decimal, not binary.)
 */
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
