/*
This runs too slowly if we try to maintain the actual polymer.
I think that since the polymer approximately doubles with each iteration,
to run 40 iterations is 2^40 elements which is a lot to iterate over.
Much like the lantern fish problem, maybe we can do this just by counting the pairs themselves.
*/
#include <cmath>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <string>
#include <sstream>
#include <unordered_map>
#include <utility>

using namespace std;

void createPolymer(string input, unordered_map<string, uintmax_t>& polymer) {
	// Read the pairs from the input and add a count of one for each to the polymer.
	string key;
	for (int i = 0; i < input.length() - 1; i++) {
		key = {input[i], input[i+1]};
		if (polymer.count(key)) {
			polymer.at(key)++;
		} else {
			polymer.insert({key, 1});
		}
	}
}

void processPair(string input, unordered_map<string, char>& pairs) {
	string pair;
	string separator;
	char atom;

	stringstream ss(input);
	ss >> pair >> separator >> atom;

	pairs.insert({pair, atom});
}

void doStep(unordered_map<string, uintmax_t>& polymer, unordered_map<string, char>& pairs) {
	unordered_map<string, uintmax_t> newPolymer {};
	string key1;
	string key2;
	char insertElement;

	for (pair<string, uintmax_t> pair : polymer) {
		insertElement = pairs.at(pair.first);
		key1 = {pair.first[0], insertElement};
		key2 = {insertElement, pair.first[1]};

		if (newPolymer.count(key1)) {
			newPolymer.at(key1) += pair.second;
		} else {
			newPolymer.insert({key1, pair.second});
		}

		if (newPolymer.count(key2)) {
			newPolymer.at(key2) += pair.second;
		} else {
			newPolymer.insert({key2, pair.second});
		}
	}

	polymer = newPolymer;
}

/*
We know that whatever the last element of the initial polymer was will be the last element of the final
polymer because nothing ever gets added after that, only before it as it is always the second element in
a pair.

So we can just count the first element in each pair, then add one to whichever element was the inital
last element.
 */
unordered_map<char, uintmax_t> getElementOccurrence(const unordered_map<string, uintmax_t>& polymer, char lastElement) {
	unordered_map<char, uintmax_t> occurrences;

	for (const pair<string, uintmax_t>& pair : polymer) {
		// Count the first element of the pair
		if (occurrences.count(pair.first[0])) {
			occurrences.at(pair.first[0]) += pair.second;
		} else {
			occurrences.insert({pair.first[0], pair.second});
		}
	}

	occurrences.at(lastElement)++; // No pair has the last element in first position, add separately.

	return occurrences;
}

int main() {
	unordered_map<string, uintmax_t> polymer;
	unordered_map<string, char> pairs;
	char lastElement;

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
			lastElement = inputBuffer[inputBuffer.length() - 1];
		}
	}

	for (int i = 0; i < 40; i++) {
		doStep(polymer, pairs);
	}

	unordered_map<char, uintmax_t> elementOccurrences = getElementOccurrence(polymer, lastElement);
	pair<char, uintmax_t> least {'!', std::numeric_limits<uintmax_t>::max()};
	pair<char, uintmax_t> most {'!', 0};
	for (pair<char, uintmax_t> elementOccurrence : elementOccurrences) {
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
