#include <iostream>
#include <vector>
#include <climits>
#include <iomanip>
#include <string>

const long long INF = 1e15;

// Part (a): Floyd-Warshall all-pairs shortest path
// dist[i][j] = shortest path distance from vertex i to vertex j
// The algorithm builds this up by allowing each vertex k as an intermediate
void floydWarshall(int V, std::vector<std::vector<long long>>& dist) {
    for (int k = 0; k < V; k++) {
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                if (dist[i][k] < INF && dist[k][j] < INF) {
                    dist[i][j] = std::min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
    }
}

// Part (b): Negative cycle detection and propagation
// After Floyd-Warshall, dist[i][i] < 0 means vertex i is on a negative cycle.
// Any pair (i,j) whose path goes through such a vertex has distance -INF.
void markNegativeCyclePairs(int V, std::vector<std::vector<long long>>& dist,
                             std::vector<bool>& onNegCycle) {
    // Step 1: find all vertices sitting on a negative cycle
    for (int i = 0; i < V; i++) {
        if (dist[i][i] < 0) onNegCycle[i] = true;
    }

    // Step 2: a pair (i,j) is -INF if there exists any vertex k on a negative cycle
    // such that i can reach k AND k can reach j
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            for (int k = 0; k < V; k++) {
                if (onNegCycle[k] && dist[i][k] < INF && dist[k][j] < INF) {
                    dist[i][j] = -INF; // mark as unbounded
                }
            }
        }
    }
}

// Part (c): Print the final distance matrix
void printDistanceMatrix(int V, const std::vector<std::vector<long long>>& dist) {
    std::cout << "\nAll-Pairs Shortest Path Distance Matrix:\n";
    std::cout << std::string(6 + V * 9, '-') << "\n";

    // Header
    std::cout << std::setw(6) << " ";
    for (int j = 0; j < V; j++) {
        std::cout << std::setw(9) << j;
    }
    std::cout << "\n";
    std::cout << std::string(6 + V * 9, '-') << "\n";

    // Rows
    for (int i = 0; i < V; i++) {
        std::cout << std::setw(4) << i << " |";
        for (int j = 0; j < V; j++) {
            if (dist[i][j] == -INF) {
                std::cout << std::setw(9) << "-INF";
            } else if (dist[i][j] >= INF) {
                std::cout << std::setw(9) << "INF";
            } else {
                std::cout << std::setw(9) << dist[i][j];
            }
        }
        std::cout << "\n";
    }
    std::cout << std::string(6 + V * 9, '-') << "\n";
}

// Part (d): Explanation
void printExplanation() {
    std::cout << "\n============================================================\n";
    std::cout << "Explanation\n";
    std::cout << "============================================================\n\n";

    std::cout << "Why Floyd-Warshall works with negative edge weights:\n";
    std::cout << "  Floyd-Warshall is a dynamic programming algorithm that builds\n";
    std::cout << "  shortest paths by progressively allowing more intermediate vertices.\n";
    std::cout << "  The recurrence dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j])\n";
    std::cout << "  only requires that each subpath is optimal, not that weights are\n";
    std::cout << "  positive, so negative edge weights are handled correctly as long\n";
    std::cout << "  as no negative cycle exists on the path.\n\n";

    std::cout << "Why it fails in the presence of negative cycles:\n";
    std::cout << "  If a negative cycle is reachable between two vertices, the shortest\n";
    std::cout << "  path becomes -infinity, since traversing the cycle repeatedly lowers\n";
    std::cout << "  the total cost without bound. Floyd-Warshall assumes paths eventually\n";
    std::cout << "  settle to a finite optimum, so it cannot model this; instead,\n";
    std::cout << "  dist[i][i] turns negative and corrupts other entries in the matrix.\n";
    std::cout << "============================================================\n";
}

int main() {
    int V, E;

    std::cout << "Floyd-Warshall All-Pairs Shortest Path\n";
    std::cout << "---------------------------------------\n";
    std::cout << "Enter number of vertices (V) and edges (E): ";
    std::cin >> V >> E;

    if (V <= 0 || E < 0) {
        std::cerr << "Invalid input.\n";
        return 1;
    }

    // Initialise: no path between distinct vertices, zero from a vertex to itself
    std::vector<std::vector<long long>> dist(V, std::vector<long long>(V, INF));
    for (int i = 0; i < V; i++) dist[i][i] = 0;

    std::cout << "Enter " << E << " edges as (u v w):\n";
    for (int i = 0; i < E; i++) {
        int u, v;
        long long w;
        std::cout << "  Edge " << i + 1 << ": ";
        std::cin >> u >> v >> w;

        if (u < 0 || u >= V || v < 0 || v >= V) {
            std::cerr << "Invalid edge: vertices must be in range [0, " << V - 1 << "].\n";
            return 1;
        }

        // Keep minimum if multiple edges exist between same pair
        dist[u][v] = std::min(dist[u][v], w);
    }

    // Part (a): run Floyd-Warshall
    floydWarshall(V, dist);

    // Part (b): detect negative cycles and propagate -INF to affected pairs
    std::vector<bool> onNegCycle(V, false);
    markNegativeCyclePairs(V, dist, onNegCycle);

    bool hasNegCycle = false;
    for (int i = 0; i < V; i++) if (onNegCycle[i]) { hasNegCycle = true; break; }

    std::cout << "\n--- Negative Cycle Detection ---\n";
    if (hasNegCycle) {
        std::cout << "Negative weight cycle DETECTED.\n";
        std::cout << "Affected vertex pairs are marked -INF in the matrix.\n";
    } else {
        std::cout << "No negative weight cycles found.\n";
    }

    // Part (c): print the final distance matrix
    printDistanceMatrix(V, dist);

    // Part (d): print explanation
    printExplanation();

    return 0;
}

