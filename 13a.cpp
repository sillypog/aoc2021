/*
There are a bunch of dots on some transparent paper. You repeatedly fold the paper
to overlay the dots and reveal a message.

Data files have a top part with x,y coordinates for the dots and a lower part with
folding instructions. The parts are separated by an empty line. eg
	10,4
	4,11
	6,0

	fold along y=7
	fold along x=5

This first part prints out the number of dots after the first fold. Overlapping dots
are counted as one dot.
 */
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <utility>

using namespace std;

pair <int, int> parseDot(string s) {
	stringstream ss(s);
	int x;
	int y;
	char ignore;
	ss >> x >> ignore >> y;
	return {x,y};
}

pair <char, int> parseFold(string s) {
	stringstream ss(s);
	string ignoreString;
	char axis, ignoreChar;
	int line;
	ss >> ignoreString >> ignoreString >> axis >> ignoreChar >> line;
	return {axis, line};
}

void doFold(const pair<char, int>& fold, vector<pair<int, int>>& dots) {
	for (pair<int, int>& dot : dots) {
		if (fold.first == 'x') {
			if (dot.first > fold.second) {
				dot.first = fold.second - (dot.first - fold.second);
			}
		} else {
			if (dot.second > fold.second) {
				// Want to be on the other side of the line. If line is 7:
				// 8 becomes 6. ie, 7 - (8 - 7)
				// 9 becomes 5. ie, 7 - (9 - 7)
				// 10 becomes 4
				// 11 becomes 3
				// 12 becomes 2
				// 13 becomes 1
				// 14 becomes 0. ie, 7 - (14-7)
				dot.second = fold.second - (dot.second - fold.second);
			}
		}
	}
}

int main() {
	vector<pair<int, int>> dots;
	pair <char, int> fold;
	bool processFolds = false;

	fstream file("13.txt", ios::in);
	string inputBuffer;

	while (getline(file, inputBuffer)) {
		if (inputBuffer.length() == 0) {
			processFolds = true;
			continue;
		} else if (processFolds) {
			fold = parseFold(inputBuffer);
			cout << "Fold along " << fold.first << " axis on line " << fold.second << endl;
			doFold(fold, dots);
			break; // We only want to do the first fold for now
		} else {
			dots.push_back(parseDot(inputBuffer));
		}
	}

	// Sort the pairs and count the uniques
	sort(dots.begin(), dots.end());
	auto it = unique(dots.begin(), dots.end());
	dots.resize(std::distance(dots.begin(), it));

	cout << "There are " << dots.size() << " dots" << endl;

}
