#include <iostream>
#include <iomanip>
#include <vector>
#include <queue>
#include <stack>
#include <climits>

struct Edge {
    int u, v, w;
};

// Reconstructs and prints the path from source to target using parent array
void printPath(int source, int target, const std::vector<int>& parent) {
    if (target == source) {
        std::cout << source;
        return;
    }
    if (parent[target] == -1) {
        std::cout << "No path";
        return;
    }
    // Trace back from target to source, then print in order
    std::stack<int> path;
    int curr = target;
    while (curr != -1) {
        path.push(curr);
        curr = parent[curr];
    }
    while (!path.empty()) {
        std::cout << path.top();
        path.pop();
        if (!path.empty()) std::cout << " -> ";
    }
}

void bellmanFord(int V, int source, const std::vector<Edge>& edges) {
    const long long INF = LLONG_MAX;

    // Step 1: initialise distances and parent tracker
    std::vector<long long> dist(V, INF);
    std::vector<int> parent(V, -1);  // parent[v] = which vertex we came from to reach v
    dist[source] = 0;

    // Step 2: relax all edges V-1 times
    // Shortest path with no negative cycles uses at most V-1 edges
    for (int i = 0; i < V - 1; i++) {
        bool updated = false;
        for (const auto& e : edges) {
            if (dist[e.u] != INF && dist[e.u] + e.w < dist[e.v]) {
                dist[e.v] = dist[e.u] + e.w;
                parent[e.v] = e.u;  // record how we got to e.v
                updated = true;
            }
        }
        if (!updated) break; // converged early
    }

    // Step 3: negative cycle detection - one extra pass
    // If we can still relax an edge, a negative cycle exists
    std::vector<bool> inNegCycle(V, false);
    for (const auto& e : edges) {
        if (dist[e.u] != INF && dist[e.u] + e.w < dist[e.v]) {
            inNegCycle[e.v] = true;
        }
    }

    // BFS to propagate -INF to all vertices reachable FROM any negative-cycle vertex
    std::queue<int> q;
    for (int i = 0; i < V; i++) {
        if (inNegCycle[i]) q.push(i);
    }

    std::vector<std::vector<int>> adj(V);
    for (const auto& e : edges) {
        adj[e.u].push_back(e.v);
    }

    while (!q.empty()) {
        int node = q.front(); q.pop();
        for (int next : adj[node]) {
            if (!inNegCycle[next]) {
                inNegCycle[next] = true;
                q.push(next);
            }
        }
    }

    bool hasNegCycle = false;
    for (int i = 0; i < V; i++) {
        if (inNegCycle[i]) { hasNegCycle = true; break; }
    }

    // Step 4: output distances and paths to all other vertices
    std::cout << "\nSource vertex: " << source << " (distance = 0)\n";
    std::cout << "\nShortest distances to all other vertices:\n";
    std::cout << std::string(60, '-') << "\n";
    std::cout << std::left
              << std::setw(10) << "  Vertex"
              << std::setw(16) << "  Distance"
              << "  Path\n";
    std::cout << std::string(60, '-') << "\n";

    for (int i = 0; i < V; i++) {
        if (i == source) continue; // question asks for all OTHER vertices

        std::cout << "  " << std::setw(8) << i;

        if (dist[i] == INF) {
            std::cout << std::setw(16) << "  UNREACHABLE" << "  N/A\n";
        } else if (inNegCycle[i]) {
            std::cout << std::setw(16) << "  -INF" << "  (negative cycle - unbounded)\n";
        } else {
            std::cout << "  " << std::setw(14) << dist[i] << "  ";
            printPath(source, i, parent);
            std::cout << "\n";
        }
    }

    std::cout << std::string(60, '-') << "\n";
    if (hasNegCycle) {
        std::cout << "Negative weight cycle detected in graph.\n";
    } else {
        std::cout << "No negative weight cycles detected.\n";
    }
}

int main() {
    int V, E;
    std::cout << "Enter number of vertices (V) and edges (E): ";
    std::cin >> V >> E;

    if (V <= 0 || E < 0) {
        std::cerr << "Invalid input: V must be > 0 and E >= 0.\n";
        return 1;
    }

    std::vector<Edge> edges(E);
    std::cout << "Enter " << E << " edges as (u v w):\n";
    for (int i = 0; i < E; i++) {
        std::cin >> edges[i].u >> edges[i].v >> edges[i].w;
        if (edges[i].u < 0 || edges[i].u >= V || edges[i].v < 0 || edges[i].v >= V) {
            std::cerr << "Invalid edge: vertex out of range [0, " << V - 1 << "].\n";
            return 1;
        }
    }

    int source;
    std::cout << "Enter source vertex (0 to " << V - 1 << "): ";
    std::cin >> source;

    if (source < 0 || source >= V) {
        std::cerr << "Invalid source vertex.\n";
        return 1;
    }

    bellmanFord(V, source, edges);

    return 0;
}
