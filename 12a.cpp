/*
 * Find your way through the cave system.
 *
 * The input is a list of connected nodes, eg AB->bc
 *
 * Nodes in uppercase can be visited any number of times.
 * Nodes in lowercase can only be visited once.
 *
 * Calculate how many routes there are from start to end.
 */
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <stack>
#include <unordered_map>
#include <vector>
#include <utility>

using namespace std;

class Node {
private:
	string id;
	vector<Node*> neighbors;

public:
	Node(string _id) : id(_id) {}

	const string& getID() const {
		return id;
	}

	int numNeighbors() const {
		return neighbors.size();
	}

	void setNeighbor(Node* neighbor) {
		neighbors.push_back(neighbor);
	}

	Node* getNeighbor(int pos) {
		return neighbors[pos];
	}

	/* Node* getNeighbor(string id) { */
	/* 	for (Node* node : neighbors) { */
	/* 		if (node->getID() == id) { */
	/* 			return node; */
	/* 		} */
	/* 	} */
	/* 	return nullptr; */
	/* } */

	bool canRevisit() {
		return id[0] < 'a' || id == "end"; // Can revisit if represented in uppercase
	}
};

pair<string, string> parseLine(string input) {
	string node1;
	string node2;
	stringstream ss(input);

	getline(ss, node1, '-');
	getline(ss, node2, '-');

	return {node1, node2};
}

Node* getOrCreateNode(string nodeID, unordered_map<string, Node*>& caveIndex) {
	if (caveIndex.count(nodeID) > 0) {
		return caveIndex.at(nodeID);
	} else {
		Node* node = new Node(nodeID);
		caveIndex.insert({nodeID, node});
		return node;
	}
}

void findPath(Node* node, vector<Node*> route, vector<vector<Node*>>& goodRoutes) {
	route.push_back(node);
	// If this is the end, print the route
	if (node->getID() == "end") {
		goodRoutes.push_back(route);
		cout << "Got to the end via: ";
		for (Node* n : route) {
			cout << n->getID() << " ";
		}
		cout<< endl;
	}
	// For each neighbor of node that can be explored, do so
	for (int i = 0; i < node->numNeighbors(); i++) {
		Node* next = node->getNeighbor(i);
		// Only check this node if it's not in the route or it can be revisited
		if (next->canRevisit() || find(route.begin(), route.end(), next) == route.end()) {
			findPath(next, route, goodRoutes); // We want a copy of route so we don't get data from lower levels added
		}
	}
}

int main() {
	unordered_map<string, Node*> caveIndex;

	fstream file("12.txt", ios::in);
	string inputBuffer;
	while (file >> inputBuffer) {
		pair<string, string> nodeIDs = parseLine(inputBuffer);

		Node* node1 = getOrCreateNode(nodeIDs.first, caveIndex);
		Node* node2 = getOrCreateNode(nodeIDs.second, caveIndex);
		if (node1->getID() != "end") {
			node1->setNeighbor(node2);
		}
		if (node2->getID() != "end") {
			node2->setNeighbor(node1);
		}
	}

	vector<vector<Node*>> goodRoutes;
	findPath(caveIndex.at("start"), {}, goodRoutes);
	cout << "There were " << goodRoutes.size() << " routes." << endl;

	// Manually explore the cave.
	/* Node* start = caveIndex.at("start"); */
	/* Node* next; */
	/* string nextID; */
	/* while (true) { */
	/* 	cout << "Enter a neighbor: "; */
	/* 	cin >> nextID; */
	/* 	next = start->getNeighbor(nextID); */
	/* 	if (next != nullptr) { */
	/* 		start = next; */
	/* 	} else { */
	/* 		cout << "Neighbor not found." << endl; */
	/* 	} */
	/* } */
}
