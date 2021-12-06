#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_set>

using namespace std;

struct Point {
	int x;
	int y;
};

struct Line {
	Point start;
	Point end;
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

// Do I want to store the map as a 2D array or a map keyed on marked squares?
int markLine(Line line, unordered_set<string>& map) {
	int overlaps = 0;
	if (line.start.x == line.end.x || line.start.y == line.end.y) { // Might need one check for horizontal, one for vert
		// Loop for the length of the line. NOTE: line could be decreasing OR increasing.
		// Build string from coordinates
		// Check against the map
		//   If it's not there, add it
		//   If it is there, incremement the return value
	}
	return overlaps;
}

int main() {
	fstream file("05small.txt", ios::in);
	unordered_set<string> seenPoints;
	int multipleOverlaps = 0;

	string inputLine;
	while (getline(file, inputLine)) {
		Line line = parseLine(inputLine);
		cout << line.start.x << "," << line.start.y << " to " << line.end.x << "," << line.end.y << endl;
		int overlaps = markLine(line, seenPoints);
		multipleOverlaps += overlaps;
	}
}
