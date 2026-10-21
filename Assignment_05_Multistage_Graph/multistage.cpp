
#include <bits/stdc++.h>
using namespace std;

const double INF = 1e18;

struct Edge {
    int to;
    double baseCost;
    double currentCost;
};

int main() {
    int S;

    cout << "Enter number of stages: ";
    cin >> S;

    if (S < 2) {
        cout << "At least 2 stages are required.\n";
        return 0;
    }

    vector<int> count(S), start(S);
    int N = 0;

    cout << "Enter number of nodes in each stage: ";

    for (int i = 0; i < S; i++) {
        cin >> count[i];

        if (count[i] <= 0) {
            cout << "Invalid number of nodes.\n";
            return 0;
        }

        start[i] = N;
        N += count[i];
    }

    // The final stage contains one destination node.
    if (count[S - 1] != 1) {
        cout << "Final stage must have one node.\n";
        return 0;
    }

    int sink = N - 1;

    // Store the stage number of each node.
    vector<int> stage(N);

    for (int s = 0; s < S; s++) {
        for (int k = 0; k < count[s]; k++) {
            stage[start[s] + k] = s;
        }
    }

    int M;
    cout << "Enter number of edges: ";
    cin >> M;

    if (M < 0) {
        cout << "Invalid number of edges.\n";
        return 0;
    }

    // Adjacency list stores outgoing edges.
    vector<vector<Edge>> adj(N);

    cout << "Enter edges (from to cost):\n";

    for (int i = 0; i < M; i++) {
        int u, v;
        double w;

        cin >> u >> v >> w;

        if (u < 0 || u >= N || v < 0 || v >= N ||
            stage[v] != stage[u] + 1 ||
            !isfinite(w) || w < 0 || w >= INF / 4) {
            cout << "Invalid edge or cost.\n";
            return 0;
        }

        adj[u].push_back({v, w, w});
    }

    vector<double> cost(N);
    vector<int> nextNode(N);

    // Calculate minimum costs using backward DP.
    auto calculateDP = [&]() {
        fill(cost.begin(), cost.end(), INF);
        fill(nextNode.begin(), nextNode.end(), -1);

        cost[sink] = 0;

        for (int s = S - 2; s >= 0; s--) {
            for (int k = 0; k < count[s]; k++) {
                int u = start[s] + k;

                for (const auto &e : adj[u]) {
                    int v = e.to;

                    if (cost[v] == INF)
                        continue;

                    double newCost = e.currentCost + cost[v];

                    if (newCost < cost[u]) {
                        cost[u] = newCost;
                        nextNode[u] = v;
                    }
                }
            }
        }
    };

    // Print the shortest path from a source node.
    auto printPath = [&](int src) {
        if (src < 0 || src >= N || stage[src] != 0) {
            cout << "Choose a valid Stage 0 source.\n";
            return;
        }

        if (cost[src] == INF) {
            cout << "No route exists from node "
                 << src << ".\n";
            return;
        }

        cout << "Shortest path: ";
        int u = src;
        cout << u;

        while (u != sink) {
            u = nextNode[u];

            if (u == -1) {
                cout << "\nPath reconstruction failed.\n";
                return;
            }

            cout << " -> " << u;
        }

        cout << "\nMinimum cost: "
             << fixed << setprecision(2)
             << cost[src] << "\n";
    };

    // Display minimum costs for all Stage 0 nodes.
    auto showCosts = [&]() {
        cout << "\nMinimum costs from Stage 0:\n";

        for (int k = 0; k < count[0]; k++) {
            int u = start[0] + k;

            cout << "Node " << u << ": ";

            if (cost[u] == INF)
                cout << "Unreachable\n";
            else
                cout << fixed << setprecision(2)
                     << cost[u] << "\n";
        }
    };

    calculateDP();
    showCosts();

    // Process multiple delivery requests.
    int Q;
    cout << "\nEnter number of delivery requests: ";
    cin >> Q;

    if (Q < 0) {
        cout << "Invalid number of requests.\n";
        return 0;
    }

    for (int i = 0; i < Q; i++) {
        int src;

        cout << "Enter source for request " << i + 1 << ": ";
        cin >> src;

        printPath(src);
    }

    // Update edge costs to simulate traffic or weather changes.
    int U;
    cout << "\nEnter number of live updates (0 to skip): ";
    cin >> U;

    if (U < 0) {
        cout << "Invalid number of updates.\n";
        return 0;
    }

    for (int i = 0; i < U; i++) {
        int u, v;
        double multiplier;

        cout << "Enter edge (from to multiplier): ";
        cin >> u >> v >> multiplier;

        if (u < 0 || u >= N || v < 0 || v >= N ||
            stage[v] != stage[u] + 1 ||
            !isfinite(multiplier) ||
            multiplier < 0 || multiplier > 1e6) {
            cout << "Invalid update. Skipped.\n";
            continue;
        }

        bool found = false;

        for (auto &e : adj[u]) {
            if (e.to == v) {
                double newCost = e.baseCost * multiplier;

                if (!isfinite(newCost) || newCost >= INF / 4) {
                    cout << "Updated cost is too large. Skipped.\n";
                    continue;
                }

                e.currentCost = newCost;
                found = true;
            }
        }

        if (!found) {
            cout << "Edge not found. Update skipped.\n";
            continue;
        }

        cout << "Edge updated successfully.\n";

        // Recalculate shortest paths after the update.
        calculateDP();
        showCosts();

        int src;
        cout << "Enter source for updated path (-1 to skip): ";
        cin >> src;

        if (src != -1)
            printPath(src);
    }

    cout << "\nFinal shortest paths:\n";

    for (int k = 0; k < count[0]; k++) {
        printPath(start[0] + k);
    }

    return 0;
}
