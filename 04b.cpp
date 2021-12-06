/*
On the other hand, it might be wise to try a different strategy: let the giant squid win.

You aren't sure how many bingo boards a giant squid could play at once, so rather than waste time counting its arms, the safe thing to do is to figure out which board will win last and choose that one. That way, no matter which boards it picks, it will win for sure.

In the above example, the second board is the last to win, which happens after 13 is eventually called and its middle column is completely marked. If you were to keep playing until this point, the second board would have a sum of unmarked numbers equal to 148 for a final score of 148 * 13 = 1924.

Figure out which board will win last. Once it wins, what would its final score be?
 */
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

struct IndexEntry {
	int board;
	int column;
	int row;
};

struct Space {
	int value = 0;
	bool marked = false;
};

class Board {
public:
	Space spaces[5][5]; // Shouldn't be public
	bool done = false;

	bool checkBingo(int row, int column) {
		// Mark element
		spaces[row][column].marked = true;
		// Then check the row and column
		bool allRow = true;
		bool allColumn = true;
		for (int i = 0; i < 5; i++) {
			if (!spaces[row][i].marked) {
				allRow = false;
			}
			if (!spaces[i][column].marked) {
				allColumn =false;
			}
		}

		return allRow || allColumn;
	}

	int score(int draw) {
		int sum = 0;
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				sum += spaces[i][j].marked ? 0 : spaces[i][j].value;
			}
		}
		return sum * draw;
	}
};

int main() {
	vector<int> draws;
	vector<Board> boards;
	unordered_multimap<int, IndexEntry> index;
	string inputLine;

	fstream file("04.txt", ios::in);

	getline(file, inputLine);
	stringstream inputLineStream(inputLine);

	// Read the first line which contains draws
	string drawString;
	while (getline(inputLineStream, drawString, ',')) {
		draws.push_back(stoi(drawString));
	}

	// Read the remaining lines into boards
	int value;
	int i  = 0;
	int j = 0;
	Board board;
	while (file >> value) {
		// Write the value into the index
		index.insert({value, {static_cast<int>(boards.size()), i, j}});

		// Write the value into the current slot of the temp board
		board.spaces[j][i] = {value, false};
		i++;
		// Move to next row
		if (i == 5) {
			i = 0;
			j++;
		}// When the board is full, push it onto the vector
		if (j == 5) {
			boards.push_back(board);
			i = 0;
			j = 0;
		}

	}

	int completedBoards = 0;
	for (int draw : draws) {
		// Check the index to get all the matches to this draw
		auto hits = index.equal_range(draw);

		for (auto it = hits.first; it != hits.second; it++) {
			// Check bingo for that location
			Board* board = &boards[it->second.board];
			if (board->done) {
				continue;
			}

			if (board->spaces[it->second.row][it->second.column].value != draw) {
				cout << "Index value mismatch!" << endl;
				exit(EXIT_FAILURE);
			}

			bool bingo = board->checkBingo(it->second.row, it->second.column);
			if (bingo) {
				board->done = true;
				completedBoards++;
				// We only want to stop if this is the last board. If it isn't, throw it out and keep going.
				if (completedBoards == boards.size()) {
					cout << "Bingo with value " << draw << " on board " << it->second.board;
					cout << ". Score is " << board->score(draw) << endl;
					exit(EXIT_SUCCESS);
				}
			}
		}
	}
}
