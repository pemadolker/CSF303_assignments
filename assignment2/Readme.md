
# Question 2: Johnson's Algorithm

## (a): Explain why Johnson's Algorithm is More Efficient than Floyd-Warshall for Sparse Graphs


The runtime of **Floyd-Warshall**  is insensitive to the number of edges, and is **O(V³)**. It applies dynamic programming to all pairs of vertices which implies that even when the graph is very sparse, it does the same work - it is totally independent of the density of the graph.

**Johnson's Algorithm** runs in **O(V² log V + VE)**. It is efficient because it only needs to run the algorithm of Dijkstra once per vertex (V times), and once with a binary heap (Dijkstra) runs in **O((V + E) log V )** each time, so runs in **O(V(V + E) log V)** , which then simplifies to  **O(V² log V + VE)**.

For a **sparse graph**, E ≪ V², so:

| Algorithm | Time Complexity | Sparse Graph (E ≈ V) |
|---|---|---|
| Floyd-Warshall | O(V³) | O(V³) |
| Johnson's | O(V² log V + VE) | O(V² log V) |

Johnson is much faster so on sparse graphs. For example, with V = 1000 and E = 2000:
- Floyd-Warshall: ~1,000,000,000 operations
- Johnson's: ~1,000,000 × 11 ≈ 11,000,000 operations

**The key insight:** Floyd-Warshall's cubic complexity is fixed. Johnson takes advantage of sparseness of the graph - the fewer the number of edges, the quicker Dijkstra is.

---

## (b): Describe the purpose of edge reweighting and how Bellman-Ford is used in this context.

**The Problem with Dijkstra on Negative Weights:**
Dijkstra's algorithm does not work when the weights of the edges are negative since its greedy assumption, that the shortest path is determined when a node has been settled, fails when a subsequent negative edge can create a shorter path. To make Johnson algorithm work, negative edges should be removed without altering shortest paths, that is, Dijkstra should be used.

**Edge Reweighting — Solution:**
In the algorithm of Johnson, the reweighting function is proposed by making use of a potential value of each vertex in the graph, which is denoted by `h(v)`  and changing each edge weight as:

```
ŵ(u, v) = w(u, v) + h(u) - h(v)
```

This transformation has two critical properties:
1. **All reweighted edges become non-negative** — safe for Dijkstra
2. **Shortest paths are preserved** —in case a path P is shortest with original weights, it is also shortest with reweighted values, since on any route between s and t, the h() terms telescope and cancel out:

```
ŵ(path) = w(path) + h(s) - h(t)
```

The offset `h(s) - h(t)` is the same for ALL paths from s to t, so the relative ordering of paths is unchanged.

**Role of Bellman-Ford:**
To compute the potential values `h(v)`, Johnson's algorithm:
1. Introduces a new source vertex q which has an edge to each other vertex of weight 0.
2. Find the shortest path of distance `h(v)` of q to each vertex v by running BellmanFord with input q.
3. These distances are taken to be the potentials that are to be reweighted.

Bellman-Ford is chosen here specifically because:
- It **handles negative edge weights**, and this is precisely what we are attempting to avoid.
- It **detects negative cycles** — a negative cycle is detected by Bellman-Ford, and this algorithm is correct in halting and reporting no solution (negative cycles make shortest paths undefined)
- It only runs **once** — O(VE) — which is acceptable as a preprocessing step

**Summary of Johnson's Full Pipeline:**
```
1. Add a dummy vertex q, which has 0-weight edges to all vertices.
2. Bellman-Ford(q) to get potentials h(v) [O(VE)].
3. Reweight all edges using ŵ(u,v) = w(u,v) + h(u) - h(v)
4. Run Dijkstra on all vertices of reweighted graph [O(V(V+E)logV)]
5. Adjust results back: d(u,v) = d̂(u,v) - h(u) + h(v)
```


# Question 3: Arbitrage Detection in Currency Exchange

## (a): Model the currency exchange problem as a weighted directed graph.

