#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <set>
#include <vector>

using namespace std;

constexpr int ONE_SEGMENTS = 2;
constexpr int FOUR_SEGMENTS = 4;
constexpr int SEVEN_SEGMENTS = 3;
constexpr int EIGHT_SEGMENTS = 7;

constexpr int sortedDigitPosition[10] = {8, 0, 8, 8, 2, 8, 8, 1, 8 ,8};

class Display {
private:
	vector<string> patterns;
	vector<string> output;
	vector<char> segments; // Top bar is 0, then go round clockwise. Middle bar is 6.
	vector<set<int>> segmentsInDigits; // The reference to which segment draws a given digit

	void populateSegmentsInDigits() {
		vector<set<int>> temp {
			{0, 1, 2, 3, 4, 5},
			{1, 2},
			{0, 1, 6, 4, 3},
			{0, 1, 6, 2, 3},
			{5, 4, 1, 2},
			{0, 5, 6, 2, 3},
			{6, 2, 3, 4, 5, 0},
			{0, 1, 2},
			{0, 1, 2, 3, 4, 5, 6},
			{0, 1, 2, 3, 5, 6}
		};

		segmentsInDigits = temp;
	}

	set<char> extractSignalsFromPattern(string pattern) {
		set<char> output;
		for (char c : pattern) {
			output.insert(c);
		}
		return output;
	}

	// For each of the segments in a one, set its possibilities to the intersection of these sets
	// For each of the segments in everything else, remove this set from its possiblities
	void eliminatePossibleSegments(int digit, vector<set<char>>& segmentPossibilities) {
		cout << "Digit " << digit << " signal is " << patterns[sortedDigitPosition[digit]] << endl;
		set<char> digitSignals = extractSignalsFromPattern(patterns[sortedDigitPosition[digit]]);
		set<char> temp;
		for (int i = 0; i < EIGHT_SEGMENTS; i++) {
			if (segmentsInDigits[digit].count(i)) {
				cout << "Setting possibilities for segment " << i << " to " << digitSignals.size() << endl;
				set_intersection(segmentPossibilities[i].begin(), segmentPossibilities[i].end(), digitSignals.begin(), digitSignals.end(), std::inserter(temp, temp.begin()));
			} else {
				cout << "Removing " << digitSignals.size() << " possiblities from segment " << i << endl;
				set_difference(segmentPossibilities[i].begin(), segmentPossibilities[i].end(), digitSignals.begin(), digitSignals.end(), std::inserter(temp, temp.begin()));
			}
			segmentPossibilities[i] = temp;
			temp.clear();
		}
	}

	void assignSegments() {
		// Store the possibilities for each segment as we go
		vector<set<char>> segmentPossibilities(EIGHT_SEGMENTS, {'a', 'b', 'c', 'd', 'e', 'f', 'g'});

		// Sort the patterns by length so that we can identify 1, 7, etc
		sort(patterns.begin(), patterns.end(), [](string s1, string s2) {
				return s1.length() < s2.length();
		});

		// Pattern for 1 is in position 0, pattern for 7 is in position 1.
		// The additional character in position 1 is the signal for the top segment
		cout << "\nDecoding digit 1" << endl;
		set<char> oneSignals = extractSignalsFromPattern(patterns[0]);
		eliminatePossibleSegments(1, segmentPossibilities);
		// Verify that worked
		for (int i = 0; i < EIGHT_SEGMENTS; i++) {
			cout << segmentPossibilities[i].count('a') << segmentPossibilities[i].count('b') << segmentPossibilities[i].count('c') << segmentPossibilities[i].count('d') << segmentPossibilities[i].count('e') << segmentPossibilities[i].count('f') << segmentPossibilities[i].count('g') << endl;
		}
		cout << endl;

		// Now compare 7 to 1 to get signal for segment 0
		cout << "\nDecoding digit 7" << endl;
		eliminatePossibleSegments(7, segmentPossibilities);
		// Verify that worked
		for (int i = 0; i < EIGHT_SEGMENTS; i++) {
			cout << segmentPossibilities[i].count('a') << segmentPossibilities[i].count('b') << segmentPossibilities[i].count('c') << segmentPossibilities[i].count('d') << segmentPossibilities[i].count('e') << segmentPossibilities[i].count('f') << segmentPossibilities[i].count('g') << endl;
		}
		cout << endl;

		// Now compare 4 to 1 to reduce possibilities for segments 5 and 6
		cout << "\nDecoding digit 4" << endl;
		eliminatePossibleSegments(4, segmentPossibilities);
		// Verify that worked
		for (int i = 0; i < EIGHT_SEGMENTS; i++) {
			cout << segmentPossibilities[i].count('a') << segmentPossibilities[i].count('b') << segmentPossibilities[i].count('c') << segmentPossibilities[i].count('d') << segmentPossibilities[i].count('e') << segmentPossibilities[i].count('f') << segmentPossibilities[i].count('g') << endl;
		}
		cout << endl;

		// Now use 6 to distinguish segment 1 from segment 2

		// Set segments based on final segmentPossibilities
	}

public:
	Display(const string& line) {
		stringstream ss(line);
		string s;
		bool reachedDelimiter = false;

		while (ss >> s) {
			if (s == "|") {
				reachedDelimiter = true;
				continue;
			}

			if (reachedDelimiter) {
				/* cout << "Adding " << s << " to output" << endl; */
				output.push_back(s);
			} else {
				/* cout << "Adding " << s << " to patterns" << endl; */
				patterns.push_back(s);
			}
		}

		// Build up our reference for how each digit is drawn
		populateSegmentsInDigits(); // I shouldn't be doing this for every object.

		// We have all the patterns now, figure out which segments are which
		assignSegments();
	}

};



int main() {
	fstream file("08one.txt", ios::in);
	string line;

	vector<Display> displays;

	while (getline(file, line)) {
		displays.emplace_back(line);
	}

	cout << "There are " << displays.size() << " displays" << endl;
}
