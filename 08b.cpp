#include <algorithm>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <sstream>
#include <string>
#include <set>
#include <vector>

using namespace std;

constexpr int ONE_SEGMENTS = 2;
constexpr int FOUR_SEGMENTS = 4;
constexpr int SEVEN_SEGMENTS = 3;
constexpr int EIGHT_SEGMENTS = 7;

constexpr int sortedDigitPosition[10] = {6, 0, 3, 3, 2, 3, 6, 1, 9 ,6};

class Display {
private:
	vector<string> patterns;
	vector<string> outputs;
	vector<char> segments; // Top bar is 0, then go round clockwise. Middle bar is 6.
	vector<set<int>> segmentsInDigits; // The reference to which segment draws a given digit
	unordered_map<string, int> signalsForDigits;

	void populateSegmentsInDigits() {
		vector<set<int>> temp {
			{0, 1, 2, 3, 4, 5},
			{1, 2},
			{0, 1, 6, 4, 3},
			{0, 1, 6, 2, 3},
			{5, 6, 1, 2},
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
	void eliminatePossibleSegments(int digit, set<char> digitSignals, vector<set<char>>& segmentPossibilities) {
		set<char> temp;
		for (int i = 0; i < EIGHT_SEGMENTS; i++) {
			if (segmentsInDigits[digit].count(i)) {
				set_intersection(segmentPossibilities[i].begin(), segmentPossibilities[i].end(), digitSignals.begin(), digitSignals.end(), std::inserter(temp, temp.begin()));
			} else {
				set_difference(segmentPossibilities[i].begin(), segmentPossibilities[i].end(), digitSignals.begin(), digitSignals.end(), std::inserter(temp, temp.begin()));
			}
			segmentPossibilities[i] = temp;
			temp.clear();
		}
	}

	void printSegmentPossibilities(const vector<set<char>>& segmentPossibilities) {
		// Verify that worked
		for (int i = 0; i < EIGHT_SEGMENTS; i++) {
			cout << segmentPossibilities[i].count('a') << segmentPossibilities[i].count('b') << segmentPossibilities[i].count('c') << segmentPossibilities[i].count('d') << segmentPossibilities[i].count('e') << segmentPossibilities[i].count('f') << segmentPossibilities[i].count('g') << endl;
		}
		cout << endl;
	}

	string setContents(const set<char>& s) {
		stringstream ss;
		for (const char c : s) {
			ss << c;
		}
		return ss.str();
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
		/* cout << "\nDecoding digit 1: " << patterns[sortedDigitPosition[1]] << endl; */
		set<char> digitSignals = extractSignalsFromPattern(patterns[sortedDigitPosition[1]]);
		eliminatePossibleSegments(1, digitSignals, segmentPossibilities);
		/* printSegmentPossibilities(segmentPossibilities); */

		// Now compare 7 to 1 to get signal for segment 0
		/* cout << "\nDecoding digit 7: " << patterns[sortedDigitPosition[7]] << endl; */
		digitSignals = extractSignalsFromPattern(patterns[sortedDigitPosition[7]]);
		eliminatePossibleSegments(7, digitSignals, segmentPossibilities);
		/* printSegmentPossibilities(segmentPossibilities); */

		// Now compare 4 to 1 to reduce possibilities for segments 5 and 6
		/* cout << "\nDecoding digit 4: " << patterns[sortedDigitPosition[4]] << endl; */
		digitSignals = extractSignalsFromPattern(patterns[sortedDigitPosition[4]]);
		eliminatePossibleSegments(4, digitSignals, segmentPossibilities);
		/* printSegmentPossibilities(segmentPossibilities); */

		// Now use 6 to distinguish segment 1 from segment 2
		// Identify digit six - it is the only one of length 6 that is missing segment 1
		int startOfSixSegmentNumbers = sortedDigitPosition[6]; // 6, 9, 0 all have six digits
		set<char> intersection;
		bool matched6 = false;
		bool matched9 = false;
		bool matched0 = false;
		for (int i = 0; i < 3; i++) {
			digitSignals = extractSignalsFromPattern(patterns[startOfSixSegmentNumbers + i]);
			// Only 6 doesn't have segment 1
			if (!matched6) {
				/* cout << "Checking if " << patterns[startOfSixSegmentNumbers + i] << " is signal for 6" << endl; */
				intersection.clear();
				set_intersection(digitSignals.begin(), digitSignals.end(), segmentPossibilities[1].begin(), segmentPossibilities[1].end(), std::inserter(intersection, intersection.begin()));
				if (intersection.size() == 1) {
					/* cout << "Digit six representation is " << patterns[startOfSixSegmentNumbers + i] << endl; */
					eliminatePossibleSegments(6, digitSignals, segmentPossibilities);
					matched6 = true;
					continue;
				}
			}
			// Only 9 doesn't have segment 4
			if (!matched9) {
				/* cout << "Checking if " << patterns[startOfSixSegmentNumbers + i] << " is signal for 9" << endl; */
				intersection.clear();
				set_intersection(digitSignals.begin(), digitSignals.end(), segmentPossibilities[4].begin(), segmentPossibilities[4].end(), std::inserter(intersection, intersection.begin()));
				/* cout << "digitSignals: " << setContents(digitSignals) << ", segment 4 possibilities: " << setContents(segmentPossibilities[4]) << ". Intersection: " << setContents(intersection) << endl; */
				if (intersection.size() == 1) {
					/* cout << "Digit nine representation is " << patterns[startOfSixSegmentNumbers + i] << endl; */
					eliminatePossibleSegments(9, digitSignals, segmentPossibilities);
					matched9 = true;
					continue;
				}
			}
			/* // Only 0 doesn't have segment 6 */
			if (!matched0) {
				/* cout << "Checking if " << patterns[startOfSixSegmentNumbers + i] << " is signal for 0" << endl; */
				intersection.clear();
				set_intersection(digitSignals.begin(), digitSignals.end(), segmentPossibilities[0].begin(), segmentPossibilities[0].end(), std::inserter(intersection, intersection.begin()));
				if (intersection.size() == 1) {
					/* cout << "Digit zero representation is " << patterns[startOfSixSegmentNumbers + i] << endl; */
					eliminatePossibleSegments(0, digitSignals, segmentPossibilities);
					matched0 = true;
					continue;
				}
			}
		}

		// Verify that worked
		/* printSegmentPossibilities(segmentPossibilities); */

		// Set segments based on final segmentPossibilities
		// I only know one way to get the value out of the set so I'll loop over the one value
		for (int i = 0; i < EIGHT_SEGMENTS; i++) {
			for (char c : segmentPossibilities[i]) {
				segments.push_back(c);
			}
			/* cout << "Segment " << i << " mapped to signal " << segments[i] << endl; */
		}
	}

	void mapSignalsToDigits() {
		int i = 0;
		stringstream ss;
		string signal;
		for (const set<int>& digitSegments : segmentsInDigits) {
			ss.clear();
			ss.str("");
			for (int segment : digitSegments) {
				// Look up the corresponding signal
				// Add that to the string
				ss << segments[segment];
			}
			signal = ss.str();
			sort(signal.begin(), signal.end());
			signalsForDigits.insert({signal, i}); // Populate the map
			i++;
		}
	}

	int	decodeSignal(string signal) const {
		sort(signal.begin(), signal.end());
		auto result = signalsForDigits.find(signal);
		if (result == signalsForDigits.end()) {
			cout << "Bad signal mapping!" << endl;
			exit(EXIT_FAILURE);
		}
		return result->second;
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
				outputs.push_back(s);
			} else {
				/* cout << "Adding " << s << " to patterns" << endl; */
				patterns.push_back(s);
			}
		}

		// Build up our reference for how each digit is drawn
		populateSegmentsInDigits(); // I shouldn't be doing this for every object.

		// We have all the patterns now, figure out which segments are which
		assignSegments();

		// Now that we know which segment is which, we can map all the patterns to digits
		mapSignalsToDigits();

		// Test that
		/* cout << "signalsForDigits contains:" << endl; */
		/* for (auto entry : signalsForDigits) { */
		/* 	cout << entry.first << ": " << entry.second << endl; */
		/* } */
	}

	int sumOutputs() const {
		int sum = 0;
		int digit;
		int unit = 1000;
		for (string output : outputs) {
			digit = decodeSignal(output);
			sum += (digit * unit);
			unit /= 10;
		}
		/* cout << sum << ", "; */
		return sum;
	}

};



int main() {
	fstream file("08.txt", ios::in);
	string line;
	int sum;

	vector<Display> displays;

	while (getline(file, line)) {
		displays.emplace_back(line);
	}

	for (const Display& display : displays) {
		sum += display.sumOutputs();
	}

	cout << "There are " << displays.size() << " displays" << endl;
	cout << "The sum of display outputs is " << sum << endl;
}