The currency exchange in terms of currency is modelled as:

- **Vertices (V):** Vertices are currencies (e.g., USD, EUR, GBP, JPY).
- **Directed Edges (E):** An edge between vertex u and vertex v is a directed edge, indicating the possibility of exchanging currency u with currency v.
- **Edge Weight:** weight of edge (u → v) is the exchange rate r(u,v) one unit of currency u exchanges r(u,v) units of currency v.

**Example:**
```
USD → EUR : rate = 0.91
EUR → GBP : rate = 0.86
GBP → USD : rate = 1.27
```

An **arbitrage opportunity** exists when starting with 1 unit of some currency, you can traverse a cycle through other currencies and return with **more than 1 unit** — i.e., when:

```
r(u→v) × r(v→w) × r(w→u) > 1
```

The problem then becomes: **detect a positive-product cycle** in this directed weighted graph.


## (b) How logarithmic transformation of exchange rates converts the problem into a shortest path or cycle detection problem.

Identifying a positive-product cycle is mathematically clumsy and requires special graph algorithms to detect it, such as the sums and products of the graph instead of the products. The gap is bridged by the use of logarithmic transformation which transforms multiplication into addition.

**The Transformation:**
Replace each edge weight r(u, v) with:

```
w(u, v) = -log(r(u, v))
```

**Why this works:**

An arbitrage cycle satisfies:
```
r₁ × r₂ × r₃ × ... × rₖ > 1
```

Taking log of both sides:
```
log(r₁) + log(r₂) + ... + log(rₖ) > 0
```

Negating (to convert to our edge weights):
```
-log(r₁) + (-log(r₂)) + ... + (-log(rₖ)) < 0
```

Thus a positive-product cycle in the original graph is turned into a negative-weight cycle in the transformed graph. It is an extensively studied issue that can be identified directly using the usual shortest-path algorithms.

**The transformation converts:**
```
Arbitrage detection  →  Negative cycle detection
Positive product cycle  →  Negative weight cycle
Multiplication  →  Addition ( through logarithm)
```

## (c):Identify the algorithm used to detect arbitrage opportunities and justify its use.

**Algorithm: Bellman-Ford**

With the logarithmic transformation, the issue is now to identify a negative-weight cycle in a directed graph. The right algorithm to do this is Bellman-Ford.

**How Bellman-Ford detects negative cycles:**

Bellman-Ford relaxes all edges **(V - 1)** times. In case any edge (u, v) remains relaxable, after these relaxations, that is, can satisfy:

```
dist[v] > dist[u] + w(u, v)
```

— there is a negative cycle in the graph, which is associated with an arbitrage opportunity.

**Reason for choosing Bellman-Ford:**

| Property | Why it matters here |
|---|---|
| Handles negative edge weights | Our log-transformed weights can be negative |
| Detects negative cycles explicitly | This is exactly what arbitrage maps to |
| Works on directed graphs |  Exchange of currencies is directed. |
| O(VE) complexity | Acceptable for typical currency graph sizes |

**Dijkstra cannot be used here** since it does not support negative edge weights, and does not have any way to identify negative cycles.

**Complete Arbitrage Detection Pipeline:**
```
1. Build graph: vertices = currencies, edges = exchange rates
2. Transform weights: w(u,v) = -log(rate(u,v))
3. Run Bellman-Ford (V-1) times.
4. On the Vth iteration, check if any edge can still be relaxed
5. If yes → negative cycle exists → ARBITRAGE OPPORTUNITY FOUND
```


# Question 4: Edmonds' Algorithm

## Q. State the problem solved by Edmonds’ Algorithm in precise terms.

**Edmonds' Algorithm** (also known as **Chu-Liu/Edmonds' Algorithm**) solves the **Minimum Spanning Arborescence (MSA)** problem, defined precisely as follows:

