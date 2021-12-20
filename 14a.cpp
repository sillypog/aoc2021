/*
Building a polymer by inserting new elements between pairs of existing elements.

The output has the initial polymer, then a list of instructions of which element
to insert between each pair of elements in the polymer, eg
	NNCB

	NN -> C
	NC -> B

So in that example, after one step we'd have NCNBCB.

Here we have to run 10 steps of this process. Once done, return the difference in
number of occurrences of the most and least frequent elements in the polymer.

I used a linked list because I thought it would be an efficient way to insert data
anywhere in a list. That worked ok for part 1...
 */
#include <forward_list>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <sstream>
#include <unordered_map>
#include <utility>

using namespace std;

void createPolymer(string input, forward_list<char>& polymer) {
	polymer = {input.begin(), input.end()};
}

void processPair(string input, unordered_map<string, char>& pairs) {
	string pair;
	string separator;
	char atom;

	stringstream ss(input);
	ss >> pair >> separator >> atom;

	pairs.insert({pair, atom});
}

void doStep(forward_list<char>& polymer, unordered_map<string, char>& pairs) {
	auto currentAtom = polymer.begin();
	auto nextAtom = std::next(currentAtom, 1);
	string pair;
	while (nextAtom != polymer.end()) {
		pair = {*currentAtom, *nextAtom};

		/* cout << "Adding " << pairs.at(pair) << " between " << pair << endl; */
		polymer.insert_after(currentAtom, pairs.at(pair));

		std::advance(currentAtom, 2);
		nextAtom = std::next(currentAtom, 1);
	}
}

unordered_map<char, int> getElementOccurrence(forward_list<char> polymer) {
	unordered_map<char, int> occurrences;

	for (char element : polymer) {
		if (occurrences.count(element)) {
			occurrences.at(element)++;
		} else {
			occurrences.insert({element, 1});
		}
	}

	return occurrences;
}

int main() {
	forward_list<char> polymer;
	unordered_map<string, char> pairs;

	fstream file("14.txt", ios::in);

	string inputBuffer;
	bool processPairs;
	while(getline(file, inputBuffer)) {
		if (inputBuffer.length() == 0) {
			processPairs = true;
			continue;
		}
		if (processPairs) {
			processPair(inputBuffer, pairs);
		} else {
			createPolymer(inputBuffer, polymer);
		}
	}

	for (int i = 0; i < 10; i++) {
		doStep(polymer, pairs);
	}

	unordered_map<char, int> elementOccurrences = getElementOccurrence(polymer);
	pair<char, int> least {'!', INT_MAX};
	pair<char, int> most {'!', 0};
	for (pair<char, int> elementOccurrence : elementOccurrences) {
		if (elementOccurrence.second > most.second) {
			most = elementOccurrence;
		}
		if (elementOccurrence.second < least.second) {
			least = elementOccurrence;
		}
	}

	cout << "Least common element is " << least.first << ", occurring " << least.second << " times." << endl;
	cout << "Most common element is " << most.first << ", occurring " << most.second << " times." << endl;
	cout << "Difference in occurrences is " << most.second - least.second << "." << endl;
}
