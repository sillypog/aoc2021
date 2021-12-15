#include <fstream>
#include <iostream>
#include <stack>
#include <vector>

#include "flat2Darray.h"

using namespace std;

struct Octopus {
	int energyLevel;
	bool didFlash;
};

void increaseEnergy(int position, Flat2DArray<Octopus>& octopuses, stack<int>& flashers) {
		if (position < 0) {
			return;
		}

		octopuses[position].energyLevel++;
		if (octopuses[position].energyLevel > 9 && octopuses[position].didFlash == false) {
			octopuses[position].didFlash = true;
			flashers.push(position);
		}
}

int doStep(Flat2DArray<Octopus>& octopuses) {
	stack<int> flashers;
	vector<int> flashed;

	for (int i = 0; i < octopuses.size(); i++) {
		increaseEnergy(i, octopuses, flashers);
	}

	// Now check all the neighbors and add them to the stack if needed
	while (!flashers.empty()) {
		int flasher = flashers.top();
		flashers.pop();
		flashed.push_back(flasher);

		int neighbor;
		// Top
		neighbor = octopuses.up(flasher);
		increaseEnergy(neighbor, octopuses, flashers);

		// Top right
		neighbor = octopuses.up(flasher);
		neighbor = octopuses.right(neighbor);
		increaseEnergy(neighbor, octopuses, flashers);

		// Right
		neighbor = octopuses.right(flasher);
		increaseEnergy(neighbor, octopuses, flashers);

		// Bottom right
		neighbor = octopuses.right(flasher);
		neighbor = octopuses.down(neighbor);
		increaseEnergy(neighbor, octopuses, flashers);

		// Bottom
		neighbor = octopuses.down(flasher);
		increaseEnergy(neighbor, octopuses, flashers);

		// Bottom left
		neighbor = octopuses.down(flasher);
		neighbor = octopuses.left(neighbor);
		increaseEnergy(neighbor, octopuses, flashers);

		// Left
		neighbor = octopuses.left(flasher);
		increaseEnergy(neighbor, octopuses, flashers);

		// Top left
		neighbor = octopuses.left(flasher);
		neighbor = octopuses.up(neighbor);
		increaseEnergy(neighbor, octopuses, flashers);
	}

	// Now set all those that flashed to 0
	for (int flasher : flashed) {
		octopuses[flasher].energyLevel = 0;
		octopuses[flasher].didFlash = false;
	}

	return flashed.size();
}

// Not sure why my overloaded << operator doesn't work
void print2DArray(const Flat2DArray<Octopus>& octopuses, int width) {
	for (int i = 0; i < octopuses.size(); i++) {
		if (i % width== 0) {
			cout << endl;
		}
		cout << octopuses[i].energyLevel;
	}
	cout << endl;
}

int main() {
	Flat2DArray<Octopus> octopuses;
	int flashCount = 0;

	fstream file("11small.txt", ios::in);

	char inputBuffer;
	while (file >> inputBuffer) {
		octopuses.insert({inputBuffer - '0', false});
	}

	// Rewind the stream and read the top line to get the width of the map
	string inputLine;
	file.clear();
	file.seekg(0, ios::beg);
	getline(file, inputLine);
	octopuses.setWidth(inputLine.length());

	cout << "There are " << octopuses.size() << " octopuses" << endl;

	int step = 0;
	while (true) {
		step++;
		flashCount = doStep(octopuses);
		if (flashCount == octopuses.size()) {
			break;
		}
	}
	print2DArray(octopuses, inputLine.length());

	cout << "All octopuses flashed simultaneously in step " << step << "." << endl;
}