Given a directed graph G = (V, E, w) and a specified root node r V, find a minimum weight spanning arborescence rooted at r - a directed subgraph T of G such that:
> 1. T has precisely V -1 edges.
> 2. At each vertex v other than r, the directed path rvi is defined by a unique directed path in T (i.e. there exists a directed path rvi) (i.e. r can reach all other vertices).
> 3. The overall weight of the edges of T is minimized.


**Key distinctions from related problems:**

| Problem | Graph Type | Goal |
|---|---|---|
| Minimum Spanning Tree (Prim's/Kruskal's) | **Undirected** | Minimum weight spanning tree |
| Minimum Spanning Arborescence | **Directed** | Minimum weight directed spanning tree rooted at r |

This is inherently more difficult due to its directed nature - you can not use Prim's or Kruskal's since edge direction does not imply reachability is symmetric.

**What makes it non-trivial:**
In an undirected MST, any edge is allowed to be traversed in both directions. In a directed graph, the optimum incoming graph-edge of every vertex is chosen with care - greedily selecting the minimum incoming graph-edge per vertex usually results in a cycle, and this problem is solved by Edmonds algorithm through a **cycle contraction and re-expansion procedure**.

**Algorithm Overview:**
```
1. For each vertex v ≠ r, select the minimum weight incoming edge
2. If no cycle forms → this is the MSA
3. If a cycle C forms → contract C into a single supernode,
   assign edge weights to account for the cycle,
   recurse on the contracted graph,
   then expand the cycle back, removing exactly one edge to break it
```
**Time Complexity: O(VE)**, with the original formulation, can be improved to O(E log V) when using priority queues (refinement by Tarjan).

**Practical applications:** Network broadcast trees, dependency resolution, optimizing directed communication networks, and finding optimal root-to-all routing in directed graphs.


# Question 5: String Matching Algorithms

## (a): For the Knuth-Morris-Pratt (KMP) algorithm, compute the Longest Prefix Suffix (LPS) array for the pattern:  "ABABCABAB"

### What is the LPS Array?

The LPS (Longest Proper Prefix which is also a Suffix) array stores, for each position `i` in the pattern, the **length of the longest proper prefix of the substring `pattern[0..i]` that is also a suffix** of that substring. "Proper" means the prefix/suffix cannot be the entire string itself.

This allows KMP to **skip redundant comparisons** on mismatch — instead of restarting from the beginning of the pattern, it jumps to the position indicated by LPS.


### Step-by-Step LPS Computation

**Pattern:** `A B A B C A B A B`  
**Index:**  `0 1 2 3 4 5 6 7 8`

We maintain two pointers:
- `len` → length of the previous longest prefix suffix (starts at 0)
- `i` → current position being computed (starts at 1)
- `LPS[0]` is always **0** by definition



**i = 1, char = 'B'**
```
Compare pattern[1]='B' vs pattern[0]='A' → MISMATCH
len = 0, so LPS[1] = 0
```

**i = 2, char = 'A'**
```
Compare pattern[2]='A' vs pattern[0]='A' → MATCH
len becomes 1, LPS[2] = 1
```

**i = 3, char = 'B'**
```
Compare pattern[3]='B' vs pattern[1]='B' → MATCH
len becomes 2, LPS[3] = 2
```

**i = 4, char = 'C'**
```
Compare pattern[4]='C' vs pattern[2]='A' → MISMATCH
len > 0, so fall back: len = LPS[len-1] = LPS[1] = 0
Compare pattern[4]='C' vs pattern[0]='A' → MISMATCH
len = 0, so LPS[4] = 0
```

**i = 5, char = 'A'**
```
Compare pattern[5]='A' vs pattern[0]='A' → MATCH
len becomes 1, LPS[5] = 1
```

**i = 6, char = 'B'**
```
Compare pattern[6]='B' vs pattern[1]='B' → MATCH
len becomes 2, LPS[6] = 2
```

**i = 7, char = 'A'**
```
Compare pattern[7]='A' vs pattern[2]='A' → MATCH
len becomes 3, LPS[7] = 3
```

**i = 8, char = 'B'**
```
Compare pattern[8]='B' vs pattern[3]='B' → MATCH
len becomes 4, LPS[8] = 4
```


