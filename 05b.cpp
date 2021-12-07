/*
Unfortunately, considering only horizontal and vertical lines doesn't give you the full picture; you need to also consider diagonal lines.

Because of the limits of the hydrothermal vent mapping system, the lines in your list will only ever be horizontal, vertical, or a diagonal line at exactly 45 degrees. In other words:

An entry like 1,1 -> 3,3 covers points 1,1, 2,2, and 3,3.
An entry like 9,7 -> 7,9 covers points 9,7, 8,8, and 7,9.
Considering all lines from the above example would now produce the following diagram:

1.1....11.
.111...2..
..2.1.111.
...1.2.2..
.112313211
...1.2....
..1...1...
.1.....1..
1.......1.
222111....
You still need to determine the number of points where at least two lines overlap. In the above example, this is still anywhere in the diagram with a 2 or larger - now a total of 12 points.

Consider all of the lines. At how many points do at least two lines overlap?
 */
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>

using namespace std;

struct Point {
	int x;
	int y;
};

class Line {
private:
	Point orderAscending(int a, int b) {
		return a < b ? Point {a, b} : Point {b, a};
	}

public:
	Point start;
	Point end;

	vector<Point> getPoints() {
		vector<Point> points;

		if (start.x == end.x) { // Vertical line
			Point limits = orderAscending(start.y, end.y);

			for (int i = limits.x; i <= limits.y; i++) {
				points.push_back({start.x, i});
			}
		} else if (start.y == end.y) { // Horizontal line
			Point limits = orderAscending(start.x, end.x);

			for (int i = limits.x; i <= limits.y; i++) {
				points.push_back({i, start.y});
			}
		} else if ((start.y < end.y && start.x < end.x) || (end.y < start.y && end.x < start.x)) {
			/* cout << start.x << "," << start.y << " to " << end.x << "," << end.y << " is a tl<->br diagonal." << endl; */
			// Always work top to bottom
			Point topLeft;
			Point bottomRight;
			if (start.x < end.x) {
				topLeft = start;
				bottomRight = end;
			} else {
				topLeft = end;
				bottomRight = start;
			}
			// Now do step down and right through all the points
			for (int i = 0; i <= bottomRight.x - topLeft.x; i++) {
				/* cout << "Point on diagonal: " << topLeft.x + i << "," << topLeft.y + i << endl; */
				points.push_back({topLeft.x + i, topLeft.y + i});
			}
		} else {
			/* cout << start.x << "," << start.y << " to " << end.x << "," << end.y << " is a bl<->tr diagonal." << endl; */
			// Always work top to bottom
			Point bottomLeft;
			Point topRight;
			if (start.x < end.x) {
				bottomLeft = start;
				topRight = end;
			} else {
				bottomLeft = end;
				topRight = start;
			}
			// Now do step down and left through all the points
			for (int i = 0; i <= topRight.x - bottomLeft.x; i++) {
				/* cout << "Point on diagonal: " << topRight.x - i << "," << topRight.y + i << endl; */
				points.push_back({topRight.x - i, topRight.y + i});
			}
		}

		return points;
	}
};

Line parseLine(string input) {
	char ignored;
	int startX;
	int startY;
	int endX;
	int endY;

	stringstream ss(input);

	ss >> startX;
	ss >> ignored;
	ss >> startY;
	ss >> ignored;
	ss >> ignored;
	ss >> endX;
	ss >> ignored;
	ss >> endY;

	return {{startX, startY}, {endX, endY}};
}

void markLine(Line line, unordered_map<string, int>& map) {
	stringstream keystream;
	string key;

	vector<Point> points = line.getPoints();
	for (const Point& point : points) {
		keystream.str("");
		keystream << point.x << "," << point.y;
		key = keystream.str();
		/* cout << "  Checking point " << key << endl; */

		auto it = map.find(key);
		if (it != map.end()) {
			/* cout << "    " << key << " was in the map already" << endl; */
			it->second++;
		} else {
			map.insert({key, 0});
		}
	}
}

int main() {
	fstream file("05.txt", ios::in);
	unordered_map<string, int> seenPoints;

	string inputLine;
	while (getline(file, inputLine)) {
		Line line = parseLine(inputLine);
		/* cout << "Doing line " << line.start.x << "," << line.start.y << " to " << line.end.x << "," << line.end.y << endl; */
		markLine(line, seenPoints);
	}

	int multipleOverlaps = 0;
	for (const auto& pair : seenPoints) {
		/* cout << pair.first << ": " << pair.second << endl; */
		if (pair.second > 0) {
			multipleOverlaps++;
		}
	}

	cout << "There were " << multipleOverlaps << " points where two or more lines overlapped." << endl;
}
