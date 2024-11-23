// COMSC-210 | Lab 34 | Vevaan Verma
using namespace std;
#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <utility>

class Graph;

/* FUNCTION PROTOTYPES */
void outputMenu();
void handleMenu(int choice, Graph& trafficNetwork);

struct Edge {
	int src, dest, weight;  // source intersection, destination intersection, and traffic flow
};

typedef pair<int, int> Pair;

// Disjoint Set Union (DSU) class for Kruskal's Algorithm
class DSU {
	vector<int> parent, rank;

public:
	DSU(int size) {

		parent.resize(size);
		rank.resize(size, 0);
		for (int i = 0; i < size; i++) parent[i] = i;

	}

	int find(int x) {

		if (parent[x] != x) parent[x] = find(parent[x]);
		return parent[x];

	}

	void unite(int x, int y) {

		int rootX = find(x);
		int rootY = find(y);

		if (rootX != rootY) {

			if (rank[rootX] > rank[rootY]) {

				parent[rootY] = rootX;

			} else if (rank[rootX] < rank[rootY]) {

				parent[rootX] = rootY;

			} else {

				parent[rootY] = rootX;
				rank[rootX]++;

			}
		}
	}
};

class Graph {

public:
	vector<vector<Pair>> adjList;
	int SIZE;
	vector<Edge> edges;  // Keep track of all edges for MST

	Graph(vector<Edge> const& edges, int size) {

		SIZE = size;
		adjList.resize(SIZE);
		this->edges = edges;  // Store all edges

		for (auto& edge : edges) {

			int src = edge.src;
			int dest = edge.dest;
			int weight = edge.weight;
			adjList[src].push_back(make_pair(dest, weight));
			adjList[dest].push_back(make_pair(src, weight)); // for bidirectional roads

		}
	}

	// printTrafficNetwork() - prints the traffic network topology
	// arguments: none
	// returns: void
	void printTrafficNetwork() {

		cout << "Traffic Network Topology:\n===========================\n";

		for (int i = 0; i < adjList.size(); i++) {

			cout << "Intersection " << i << " connects to:\n";

			for (Pair road : adjList[i])
				cout << "  -> Intersection " << road.first << " (Traffic Flow: " << road.second << " vehicles/min)\n";

		}

		cout << endl;

	}

	// analyzeTrafficDFS() - performs a depth-first search on the traffic network
	// arguments: int start - the starting intersection to analyze
	// returns: void
	void analyzeTrafficDFS(int start) {

		cout << "Traffic Congestion Trace (DFS) from Intersection " << start << ":\n";
		vector<bool> visited(SIZE, false);
		stack<int> s;
		s.push(start);
		visited[start] = true;

		while (!s.empty()) {

			int node = s.top();
			s.pop();
			cout << "Inspecting Intersection " << node << endl;

			for (auto& neighbor : adjList[node]) {

				int adjNode = neighbor.first;

				if (!visited[adjNode]) {

					cout << "  -> Possible congestion spread to Intersection " << adjNode
						<< " (Traffic Flow: " << neighbor.second << " vehicles/min)\n";
					visited[adjNode] = true; // mark as visited
					s.push(adjNode); // push to stack

				}
			}
		}

		cout << endl;

	}

	// analyzeTrafficBFS() - performs a breadth-first search on the traffic network
	// arguments: int start - the starting intersection to analyze
	// returns: void
	void analyzeTrafficBFS(int start) {

		cout << "Layer-by-Layer Traffic Analysis (BFS) from Intersection " << start << ":\n";
		vector<bool> visited(SIZE, false);
		queue<int> q;
		q.push(start);
		visited[start] = true;

		while (!q.empty()) {

			int node = q.front();
			q.pop();
			cout << "Checking Intersection " << node << endl;

			for (auto& neighbor : adjList[node]) {

				int adjNode = neighbor.first;

				if (!visited[adjNode]) {

					cout << "  -> Next area to inspect: Intersection " << adjNode
						<< " (Traffic Flow: " << neighbor.second << " vehicles/min)\n";
					visited[adjNode] = true; // mark as visited
					q.push(adjNode); // push to queue

				}
			}
		}

		cout << endl;

	}

