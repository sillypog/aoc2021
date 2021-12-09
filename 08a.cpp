#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

constexpr int ONE_SEGMENTS = 2;
constexpr int FOUR_SEGMENTS = 4;
constexpr int SEVEN_SEGMENTS = 3;
constexpr int EIGHT_SEGMENTS = 7;

struct Display {
	vector<string> patterns;
	vector<string> output;
};

Display parseLine(const string& line) {
	stringstream ss(line);
	string s;
	Display output;
	bool reachedDelimiter = false;

	/* cout << "Parsing next line" << endl; */
	while (ss >> s) {
		if (s == "|") {
			reachedDelimiter = true;
			continue;
		}

		if (reachedDelimiter) {
			/* cout << "Adding " << s << " to output" << endl; */
			output.output.push_back(s);
		} else {
			/* cout << "Adding " << s << " to patterns" << endl; */
			output.patterns.push_back(s);
		}
	}

	return output;
}

int countUniqueDigits(Display& display) {
	int count = 0;
	for (const string& s : display.output) {
		if (s.length() == ONE_SEGMENTS || s.length() == FOUR_SEGMENTS || s.length() == SEVEN_SEGMENTS || s.length() == EIGHT_SEGMENTS) {
			count++;
		}
	}
	return count;
}

int main() {
	fstream file("08.txt", ios::in);
	string line;
	int uniqueDigits = 0;

	while (getline(file, line)) {
		Display display = parseLine(line);
		uniqueDigits += countUniqueDigits(display);
	}

	cout << "There were " << uniqueDigits << " digits identifiable by their unique segment count." << endl;
}
