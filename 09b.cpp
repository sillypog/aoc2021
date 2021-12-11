/*
Next, you need to find the largest basins so you know what areas are most important to avoid.

A basin is all locations that eventually flow downward to a single low point. Therefore, every low point has a basin, although some basins are very small. Locations of height 9 do not count as being in any basin, and all other locations will always be part of exactly one basin.

The size of a basin is the number of locations within the basin, including the low point. The example above has four basins.

The top-left basin, size 3:

2199943210
3987894921
9856789892
8767896789
9899965678
The top-right basin, size 9:

2199943210
3987894921
9856789892
8767896789
9899965678
The middle basin, size 14:

2199943210
3987894921
9856789892
8767896789
9899965678
The bottom-right basin, size 9:

2199943210
3987894921
9856789892
8767896789
9899965678
Find the three largest basins and multiply their sizes together. In the above example, this is 9 * 14 * 9 = 1134.

What do you get if you multiply together the sizes of the three largest basins?
*/
#include <fstream>
#include <iostream>
#include <stack>
#include <string>
#include <vector>

using namespace std;

class Flat2DArray {
private:
	vector<int> values;
	int width;

public:
	Flat2DArray() {}
	Flat2DArray(int size, int defaultValue) : values(size, defaultValue) {}

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

	int& operator[](int i) {
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
	vector<int> neighborPositions {heightmap.up(pos), heightmap.right(pos), heightmap.down(pos), heightmap.left(pos)};
	for (int neighborPosition : neighborPositions) {
		if (neighborPosition > -1) {
			if (heightmap[neighborPosition] <= heightmap[pos]) {
				isMinima = false;
			}
		}
	}

	return isMinima;
}

vector<int> findMinima(const Flat2DArray& heightmap) {
	vector<int> output;
	for (int i = 0; i < heightmap.size(); i++) {
		if (isLocalMinima(i, heightmap)) {
			output.push_back(i);
		}
	}

	return output;
}

/*
Remember: two minima could be in the same basin
UPDATE: I got lucky and didn't have to deal with this :D

Create another grid to track which basin each position is assigned to.
Track if two basins are conjoined. (UPDATE: I didn't need to)

I think we can sum as we go.

Can use a stack to explore the basin, starting with each local minima and exploring out.
 */
void findBasins(const vector<int>& minima, const Flat2DArray& heightmap, Flat2DArray& basinmap) {
	stack<int> candidates; // Positions that might be in the basin
	int pos;
	int currentBasin = 0;
	vector<int> basinSizes;

	// Remember, some minima might be in the same basin. Let's see if we get lucky.
	for (int minimum : minima) {
		candidates.push(minimum);
		basinSizes.push_back(0);
		while (!candidates.empty()) {
			pos = candidates.top();
			candidates.pop();
			if (heightmap[pos] != 9 && basinmap[pos] < 0) {
				basinSizes[currentBasin]++;
				basinmap[pos] = currentBasin; // This is what we can use to check for overlaps
				// Now add neighbors to the stack
				vector<int> neighborPositions {heightmap.up(pos), heightmap.right(pos), heightmap.down(pos), heightmap.left(pos)};
				for (int neighborPosition : neighborPositions) {
					// Only follow this neightbor if it's in the grid and hasn't already been visited
					if (neighborPosition > -1 && basinmap[neighborPosition] < 0) {
						candidates.push(neighborPosition);
					}
				}
			}
		}
		currentBasin++;
	}

	// Get the three largest basins
	sort(basinSizes.begin(), basinSizes.end());
	int lastPos = basinSizes.size() - 1;
	int result = basinSizes[lastPos] * basinSizes[lastPos - 1] * basinSizes[lastPos - 2];
	cout << "Product of 3 largest basin sizes is " << result << endl;
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

	Flat2DArray basinmap(heightmap.size(), -1);
	basinmap.setWidth(inputLine.length());

	vector<int> minima = findMinima(heightmap);
	findBasins(minima, heightmap, basinmap);
}
