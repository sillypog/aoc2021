/*
 * Find your way through the cave system.
 *
 * The input is a list of connected nodes, eg AB->bc
 *
 * Nodes in uppercase can be visited any number of times.
 * Nodes in lowercase can only be visited once EXCEPT one of these caves
 * can be visited one additional time.
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

void findPath(Node* node, vector<Node*> route, Node* repeatNode, vector<vector<Node*>>& goodRoutes) {
	route.push_back(node);
	// If we made it to the end, add the route to the list of good routes
	if (node->getID() == "end") {
		goodRoutes.push_back(route);
	}
	// For each neighbor of node that can be explored, do so
	for (int i = 0; i < node->numNeighbors(); i++) {
		Node* next = node->getNeighbor(i);
		// Only check this node if it's not in the route or it can be revisited
		if (next->canRevisit()) {
			// If we can revisit this node, it doesn't matter what other rules are in place
			findPath(next, route, repeatNode, goodRoutes);
		} else if (find(route.begin(), route.end(), next) != route.end() && repeatNode == nullptr) {
			// The node is already in the route, but we're allowed one revisit
			findPath(next, route, next, goodRoutes);
		} else if (find(route.begin(), route.end(), next) == route.end()) {
			// The node isn't in the route yet so it's fine to proceed
			findPath(next, route, repeatNode, goodRoutes);
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
		// Can't revisit start or end
		// End should not have any outgoing neighbors
		// Start should not have any incoming neighbors, ie no nodes should list start as a neighbor
		if (node1->getID() != "end" && node2->getID() != "start") {
			node1->setNeighbor(node2);
		}
		if (node2->getID() != "end" && node1->getID() != "start") {
			node2->setNeighbor(node1);
		}
	}

	vector<vector<Node*>> goodRoutes;
	findPath(caveIndex.at("start"), {}, nullptr, goodRoutes);
	cout << "There were " << goodRoutes.size() << " routes." << endl;
}
