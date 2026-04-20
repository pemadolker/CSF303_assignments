<!-- # Assignment 1 — DSA (STL, Bellman-Ford, Floyd-Warshall)

---

## Overview

This assignment covers three main topics: STL containers in C++, the Bellman-Ford shortest path algorithm, and the Floyd-Warshall all-pairs shortest path algorithm. Each question is in its own `.cpp` file. All programs take user input at runtime.

Files:
- `q1_stl.cpp` — STL containers (vector, deque, array)
- `q2_bellman_ford.cpp` — Bellman-Ford + negative cycle detection
- `q3_floyd_warshall.cpp` — Floyd-Warshall + negative cycle detection

Compile with: `g++ -std=c++17 -o <output> <file>.cpp`

---

## Question 1 — STL Containers

### Part A — vector, reverse output

I used a `std::vector<int>` to store N integers entered by the user, then printed them in reverse. Instead of doing it manually with a loop counting down from N-1, I used reverse iterators (`rbegin()` and `rend()`) which is the proper STL way to do it. The size N is read at runtime so it works for any input up to 10^5.

```cpp
for (auto it = nums.rbegin(); it != nums.rend(); ++it) {
    std::cout << *it << " ";
}
```

Sample:
```
Enter N: 5
Enter 5 integers: 3 1 4 1 5
Reversed: 5 1 4 1 3
```

---

### Part B — deque, operation sequence

For the deque part I made it interactive — the user types operations one at a time (PUSH_FRONT, PUSH_BACK, POP_FRONT) and types DONE when finished. Each operation prints what it did so you can follow along. I also added a check so POP_FRONT on an empty deque does not crash.

The reason deque makes sense here is that push_front is O(1) on a deque but O(N) on a vector, so for double-ended operations deque is the right choice.

```
PUSH_BACK 10     ->  push_back(10) done
PUSH_FRONT 5     ->  push_front(5) done
POP_FRONT        ->  pop_front() removed: 5
PUSH_BACK 30     ->  push_back(30) done
DONE

Final deque contents: 10 30
Size: 2
```

---

### Part C — std::array, sum with STL

`std::array` is different from vector because its size has to be known at compile time — you cannot resize it. I set the size to 5 as a compile-time constant and read the values from the user. The sum is computed using `std::accumulate` from `<numeric>` rather than a manual loop, and I also used `std::minmax_element` to get the min and max in one pass.

```cpp
const int SIZE = 5;
std::array<int, SIZE> arr;
int total = std::accumulate(arr.begin(), arr.end(), 0);
```

Sample:
```
Enter 5 integers: 4 7 2 9 1
Array elements: 4 7 2 9 1
Sum (via std::accumulate): 23
Min: 1, Max: 9
```

---

## Question 2 — Bellman-Ford

### How the algorithm works

Bellman-Ford finds shortest paths from a single source vertex to all others. The idea is edge relaxation — you go through every edge and check if you can improve the known distance to the destination vertex. You repeat this V-1 times because the longest possible shortest path (with no negative cycles) visits at most V-1 edges.

```
for V-1 iterations:
    for each edge (u, v, w):
        if dist[u] + w < dist[v]:
            dist[v] = dist[u] + w
            parent[v] = u
```

I also added an early exit — if a full pass over all edges makes no update, the algorithm has already converged and there is no point doing more iterations.

### Path reconstruction

I kept a parent[] array that records how we arrived at each vertex during relaxation. Once the algorithm finishes, tracing back through the parent chain from any target vertex gives the full path. I used a stack to reverse it so it prints in the right order (source to destination).

### Negative cycle detection

After V-1 passes, I do one more relaxation pass. If any distance still gets updated in this extra pass, it means there is a negative cycle — the algorithm should have fully converged by now if there was not one.

I also ran a BFS after finding the directly affected vertices to propagate the -INF flag forward. A vertex being on a negative cycle makes every vertex reachable from it also have distance -INF, not just the ones directly on the cycle itself.

### Input / Output

Input format:
```
5 8        <- V and E
0 1 -1     <- edges (u v w), one per line
0 2 4
1 2 3
1 3 2
1 4 2
3 2 5
3 1 1
4 3 -3
0          <- source vertex
```

Output:
```
Shortest distances from source vertex 0:
  Vertex 1: -1
  Vertex 2: 2
  Vertex 3: -2
  Vertex 4: 1

Shortest paths from source vertex 0:
  To vertex 1: 0 -> 1  (distance: -1)
  To vertex 2: 0 -> 1 -> 2  (distance: 2)
  To vertex 3: 0 -> 1 -> 4 -> 3  (distance: -2)
  To vertex 4: 0 -> 1 -> 4  (distance: 1)

No negative weight cycles detected.
```

Vertices that cannot be reached show UNREACHABLE, and vertices caught in or downstream of a negative cycle show -INF.

Complexity: O(V x E) time, O(V) space

---

## Question 3 — Floyd-Warshall

### How the algorithm works

While Bellman-Ford works from one source, Floyd-Warshall computes shortest paths between every pair of vertices at once. It does this with three nested loops and a DP recurrence:

```
for each intermediate vertex k:
    for each pair (i, j):
        dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j])
```

The idea is that for each k, you are asking: is the path from i to j shorter if we are allowed to go through k? After all V values of k have been tried, every pair has its optimal answer.

The matrix is initialised with direct edge weights (INF where no edge exists, 0 on the diagonal).

Complexity: O(V^3) time, O(V^2) space

### Negative cycle detection

After running the algorithm, I check if dist[i][i] < 0 for any vertex. If a vertex has a negative path back to itself, it is on a negative cycle.

I then mark all pairs (i, j) as -INF if there is any negative-cycle vertex k that lies on the path from i to j — meaning i can reach k and k can reach j. Without this the matrix would just show garbage numbers for those pairs.

### Distance matrix output

No negative cycle:
```
              0        1        2        3
------------------------------------------
   0 |        0        3        5        0
   1 |        8        0        2       -3
   2 |      INF      INF        0       -5
   3 |      INF      INF      INF        0
```

With a negative cycle:
```
              0        1        2
---------------------------------
   0 |     -INF     -INF     -INF
   1 |     -INF     -INF     -INF
   2 |     -INF     -INF     -INF
```

### Part D — Written answers

**Why Floyd-Warshall works with negative edge weights:**
The algorithm does not care whether individual weights are positive or negative. All it does at each step is compare two path lengths and keep the smaller one. As long as the shortest paths are finite and well-defined (no negative cycles), the DP converges to the correct answer regardless of the sign of the weights.

**Why it fails with negative cycles:**
If there is a negative cycle somewhere, the shortest path between vertices that can reach it becomes -infinity — you just keep going around the loop and the total cost keeps going down. Floyd-Warshall has no mechanism for this; it runs its fixed iterations and leaves wrong values in the matrix. The sign of dist[i][i] going negative is how you detect it happened.

--- -->

