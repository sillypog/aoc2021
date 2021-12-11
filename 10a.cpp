#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <unordered_set>

using namespace std;

unordered_set<char> openings {'{', '<', '(', '['};
unordered_set<char> closings {'}', '>', ')', ']'};
unordered_map<char, int> scoring {{')',3}, {']',57}, {'}',1197}, {'>',25137}};

// I need to modify this so that within each line it process CHUNKS.
// I don't think I need to change much, just make it so that each time we open a chunk,
// we recurse and process that bit.
int processLine(string& input) {
	cout << "Processing: " << input << endl;
	unordered_map<char, int> counts {{'{',0}, {'<',0}, {'(',0}, {'[',0}};

	char c;
	stringstream ss(input);

	while(ss >> c) {
		if (openings.count(c)) {
			// Increment out count
			auto count =  counts.find(c);
			count->second++;
			cout << "   " << c << " " << count->second << endl;
		} else {
			// Decrement the count. If it went below 0, it's corrupted and we should break and return score
			char pairChar = c == ')' ? c - 1 : c - 2;
			auto count = counts.find(pairChar);
			count->second--;
			cout << "   " << c << ". " << pairChar << " " << count->second << endl;
			if (count->second < 0) {
				cout << "Corrupt" << endl;
				// Return the score for this character
				auto score = scoring.find(c);
				return score->second;
			}
		}
	}

	return 0; // No corrupt elements found
}

int main() {
	fstream file("10small.txt", ios::in);
	string inputBuffer;
	int score = 0;

	while (getline(file, inputBuffer)) {
		score += processLine(inputBuffer);
	}

	cout << "Syntax score is " << score << endl;
}
