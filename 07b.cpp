/*
The crabs don't seem interested in your proposed solution. Perhaps you misunderstand crab engineering?

As it turns out, crab submarine engines don't burn fuel at a constant rate. Instead, each change of 1 step in horizontal position costs 1 more unit of fuel than the last: the first step costs 1, the second step costs 2, the third step costs 3, and so on.

As each crab moves, moving further becomes more expensive. This changes the best horizontal position to align them all on; in the example above, this becomes 5:

Move from 16 to 5: 66 fuel
Move from 1 to 5: 10 fuel
Move from 2 to 5: 6 fuel
Move from 0 to 5: 15 fuel
Move from 4 to 5: 1 fuel
Move from 2 to 5: 6 fuel
Move from 7 to 5: 3 fuel
Move from 1 to 5: 10 fuel
Move from 2 to 5: 6 fuel
Move from 14 to 5: 45 fuel
This costs a total of 168 fuel. This is the new cheapest possible outcome; the old alignment position (2) now costs 206 fuel instead.

Determine the horizontal position that the crabs can align to using the least fuel possible so they can make you an escape route! How much fuel must they spend to align to that position?
 */
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

void printVec(const vector<int>& values) {
	for (int i : values) {
		cout << i << " ";
	}
	cout << endl;
}

int calculateMeanValue(vector<int>& values) {
	double sum = 0;
	for (int i : values) {
		sum += i;
	}
	double mean = sum / values.size();
	/* cout << "The mean value is " << mean << endl; */
	return round(mean); // Maybe I should floor. Then I wouldn't have to subtract 1 later.
}

int calculateTriangleNumber(int n) {
	return n * (n+1) / 2;
}

int calculateFuel(vector<int>& values, int target) {
	int fuel = 0;
	int steps;
	for (int pos : values) {
		steps = abs(pos - target);
		fuel += calculateTriangleNumber(steps);
	}
	return fuel;
}

int main() {
	fstream file("07.txt", ios::in);
	vector<int> horizontalPositions;

	int pos;
	char comma;

	file >> pos;

	do {
		horizontalPositions.push_back(pos);
	} while (file >> comma >> pos);
	sort(horizontalPositions.begin(), horizontalPositions.end());
	printVec(horizontalPositions);

	int mean = calculateMeanValue(horizontalPositions);

	cout << "Setting horizontal positions to " << mean << endl;

	// Not sure why, but subtracting one from the mean gave the correct answer!
	cout << "This will cost " << calculateFuel(horizontalPositions, mean - 1) << " fuel." << endl;
}
