/*
--- Day 10: Syntax Scoring ---

You ask the submarine to determine the best route out of the deep-sea cave, but it only replies:

Syntax error in navigation subsystem on line: all of them
All of them?! The damage is worse than you thought. You bring up a copy of the navigation subsystem (your puzzle input).

The navigation subsystem syntax is made of several lines containing chunks. There are one or more chunks on each line, and chunks contain zero or more other chunks. Adjacent chunks are not separated by any delimiter; if one chunk stops, the next chunk (if any) can immediately start. Every chunk must open and close with one of four legal pairs of matching characters:

If a chunk opens with (, it must close with ).
If a chunk opens with [, it must close with ].
If a chunk opens with {, it must close with }.
If a chunk opens with <, it must close with >.
So, () is a legal chunk that contains no other chunks, as is []. More complex but valid chunks include ([]), {()()()}, <([{}])>, [<>({}){}[([])<>]], and even (((((((((()))))))))).

Some lines are incomplete, but others are corrupted. Find and discard the corrupted lines first.

A corrupted line is one where a chunk closes with the wrong character - that is, where the characters it opens and closes with do not form one of the four legal pairs listed above.

Examples of corrupted chunks include (], {()()()>, (((()))}, and <([]){()}[{}]). Such a chunk can appear anywhere within a line, and its presence causes the whole line to be considered corrupted.

For example, consider the following navigation subsystem:

[({(<(())[]>[[{[]{<()<>>
[(()[<>])]({[<{<<[]>>(
{([(<{}[<>[]}>{[]{[(<()>
(((({<>}<{<{<>}{[]{[]{}
[[<[([]))<([[{}[[()]]]
[{[{({}]{}}([{[{{{}}([]
{<[[]]>}<{[{[{[]{()[[[]
[<(<(<(<{}))><([]([]()
<{([([[(<>()){}]>(<<{{
<{([{{}}[<[[[<>{}]]]>[]]
Some of the lines aren't corrupted, just incomplete; you can ignore these lines for now. The remaining five lines are corrupted:

{([(<{}[<>[]}>{[]{[(<()> - Expected ], but found } instead.
[[<[([]))<([[{}[[()]]] - Expected ], but found ) instead.
[{[{({}]{}}([{[{{{}}([] - Expected ), but found ] instead.
[<(<(<(<{}))><([]([]() - Expected >, but found ) instead.
<{([([[(<>()){}]>(<<{{ - Expected ], but found > instead.
Stop at the first incorrect closing character on each corrupted line.

Did you know that syntax checkers actually have contests to see who can get the high score for syntax errors in a file? It's true! To calculate the syntax error score for a line, take the first illegal character on the line and look it up in the following table:

): 3 points.
]: 57 points.
}: 1197 points.
>: 25137 points.
In the above example, an illegal ) was found twice (2*3 = 6 points), an illegal ] was found once (57 points), an illegal } was found once (1197 points), and an illegal > was found once (25137 points). So, the total syntax error score for this file is 6+57+1197+25137 = 26397 points!

Find the first illegal character in each corrupted line of the navigation subsystem. What is the total syntax error score for those errors?
 */
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>

using namespace std;

unordered_set<char> openings {'{', '<', '(', '['};
unordered_set<char> closings {'}', '>', ')', ']'};
unordered_map<char, int> scoring {{')',3}, {']',57}, {'}',1197}, {'>',25137}};

int processChunk(char chunkOpener, string_view& input, int indent = 1) {
	string indentString;
	for (int i = 0; i < indent - 1; i++) {
		indentString += " ";
	}

	while (input.length() > 0) {
		char c; // The next item in the input
		c = input[0];
		input.remove_prefix(1); // Advance the string view for next round

		if (openings.count(c)) {
			cout << indentString << " " << c << endl;
			// Process the chunk within this
			int result = processChunk(c, input, indent + 1);
			if (result > 0) {
				return result;
			}
		} else {
			cout << indentString << c << endl;
			// Decrement the count. If it went below 0, it's corrupted and we should break and return score
			char pairChar = c == ')' ? c - 1 : c - 2;
			if (pairChar == chunkOpener) {
				return 0;
			} else {
				cout << indentString << "Corrupt" << endl;
				// Return the score for this character
				auto score = scoring.find(c);
				return score->second;
			}
		}
	}

	return 0; // No corrupt elements found

}

// I need to modify this so that within each line it process CHUNKS.
// I don't think I need to change much, just make it so that each time we open a chunk,
// we recurse and process that bit.
int processLine(string& input) {
	cout << "Processing: " << input << endl;

	int score = 0;
	string_view sv(input);

	while (sv.length() > 0 && score == 0) {
		char c = sv[0];
		sv.remove_prefix(1);
		cout << c << endl;
		score += processChunk(c, sv);
		cout << "There are " << sv.length() << " unprocessed characters" << endl;
	}

	return score;
}

int main() {
	fstream file("10.txt", ios::in);
	string inputBuffer;
	int score = 0;

	while (getline(file, inputBuffer)) {
		score += processLine(inputBuffer);
	}

	cout << "Syntax score is " << score << endl;
}