	// findShortestPaths() - calculates the shortest paths from the start node
	// arguments: int start - the starting intersection
	// returns: void
	void findShortestPaths(int start) {

		vector<int> dist(SIZE, numeric_limits<int>::max());
		priority_queue<Pair, vector<Pair>, greater<>> pq;
		dist[start] = 0;
		pq.push({ 0, start });

		while (!pq.empty()) {

			int currDist = pq.top().first;
			int currNode = pq.top().second;
			pq.pop();

			// skip outdated pairs
			if (currDist > dist[currNode]) continue;

			for (auto& neighbor : adjList[currNode]) {

				int adjNode = neighbor.first;
				int weight = neighbor.second;

				if (dist[currNode] + weight < dist[adjNode]) {

					dist[adjNode] = dist[currNode] + weight;
					pq.push({ dist[adjNode], adjNode });

				}
			}
		}

		// output the shortest paths
		cout << "Shortest path from Intersection " << start << ":\n";

		for (int i = 0; i < SIZE; i++) {

			cout << start << " -> " << i << " : ";

			if (dist[i] == numeric_limits<int>::max())
				cout << "∞ (unreachable)\n";
			else
				cout << dist[i] << " minutes\n";

		}

		cout << endl;

	}

	void findMST() {

		sort(edges.begin(), edges.end(), [](Edge a, Edge b) { return a.weight < b.weight; }); // sort edges by weight

		DSU dsu(SIZE);
		vector<Edge> mst;

		for (auto& edge : edges) { // iterate through all edges

			if (dsu.find(edge.src) != dsu.find(edge.dest)) { // if the source and destination are not in the same set

				dsu.unite(edge.src, edge.dest); // unite the sets
				mst.push_back(edge); // add the edge to the MST

			}
		}

		// output the MST edges
		cout << "Minimum Spanning Tree edges:\n";

		for (auto& edge : mst)
			cout << "Edge from " << edge.src << " to " << edge.dest << " with capacity: " << edge.weight << " units\n";

		cout << endl;

	}
};

// main() is the entry point of the program and drives the program
// arguments: none
// returns: int - the exit code of the program
int main() {

	vector<Edge> edges = {
		{1,0,8},{2,0,21},{2,1,6},{3,1,5},{4,1,4},{7,2,11},{8,2,8},
		{4,3,9},{6,5,10},{7,5,15},{8,5,5},{7,6,3},{8,6,7}
	};

	int numIntersections = 9; // intersections range from 0 - 8
	Graph trafficNetwork(edges, numIntersections);

	while (true) {

		outputMenu();
		int choice;
		cin >> choice;

		if (choice == 0) // exit the program
			break;

		handleMenu(choice, trafficNetwork);

	}

	return 0;

}

// outputMenu() - outputs the menu for the traffic network program
// arguments: none
// returns: void
void outputMenu() {

	cout << "Traffic Network Menu:" << endl;
	cout << "[1] Display traffic network" << endl;
	cout << "[2] Check congestion spread (BFS)" << endl;
	cout << "[3] Plan inspection route (DFS)" << endl;
	cout << "[4] Calculate shortest paths" << endl;
	cout << "[5] Find Minimum Spanning Tree" << endl;
	cout << "[0] Exit" << endl;
	cout << "Enter your choice: ";

}

// handleMenu() - handles the user's menu choice
// arguments: int choice - the user's menu choice, Graph& trafficNetwork - the traffic network graph
// returns: void
void handleMenu(int choice, Graph& trafficNetwork) {

	cout << endl; // output blank line for formatting purposes
	int start;

	switch (choice) {

	case 1:
		trafficNetwork.printTrafficNetwork();
		break;

	case 2:
		cout << "Enter starting intersection for BFS: ";
		cin >> start;
		trafficNetwork.analyzeTrafficBFS(start);
		break;

	case 3:
		cout << "Enter starting intersection for DFS: ";
		cin >> start;
		trafficNetwork.analyzeTrafficDFS(start);
		break;

	case 4:
		cout << "Enter starting intersection for shortest paths: ";
		cin >> start;
		trafficNetwork.findShortestPaths(start);
		break;

	case 5:
		trafficNetwork.findMST();
		break;

	default:
		cout << "Invalid choice. Please try again.\n";

	}
}