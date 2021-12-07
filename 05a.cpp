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
		cout << "  Checking point " << key << endl;

		auto it = map.find(key);
		if (it != map.end()) {
			cout << "    " << key << " was in the map already" << endl;
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
		cout << "Doing line " << line.start.x << "," << line.start.y << " to " << line.end.x << "," << line.end.y << endl;
		markLine(line, seenPoints);
	}

	int multipleOverlaps = 0;
	for (const auto& pair : seenPoints) {
		cout << pair.first << ": " << pair.second << endl;
		if (pair.second > 0) {
			multipleOverlaps++;
		}
	}

	cout << "There were " << multipleOverlaps << " points where two or more lines overlapped." << endl;
}
