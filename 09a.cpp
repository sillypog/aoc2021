/*
--- Day 9: Smoke Basin ---

These caves seem to be lava tubes. Parts are even still volcanically active; small hydrothermal vents release smoke into the caves that slowly settles like rain.

If you can model how the smoke flows through the caves, you might be able to avoid it and be that much safer. The submarine generates a heightmap of the floor of the nearby caves for you (your puzzle input).

Smoke flows to the lowest point of the area it's in. For example, consider the following heightmap:

2199943210
3987894921
9856789892
8767896789
9899965678
Each number corresponds to the height of a particular location, where 9 is the highest and 0 is the lowest a location can be.

Your first goal is to find the low points - the locations that are lower than any of its adjacent locations. Most locations have four adjacent locations (up, down, left, and right); locations on the edge or corner of the map have three or two adjacent locations, respectively. (Diagonal locations do not count as adjacent.)

In the above example, there are four low points, all highlighted: two are in the first row (a 1 and a 0), one is in the third row (a 5), and one is in the bottom row (also a 5). All other locations on the heightmap have some lower adjacent location, and so are not low points.

The risk level of a low point is 1 plus its height. In the above example, the risk levels of the low points are 2, 1, 6, and 6. The sum of the risk levels of all low points in the heightmap is therefore 15.

Find all of the low points on your heightmap. What is the sum of the risk levels of all low points on your heightmap?
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
