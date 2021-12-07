#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

constexpr int NUM_DAYS = 80;

void printHistogram(const vector<int>& histogram) {
	for (int i : histogram) {
		cout << i << " ";
	}
	cout << endl;
}

int totalHistogram(const vector<int>& histogram) {
	int sum = 0;
	for (int i : histogram) {
		sum += i;
	}
	return sum;
}

int main() {
	fstream file("06.txt", ios::in);
	vector<int> histogram(9, 0);

	int stage;
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
		int zeroDays = histogram[0]; // At the end we'll inc 6 by this and set 8 to this
		for (int i = 0; i < 8; i++) {
			histogram[i] = histogram[i+1];
		}
		histogram[6] += zeroDays;
		histogram[8] = zeroDays;

		/* printHistogram(histogram); */
	}

	cout << "After " << NUM_DAYS << " there will be " << totalHistogram(histogram) << " latern fish." << endl;
}
