/*
Now, discard the corrupted lines. The remaining lines are incomplete.

Incomplete lines don't have any incorrect characters - instead, they're missing some closing characters at the end of the line. To repair the navigation subsystem, you just need to figure out the sequence of closing characters that complete all open chunks in the line.

You can only use closing characters (), ], }, or >), and you must add them in the correct order so that only legal pairs are formed and all chunks end up closed.

In the example above, there are five incomplete lines:

[({(<(())[]>[[{[]{<()<>> - Complete by adding }}]])})].
[(()[<>])]({[<{<<[]>>( - Complete by adding )}>]}).
(((({<>}<{<{<>}{[]{[]{} - Complete by adding }}>}>)))).
{<[[]]>}<{[{[{[]{()[[[] - Complete by adding ]]}}]}]}>.
<{([{{}}[<[[[<>{}]]]>[]] - Complete by adding ])}>.
Did you know that autocomplete tools also have contests? It's true! The score is determined by considering the completion string character-by-character. Start with a total score of 0. Then, for each character, multiply the total score by 5 and then increase the total score by the point value given for the character in the following table:

): 1 point.
]: 2 points.
}: 3 points.
>: 4 points.
So, the last completion string above - ])}> - would be scored as follows:

Start with a total score of 0.
Multiply the total score by 5 to get 0, then add the value of ] (2) to get a new total score of 2.
Multiply the total score by 5 to get 10, then add the value of ) (1) to get a new total score of 11.
Multiply the total score by 5 to get 55, then add the value of } (3) to get a new total score of 58.
Multiply the total score by 5 to get 290, then add the value of > (4) to get a new total score of 294.
The five lines' completion strings have total scores as follows:

}}]])})] - 288957 total points.
)}>]}) - 5566 total points.
}}>}>)))) - 1480781 total points.
]]}}]}]}> - 995444 total points.
])}> - 294 total points.
Autocomplete tools are an odd bunch: the winner is found by sorting all of the scores and then taking the middle score. (There will always be an odd number of scores to consider.) In this example, the middle score is 288957 because there are the same number of scores smaller and larger than it.

Find the completion string for each incomplete line, score the completion strings, and sort the scores. What is the middle score?
 */
#include <algorithm>
#include <fstream>
#include <iostream>
#include <stack>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

unordered_set<char> openings {'{', '<', '(', '['};
unordered_set<char> closings {'}', '>', ')', ']'};
unordered_map<char, int> scoring {{'(',1}, {'[',2}, {'{',3}, {'<',4}};

// Instead of just storing the current chunkOpener, store all open chunkOpeners.
// When we run out of characters, we can just close out whatever's left
uintmax_t processChunk(stack<char>& chunkOpeners, string_view& input, int indent = 1) {
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
			chunkOpeners.push(c);
			uintmax_t result = processChunk(chunkOpeners, input, indent + 1);
			if (result != 0) {
				return result;
			}
		} else {
			cout << indentString << c << endl;
			// Decrement the count. If it went below 0, it's corrupted and we should break and return score
			char pairChar = c == ')' ? c - 1 : c - 2;
			if (pairChar == chunkOpeners.top()) {
				chunkOpeners.pop();
				return 0;
			} else {
				cout << indentString << "Corrupt" << endl;
				// Skip the rest of this line
				return -1;
			}
		}
	}

	return 0; // No corrupt elements found
}

/*
Something weird happens here.
Within the function, the score appears to be correct. I'm using uintmax_t because some scores are larger than INT_MAX.
However, when the numbers are returned and stored in the vector, some of them get garbled.
So for now I just solved the puzzle by taking the printed output from this function and pasting it into Excel.
 */
// Just pop everything off and add up the scores for the corresponding chars.
uintmax_t closeChunks(stack<char>& chunkOpeners) {
	uintmax_t score = 0;
	char chunkOpener;
	cout << "Closing open chunks: ";
	while (chunkOpeners.size() > 0) {
		chunkOpener = chunkOpeners.top();
		chunkOpeners.pop();
		cout << chunkOpener;
		auto result = scoring.find(chunkOpener);
		score = score * 5 + result->second;
	}
	cout << " " << score << endl;
	return score;
}

// I need to modify this so that within each line it process CHUNKS.
// I don't think I need to change much, just make it so that each time we open a chunk,
// we recurse and process that bit.
uintmax_t processLine(string& input) {
	cout << "Processing: " << input << endl;

	stack<char> chunkOpeners;
	uintmax_t score = 0;
	string_view sv(input);

	while (sv.length() > 0 && score == 0) {
		char c = sv[0];
		sv.remove_prefix(1);
		cout << c << endl;
		chunkOpeners.push(c);
		score += processChunk(chunkOpeners, sv);
		cout << "There are " << sv.length() << " unprocessed characters" << endl;
	}

	if (score == 0) {
		cout << input << " needs to be completed. It has " << chunkOpeners.size() << " open chunks." << endl;
		return closeChunks(chunkOpeners);
	}

	return score;
}

int main() {
	fstream file("10.txt", ios::in);
	string inputBuffer;
	vector<uintmax_t> scores;
	uintmax_t lineScore = 0;

	while (getline(file, inputBuffer)) {
		lineScore = processLine(inputBuffer);
		if (lineScore > 0) {
			scores.push_back(lineScore);
		}
	}

	sort(scores.begin(), scores.end());
	// See what scores we got
	for (uintmax_t score : scores) {
		cout << "Score: " << score << endl;
	}

	uintmax_t middleScore = scores[scores.size() / 2];
	cout << "Middle score is " << middleScore << endl;
}
