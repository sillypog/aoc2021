/*
Remember: two minima could be in the same basin

Create another grid to track which basin each position is assigned to.
Track if two basins are conjoined.

I think we can sum as we go.

Can use a stack to explore the basin, starting with each local minima and exploring out.
 */
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Flat2DArray {
private:
	vector<int> values;
	int width;

public:
	void setWidth(int w) {
		width = w;
	}

	void insert(int v) {
		values.push_back(v);
	}

	int size() const {
		return values.size();
	}

	const int& operator[](int i) const {
		return values[i];
	}

	int left(int i) const {
		// Check that this won't take us past the left edge
		return (i % width == 0) ? -1 : i - 1;
	}

	int right(int i) const{
		// Check that this won't take us past the right edge
		return (i % width == (width - 1)) ? -1 : i + 1;
	}

	int up(int i) const{
		// Check that this won't take us past the top edge
		return (i < width - 1) ? -1 : i - width;
	}

	int down (int i) const{
		// Check that this won't take us past the bottom edge
		return (i > values.size() - width) ? -1 : i + width;
	}
};

bool isLocalMinima(int pos, const Flat2DArray& heightmap) {
	bool isMinima = true;
	int neighborPos;
	// Check against left neighbor
	neighborPos = heightmap.left(pos);
	if (neighborPos > -1) {
		if (heightmap[neighborPos] <= heightmap[pos]) {
			isMinima = false;
		}
	}
	// Check against right neighbor
	neighborPos = heightmap.right(pos);
	if (neighborPos > -1) {
		if (heightmap[neighborPos] <= heightmap[pos]) {
			isMinima = false;
		}
	}
	// Check against top neighbor
	neighborPos = heightmap.up(pos);
	if (neighborPos > -1) {
		if (heightmap[neighborPos] <= heightmap[pos]) {
			isMinima = false;
		}
	}
	// Check against bottom neighbor
	neighborPos = heightmap.down(pos);
	if (neighborPos > -1) {
		if (heightmap[neighborPos] <= heightmap[pos]) {
			isMinima = false;
		}
	}

	return isMinima;
}

int sumRisk(const Flat2DArray& heightmap) {
	int sum = 0;

	for (int i = 0; i < heightmap.size(); i++) {
		if (isLocalMinima(i, heightmap)) {
			sum += heightmap[i] + 1;
		}
	}

	return sum;
}

int main() {
	Flat2DArray heightmap;

	fstream file("08.txt", ios::in);

	char inputBuffer;
	while (file >> inputBuffer) {
		heightmap.insert(inputBuffer - '0');
	}

	// Rewind the stream and read the top line to get the width of the map
	string inputLine;
	file.clear();
	file.seekg(0, ios::beg);
	getline(file, inputLine);
	heightmap.setWidth(inputLine.length());

	int risk = sumRisk(heightmap);
	cout << "The sum of the risk levels for all the low points is " << risk<< endl;
}
