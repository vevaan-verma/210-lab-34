// COMSC-210 | Lab 34 | Vevaan Verma
using namespace std;
#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <utility>

struct Edge {

	int src, dest, weight;

};

typedef pair<int, int> Pair;  // Creates an alias 'Pair' for the pair<int, int> data type

class Graph {

public:
	vector<vector<Pair>> adjList;
	int SIZE;

	// constructor
	Graph(vector<Edge> const& edges, int size) {

		SIZE = size;
		adjList.resize(SIZE);

		for (auto& edge : edges) {

			int src = edge.src;
			int dest = edge.dest;
			int weight = edge.weight;
			adjList[src].push_back(make_pair(dest, weight));
			adjList[dest].push_back(make_pair(src, weight));  // For undirected graph

		}
	}

	// DFS() is the Depth First Search algorithm
	// arguments: int start - the starting node
	// returns: void
	void DFS(int start) {

		vector<bool> visited(SIZE, false);
		stack<int> s;
		s.push(start);
		visited[start] = true;

		while (!s.empty()) {

			int node = s.top();
			s.pop();
			cout << node << " ";

			for (auto& neighbor : adjList[node]) {

				int adjNode = neighbor.first;

				if (!visited[adjNode]) {

					visited[adjNode] = true;
					s.push(adjNode);

				}
			}
		}

		cout << endl;

	}

	// BFS() is the Breadth First Search algorithm
	// arguments: int start - the starting node
	// returns: void
	void BFS(int start) {

		vector<bool> visited(SIZE, false);
		queue<int> q;
		q.push(start);
		visited[start] = true;

		while (!q.empty()) {

			int node = q.front();
			q.pop();
			cout << node << " ";

			for (auto& neighbor : adjList[node]) {

				int adjNode = neighbor.first;

				if (!visited[adjNode]) {

					visited[adjNode] = true;
					q.push(adjNode);

				}
			}
		}

		cout << endl;

	}

	void printGraph() {

		for (int i = 0; i < adjList.size(); i++) {

			cout << i << " -> ";

			for (Pair v : adjList[i])
				cout << "(" << v.first << ", " << v.second << ") ";

			cout << endl;

		}
	}
};

// main() is the entry point of the program and drives the program
// arguments: none
// returns: int - the exit code of the program
int main() {

	vector<Edge> edges = {
		{0,1,12},{0,2,8},{0,3,21},{2,3,6},{2,6,2},{5,6,6},{4,5,9},{2,4,4},{2,5,5}
	};

	int numNodes = 7;
	Graph graph(edges, numNodes);

	cout << "Graph's adjacency list:" << endl;
	graph.printGraph();

	cout << "DFS starting from vertex 0:" << endl;
	graph.DFS(0);

	cout << "BFS starting from vertex 0:" << endl;
	graph.BFS(0);

	return 0;

}