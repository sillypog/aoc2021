/*
Suppose the lanternfish live forever and have unlimited food and space. Would they take over the entire ocean?

After 256 days in the example above, there would be a total of 26984457539 lanternfish!

How many lanternfish would there be after 256 days?
 */
#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

constexpr int NUM_DAYS = 256;

void printHistogram(const vector<uintmax_t>& histogram) {
	for (const uintmax_t& i : histogram) {
		cout << i << " ";
	}
	cout << endl;
}

uintmax_t totalHistogram(const vector<uintmax_t>& histogram) {
	uintmax_t sum = 0;
	for (const uintmax_t& i : histogram) {
		sum += i;
	}
	return sum;
}

int main() {
	fstream file("06.txt", ios::in);
	vector<uintmax_t> histogram(9, 0);

	uintmax_t stage;
	char comma;

	file >> stage;
	do {
		/* cout << stage << endl; */
		histogram[stage]++;
	} while (file >> comma >> stage);

	/* printHistogram(histogram); */

	// Now run simulations for the next few days
	for (int day = 0; day < NUM_DAYS; day++) {
		// For all the zeros, create a new 8
		uintmax_t zeroDays = histogram[0]; // At the end we'll inc 6 by this and set 8 to this
		for (int i = 0; i < 8; i++) {
			histogram[i] = histogram[i+1];
		}
		histogram[6] += zeroDays;
		histogram[8] = zeroDays;

		/* printHistogram(histogram); */
	}

	cout << "After " << NUM_DAYS << " there will be " << totalHistogram(histogram) << " latern fish." << endl;
}
