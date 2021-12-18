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

This part does all of the folds then prints out the dots to reveal a message of 8
uppercase characters.
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
		} else {
			dots.push_back(parseDot(inputBuffer));
		}
	}

	// Sort the pairs and keep the uniques
	sort(dots.begin(), dots.end());
	auto it = unique(dots.begin(), dots.end());
	dots.resize(std::distance(dots.begin(), it));

	int width = dots[dots.size() - 1].first + 1; // The last dot has largest x value. Add one because zero-based.

	// To print, sort by y coordinate first
	sort(dots.begin(), dots.end(), [](pair<int,int> a, pair<int,int> b){
    return a.second == b.second? a.first < b.first : a.second < b.second;
  });

	// Then print line by line using . or # as appropriate
	int x = 0, dotIterator = 0;
	while (dotIterator < dots.size()) {
		if (x == width) {
			x = 0;
			cout << endl;
		}
		if (dots[dotIterator].first == x) {
			cout << "#";
			dotIterator++;
		} else {
			cout << ".";
		}
		x++;
	}
}