### Final LPS Array

```
Pattern:  A  B  A  B  C  A  B  A  B
Index:    0  1  2  3  4  5  6  7  8
LPS:      0  0  1  2  0  1  2  3  4
```

### Verification of Key Entries

| Index | Substring | Longest Prefix = Suffix | LPS Value |
|---|---|---|---|
| 0 | `A` | none (single char) | 0 |
| 2 | `ABA` | `A` | 1 |
| 3 | `ABAB` | `AB` | 2 |
| 4 | `ABABC` | none | 0 |
| 7 | `ABABCABA` | `ABA` | 3 |
| 8 | `ABABCABAB` | `ABAB` | **4** |

LPS[8] = 4 indicates the longest prefix and suffix (ABAB) of the pattern AABBCABAB are both prefixes and suffixes of the pattern and thus the KMP does not need to rescan the letters that have already been recognized as matching the pattern.



## (b): For the Rabin-Karp Algorithm: 
- Explain how hash collisions are handled

- State its average-case and worst-case time complexities

### The way that Hash Collisions are dealt with

RabinKarp operates by calculating a rolling hash of the pattern, and the hash of each window of the text of that length, and comparing hashes. A match of hash values indicates a possible pattern match. Two dissimilar strings may however have the same hash - this is a hash collision - and Rabin-Karp treats them as follows:

**Step 1 — Hash comparison (filter step):**
```
If hash(text window) ≠ hash(pattern) → definitely NOT a match, skip
If hash(text window) = hash(pattern) → POSSIBLE match, verify
```

**Step 2 -character by character verification: explicit verification:**
Each time a hash match is observed, Rabin-Karp does a complete string compare between the pattern and the text window character by character. This guarantees correctness:

- When the strings are equal, then this indicates a pattern found, hence a true positive.
- when they are not equal, then, spurious hit / false positive due to collision, discard and proceed.

**Collision reduction strategies:**
- Choose a large prime number as the modulus in the hash function - decreases the likelihood of collision a lot.
- Double hashing (two independent hash functions) - a collision in both functions is astronomically improbable.
- The standard hash formula used is:


```
hash = (d * hash + char) % q
```

Where `d` = number of characters in alphabet (typically 256), `q` = a large prime (e.g., 101 or 1000000007)

The rolling hash updates in **O(1)** per window shift:
```
hash(next window) = (d * (hash(current) - text[i] * h) + text[i+m]) % q
```
Where `h = d^(m-1) % q` and m = pattern length.


### Time Complexities

| Case | Complexity | When it occurs |
|---|---|---|
| **Best Case** | O(N + M) | No or very few hash collisions |
| **Average Case** | O(N + M) | Expected with a good hash function |
| **Worst Case** | O(NM) | Every window causes a spurious hash collision |

**Explanation:**

- **N** = length of text, **M** = length of pattern
- Calculation of first hash = O(M)
- Sliding the rolling hash across N - M + 1 windows = O(N)
- **Average case O(N + M): The number of spurious hits of a good hash function is very small. Checks of windows are O(1) and only true matches result in O(M) checking - providing O(N + M).
- **Worst case O(NM):** This is the worst case when each and every window has a hash collision which must be checked in O(M). e.g. finding pattern AAAA in text AAAA... where all windows hash to the same value. Each window provokes a complete O(M) comparison, with O(NM) overall.
|human| Preprocessing Preprocessing | Search Worst Case
|---|---|---|---|
| KMP | O(M) | O(N) | O(N + M) always |
| Rabin-Karp | O(M) | O(N) average | O(NM) worst case |

KMP should ensure O(N + M) is always the case with the use of its LPS structure. The advantage of Rabin-Karp is in the ability to match multiple patterns simultaneously, i.e. in the case of k patterns, Rabin-Karp will run in O(N + kM) average time, not in KMP k times, and is thus highly useful in plagiarism detection, DNA sequencing and file fingerprinting.

