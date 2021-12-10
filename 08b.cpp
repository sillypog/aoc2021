/*
Through a little deduction, you should now be able to determine the remaining digits. Consider again the first example above:

acedgfb cdfbe gcdfa fbcad dab cefabd cdfgeb eafb cagedb ab |
cdfeb fcadb cdfeb cdbaf
After some careful analysis, the mapping between signal wires and segments only make sense in the following configuration:

 dddd
e    a
e    a
 ffff
g    b
g    b
 cccc
So, the unique signal patterns would correspond to the following digits:

acedgfb: 8
cdfbe: 5
gcdfa: 2
fbcad: 3
dab: 7
cefabd: 9
cdfgeb: 6
eafb: 4
cagedb: 0
ab: 1
Then, the four digits of the output value can be decoded:

cdfeb: 5
fcadb: 3
cdfeb: 5
cdbaf: 3
Therefore, the output value for this entry is 5353.

Following this same process for each entry in the second, larger example above, the output value of each entry can be determined:

fdgacbe cefdb cefbgd gcbe: 8394
fcgedb cgb dgebacf gc: 9781
cg cg fdcagb cbg: 1197
efabcd cedba gadfec cb: 9361
gecf egdcabf bgf bfgea: 4873
gebdcfa ecba ca fadegcb: 8418
cefg dcbef fcge gbcadfe: 4548
ed bcgafe cdgba cbgef: 1625
gbdfcae bgc cg cgb: 8717
fgae cfgab fg bagce: 4315
Adding all of the output values in this larger example produces 61229.

For each entry, determine all of the wire/segment connections and decode the four-digit output values. What do you get if you add up all of the output values?
 */
#include <algorithm>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <sstream>
#include <string>
#include <set>
#include <vector>

using namespace std;

constexpr int NUM_SEGMENTS = 7;

constexpr int sortedDigitPosition[10] = {6, 0, 3, 3, 2, 3, 6, 1, 9 ,6};

vector<set<int>> segmentsInDigits; // The reference to which segment draws a given digit

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

class Display {
private:
	vector<string> patterns; // These are the set of strings representing all 10 digits, read from the file.
	vector<string> outputs; // These are the set of strings representing the value of this display, read from the file.
	vector<char> segments; // Top bar is 0, then go round clockwise. Middle bar is 6.
	unordered_map<string, int> signalsForDigits;

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
		for (int i = 0; i < NUM_SEGMENTS; i++) {
			if (segmentsInDigits[digit].count(i)) {
				set_intersection(segmentPossibilities[i].begin(), segmentPossibilities[i].end(), digitSignals.begin(), digitSignals.end(), std::inserter(temp, temp.begin()));
			} else {
				set_difference(segmentPossibilities[i].begin(), segmentPossibilities[i].end(), digitSignals.begin(), digitSignals.end(), std::inserter(temp, temp.begin()));
			}
			segmentPossibilities[i] = temp;
			temp.clear();
		}
	}

	bool identifySixSegmentDigit(const set<char>& digitSignals, const set<char>& segmentPossibilities) {
		set<char> intersection;
		set_intersection(digitSignals.begin(), digitSignals.end(), segmentPossibilities.begin(), segmentPossibilities.end(), std::inserter(intersection, intersection.begin()));
		return intersection.size() == 1;
	}

	void assignSegments() {
		// Store the possibilities for each segment as we go
		vector<set<char>> segmentPossibilities(NUM_SEGMENTS, {'a', 'b', 'c', 'd', 'e', 'f', 'g'});

		// Sort the patterns by length so that we can identify 1, 7, etc
		sort(patterns.begin(), patterns.end(), [](string s1, string s2) {
				return s1.length() < s2.length();
		});

		// Pattern for 1 is in position 0, pattern for 7 is in position 1.
		// The additional character in position 1 is the signal for the top segment
		set<char> digitSignals = extractSignalsFromPattern(patterns[sortedDigitPosition[1]]);
		eliminatePossibleSegments(1, digitSignals, segmentPossibilities);

		// Now compare 7 to 1 to get signal for segment 0
		digitSignals = extractSignalsFromPattern(patterns[sortedDigitPosition[7]]);
		eliminatePossibleSegments(7, digitSignals, segmentPossibilities);

		// Now compare 4 to 1 to reduce possibilities for segments 5 and 6
		digitSignals = extractSignalsFromPattern(patterns[sortedDigitPosition[4]]);
		eliminatePossibleSegments(4, digitSignals, segmentPossibilities);

		// Now use 6 to distinguish segment 1 from segment 2
		// Identify digit six - it is the only one of length 6 that is missing segment 1
		int startOfSixSegmentNumbers = sortedDigitPosition[6]; // 6, 9, 0 all have six digits
		bool matched6 = false;
		bool matched9 = false;
		bool matched0 = false;
		for (int i = 0; i < 3; i++) {
			digitSignals = extractSignalsFromPattern(patterns[startOfSixSegmentNumbers + i]);
			// Only 6 doesn't have segment 1
			if (!matched6) {
				if (identifySixSegmentDigit(digitSignals, segmentPossibilities[1])) {
					eliminatePossibleSegments(6, digitSignals, segmentPossibilities);
					matched6 = true;
					continue;
				}
			}
			// Only 9 doesn't have segment 4
			if (!matched9) {
				if (identifySixSegmentDigit(digitSignals, segmentPossibilities[4])) {
					eliminatePossibleSegments(9, digitSignals, segmentPossibilities);
					matched9 = true;
					continue;
				}
			}
			/* // Only 0 doesn't have segment 6 */
			if (!matched0) {
				if (identifySixSegmentDigit(digitSignals, segmentPossibilities[0])) {
					eliminatePossibleSegments(0, digitSignals, segmentPossibilities);
					matched0 = true;
					continue;
				}
			}
		}

		// Set segments based on final segmentPossibilities
		// I only know one way to get the value out of the set so I'll loop over the one value
		for (int i = 0; i < NUM_SEGMENTS; i++) {
			for (char c : segmentPossibilities[i]) {
				segments.push_back(c);
			}
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
				outputs.push_back(s);
			} else {
				patterns.push_back(s);
			}
		}

		// We have all the patterns now, figure out which segments are which
		assignSegments();

		// Now that we know which segment is which, we can map all the patterns to digits
		mapSignalsToDigits();
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
		return sum;
	}
};



int main() {
	fstream file("08.txt", ios::in);
	string line;
	int sum;
	vector<Display> displays;

	// Build up our reference for how each digit is drawn
	populateSegmentsInDigits();

	while (getline(file, line)) {
		displays.emplace_back(line);
	}

	for (const Display& display : displays) {
		sum += display.sumOutputs();
	}

	cout << "The sum of display outputs is " << sum << endl;
}
