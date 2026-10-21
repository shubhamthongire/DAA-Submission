#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <algorithm>

using namespace std;

// Edge class
class Edge {
public:
    int v;      // Destination vertex
    int wt;     // Weight / travel time

    Edge(int v, int wt) {
        this->v = v;
        this->wt = wt;
    }
};

// Structure to store shortest path result
class Result {
public:
    int distance;
    vector<int> path;

    Result(int distance, vector<int> path) {
        this->distance = distance;
        this->path = path;
    }
};

// Dijkstra's Algorithm
Result dijkstra(int source,
                vector<vector<Edge>>& graph,
                vector<int>& hospitals) {

    int V = graph.size();

    // Distance array
    vector<int> dist(V, INT_MAX);

    // Parent array for path reconstruction
    vector<int> parent(V, -1);

    // Min Heap
    priority_queue<
        pair<int, int>,
        vector<pair<int, int>>,
        greater<pair<int, int>>
    > pq;

    // Distance of source = 0
    dist[source] = 0;

    pq.push({0, source});

    while (!pq.empty()) {

        int u = pq.top().second;
        int currentDist = pq.top().first;

        pq.pop();

        // Ignore outdated entry
        if (currentDist > dist[u])
            continue;

        // Check whether current node is a hospital
        bool isHospital = false;

        for (int h : hospitals) {
            if (u == h) {
                isHospital = true;
                break;
            }
        }

        // Since this is the closest unprocessed node,
        // if it is a hospital, we found the nearest hospital.
        if (isHospital) {

            vector<int> path;

            int current = u;

            // Reconstruct path using parent array
            while (current != -1) {
                path.push_back(current);
                current = parent[current];
            }

            reverse(path.begin(), path.end());

            return Result(dist[u], path);
        }

        // Relax all adjacent edges
        for (Edge edge : graph[u]) {

            int v = edge.v;
            int wt = edge.wt;

            if (dist[u] != INT_MAX &&
                dist[v] > dist[u] + wt) {

                dist[v] = dist[u] + wt;

                // Store parent for path reconstruction
                parent[v] = u;

                pq.push({dist[v], v});
            }
        }
    }

    // No hospital is reachable
    return Result(-1, {});
}


// Function to add an undirected edge
void addEdge(vector<vector<Edge>>& graph,
             int u, int v, int wt) {

    graph[u].push_back(Edge(v, wt));
    graph[v].push_back(Edge(u, wt));
}


// Function to update edge weight
void updateEdgeWeight(vector<vector<Edge>>& graph,
                      int u, int v, int newWeight) {

    // Update u -> v
    for (Edge& edge : graph[u]) {
        if (edge.v == v) {
            edge.wt = newWeight;
        }
    }

    // Update v -> u
    for (Edge& edge : graph[v]) {
        if (edge.v == u) {
            edge.wt = newWeight;
        }
    }
}


// Function to print path
void printPath(vector<int>& path) {

    for (int i = 0; i < path.size(); i++) {

        cout << path[i];

        if (i != path.size() - 1)
            cout << " -> ";
    }

    cout << endl;
}


int main() {

    int V, E;

    // ---------------- INPUT GRAPH ----------------

    cout << "Enter number of intersections (vertices): ";
    cin >> V;

    cout << "Enter number of roads (edges): ";
    cin >> E;

    vector<vector<Edge>> graph(V);

    cout << "Enter edges (u v w):" << endl;

    for (int i = 0; i < E; i++) {

        int u, v, w;

        cin >> u >> v >> w;

        // Roads are bidirectional
        addEdge(graph, u, v, w);
    }


    // ---------------- HOSPITAL LOCATIONS ----------------

    int H;

    cout << "Enter number of hospitals: ";
    cin >> H;

    vector<int> hospitals(H);

    cout << "Enter hospital nodes: ";

    for (int i = 0; i < H; i++) {
        cin >> hospitals[i];
    }


    // ---------------- AMBULANCE LOCATION ----------------

    int source;

    cout << "Enter ambulance starting location: ";
    cin >> source;


    // ---------------- INITIAL SHORTEST PATH ----------------

    Result result = dijkstra(source, graph, hospitals);

    if (result.distance != -1) {

        cout << "\nInitial shortest travel time: "
             << result.distance << " minutes" << endl;

        cout << "Path: ";
        printPath(result.path);

        cout << "Nearest hospital: "
             << result.path.back() << endl;
    }
    else {

        cout << "\nNo hospital reachable." << endl;
    }


    // ---------------- REAL-TIME UPDATE ----------------

    int u, v, newWeight;

    cout << "\nEnter edge to update (u v newWeight): ";
    cin >> u >> v >> newWeight;

    updateEdgeWeight(graph, u, v, newWeight);


    // ---------------- RECOMPUTE SHORTEST PATH ----------------

    Result newResult = dijkstra(source, graph, hospitals);

    if (newResult.distance != -1) {

        cout << "\nAfter traffic update:" << endl;

        cout << "New shortest travel time: "
             << newResult.distance << " minutes" << endl;

        cout << "New path: ";
        printPath(newResult.path);

        cout << "Nearest hospital: "
             << newResult.path.back() << endl;
    }
    else {

        cout << "\nNo hospital reachable after update." << endl;
    }


    return 0;
}