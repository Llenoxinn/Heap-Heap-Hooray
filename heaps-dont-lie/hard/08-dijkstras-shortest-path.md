# 08. Dijkstra's Shortest Path (Custom Min-Heap)

- **Difficulty:** Hard
- **Reference:** [GeeksforGeeks — Dijkstra's Shortest Path Algorithm](https://www.geeksforgeeks.org/dsa/dijkstras-shortest-path-algorithm-greedy-algo-7/)

---

## 1. Problem Statement

Given a weighted graph with `V` vertices (`0` to `V - 1`), non-negative edge weights, and a source vertex `src`, find the shortest path distance from `src` to every vertex `0..V - 1`. If a vertex is unreachable from `src`, report its distance as `INF` (`INT_MAX`).

---

## 2. Pattern Recognition & Intuition

### Why Think "Heap" Here?
Dijkstra's algorithm is a **greedy best-first search** over path weights: at every step, it needs to pick the **unfinalized vertex with the smallest tentative distance** from `src`.

- **Naive approach (linear scan):** Maintaining an array `visited[]` and scanning all `V` vertices to find the minimum `dist[u]` takes `O(V)` per extraction, giving `O(V^2 + E)` total time. That degrades quickly on sparse graphs (e.g., `V = 10^5, E = 2 * 10^5`).
- **Heap approach:** A **Min-Heap** keyed by tentative distance lets us query and extract the closest unfinalized vertex in `O(log V)` (or `O(log E)` with lazy deletion), reducing runtime to `O((V + E) log V)`.

### Min-Heap vs. Max-Heap Decision Rule
- We always want the **minimum** tentative distance next, so we use a **Min-Heap** storing `{dist, vertex}` pairs ordered by `dist`.

### Key Invariants & Lazy Deletion
1. **Non-negative weights invariant:** Once a vertex `u` is popped from the Min-Heap with `d == dist[u]`, no future path through other unpopped vertices (which all have distance `>= d`) can ever produce a shorter path to `u` because edge weights are `>= 0`. Thus `dist[u]` is permanently finalized.
2. **Lazy deletion (`if (d > dist[u]) continue;`):** Instead of implementing an indexed `decreaseKey` inside the heap, whenever we relax an edge `u -> v` and find a better distance `dist[v]`, we simply push the new `{dist[v], v}` pair onto the Min-Heap. The older, larger `{oldDist, v}` entry remains in the heap until it surfaces, at which point `d > dist[u]` immediately discards it.

### Edge Cases
- **Disconnected vertices:** Remain at `INT_MAX` (`INF`); guard additions with `dist[u] != INT_MAX` to prevent signed integer overflow.
- **Multiple edges / stale heap entries:** Handled automatically by `if (d > dist[u]) continue;`.
- **Negative edge weights:** Break the greedy invariant (use Bellman-Ford instead).

---

## 3. Step-by-Step Trace (Test Case 1 from `main()`)

**Input Graph (`V = 5`, `src = 0`, undirected):**
Edges: `(0,1,4)`, `(0,2,1)`, `(2,1,2)`, `(1,3,1)`, `(2,3,5)`, `(3,4,3)`

| Step | Action / Popped `{dist, u}` | `dist` Array `[0..4]` | Relaxations & Pushes | Min-Heap Contents (Ordered by `dist`) |
| :--- | :--- | :--- | :--- | :--- |
| Init | Push `{0, 0}` | `[0, INF, INF, INF, INF]` | — | `[{0,0}]` |
| 1 | Extract `{0, 0}` (`0 == dist[0]`) | `[0, 4, 1, INF, INF]` | `0->1`: `dist[1]=4`; `0->2`: `dist[2]=1` | `[{1,2}, {4,1}]` |
| 2 | Extract `{1, 2}` (`1 == dist[2]`) | `[0, 3, 1, 6, INF]` | `2->1`: `1+2=3 < 4` -> `dist[1]=3`; `2->3`: `1+5=6` -> `dist[3]=6` | `[{3,1}, {4,1}, {6,3}]` |
| 3 | Extract `{3, 1}` (`3 == dist[1]`) | `[0, 3, 1, 4, INF]` | `1->3`: `3+1=4 < 6` -> `dist[3]=4` (note `{3,1}` ignores `1->0` and `1->2`) | `[{4,1}, {4,3}, {6,3}]` |
| 4 | Extract `{4, 1}` (`4 > dist[1]=3`) | `[0, 3, 1, 4, INF]` | **Stale entry skipped!** | `[{4,3}, {6,3}]` |
| 5 | Extract `{4, 3}` (`4 == dist[3]`) | `[0, 3, 1, 4, 7]` | `3->4`: `4+3=7` -> `dist[4]=7` | `[{6,3}, {7,4}]` |
| 6 | Extract `{6, 3}` (`6 > dist[3]=4`) | `[0, 3, 1, 4, 7]` | **Stale entry skipped!** | `[{7,4}]` |
| 7 | Extract `{7, 4}` (`7 == dist[4]`) | `[0, 3, 1, 4, 7]` | `4->3` (`7+3 > 4`, no change) | `[]` |

**Final Output:** `0 -> 0`, `1 -> 3`, `2 -> 1`, `3 -> 4`, `4 -> 7`.

---

## 4. Time & Space Complexity

- **Time Complexity:** `O((V + E) log E)` (equivalently `O((V + E) log V)` for simple graphs). Each edge relaxation inserts at most one entry into the Min-Heap (`O(E)` total pushes), and each heap operation takes `O(log E)` = `O(log V)`.
- **Space Complexity:** `O(V + E)` for the adjacency list, `dist` array of size `V`, and at most `E` entries in the Min-Heap.

---

## 5. C++ Implementation

Source file: [08-dijkstras-shortest-path.cpp](08-dijkstras-shortest-path.cpp)

```cpp
#include <climits>
#include <iostream>
#include <vector>
using namespace std;

struct Edge {
    int to;
    int weight;
};

struct HeapNode {
    int dist;
    int vertex;
};

class MinHeap {
private:
    vector<HeapNode> items;
    int size;

    void heapifyUp(int curr);
    void minHeapify(int curr);

public:
    MinHeap();
    void insert(HeapNode node);
    HeapNode getMin();
    HeapNode extractMin();
    bool isEmpty();
    int getSize();
};

MinHeap::MinHeap() {
    items.push_back({0, -1});
    size = 0;
}

void MinHeap::heapifyUp(int curr) {
    HeapNode x = items[curr];
    while (curr / 2 > 0 && x.dist < items[curr / 2].dist) {
        items[curr] = items[curr / 2];
        curr = curr / 2;
    }
    items[curr] = x;
}

void MinHeap::minHeapify(int curr) {
    HeapNode x = items[curr];
    while (curr * 2 <= size) {
        int s = curr * 2;
        if (s + 1 <= size && items[s].dist > items[s + 1].dist) {
            s++;
        }
        if (items[s].dist < x.dist) {
            items[curr] = items[s];
            curr = s;
        } else {
            break;
        }
    }
    items[curr] = x;
}

void MinHeap::insert(HeapNode node) {
    items.push_back(node);
    size++;
    heapifyUp(size);
}

HeapNode MinHeap::getMin() {
    if (size == 0) {
        return {INT_MAX, -1};
    }
    return items[1];
}

HeapNode MinHeap::extractMin() {
    if (size == 0) {
        return {INT_MAX, -1};
    }
    HeapNode minNode = items[1];
    items[1] = items[size];
    items.pop_back();
    size--;
    if (size > 0) {
        minHeapify(1);
    }
    return minNode;
}

bool MinHeap::isEmpty() {
    return size == 0;
}

int MinHeap::getSize() {
    return size;
}

vector<int> dijkstra(int V, vector<vector<Edge>>& adj, int src) {
    vector<int> dist(V, INT_MAX);
    dist[src] = 0;

    MinHeap pq;
    pq.insert({0, src});

    while (!pq.isEmpty()) {
        HeapNode top = pq.extractMin();
        int d = top.dist;
        int u = top.vertex;

        // Skip stale entries left in the heap from earlier relaxations
        if (d > dist[u]) {
            continue;
        }

        for (int i = 0; i < (int)adj[u].size(); i++) {
            int v = adj[u][i].to;
            int w = adj[u][i].weight;
            if (dist[u] != INT_MAX && dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pq.insert({dist[v], v});
            }
        }
    }

    return dist;
}

void addUndirectedEdge(vector<vector<Edge>>& adj, int u, int v, int w) {
    adj[u].push_back({v, w});
    adj[v].push_back({u, w});
}

void printDistances(vector<int>& dist, int src) {
    cout << "Shortest distances from source " << src << ":\n";
    for (int i = 0; i < (int)dist.size(); i++) {
        cout << "  Vertex " << i << " -> ";
        if (dist[i] == INT_MAX) {
            cout << "INF\n";
        } else {
            cout << dist[i] << "\n";
        }
    }
}

int main() {
    // Test Case 1: 5-vertex weighted graph (demonstrates relaxation & stale heap entries)
    int V1 = 5;
    vector<vector<Edge>> adj1(V1);
    addUndirectedEdge(adj1, 0, 1, 4);
    addUndirectedEdge(adj1, 0, 2, 1);
    addUndirectedEdge(adj1, 2, 1, 2);
    addUndirectedEdge(adj1, 1, 3, 1);
    addUndirectedEdge(adj1, 2, 3, 5);
    addUndirectedEdge(adj1, 3, 4, 3);

    cout << "=== Test Case 1: 5-Vertex Graph ===\n";
    vector<int> dist1 = dijkstra(V1, adj1, 0);
    printDistances(dist1, 0);

    // Test Case 2: Classic 9-vertex graph
    int V2 = 9;
    vector<vector<Edge>> adj2(V2);
    addUndirectedEdge(adj2, 0, 1, 4);
    addUndirectedEdge(adj2, 0, 7, 8);
    addUndirectedEdge(adj2, 1, 2, 8);
    addUndirectedEdge(adj2, 1, 7, 11);
    addUndirectedEdge(adj2, 2, 3, 7);
    addUndirectedEdge(adj2, 2, 8, 2);
    addUndirectedEdge(adj2, 2, 5, 4);
    addUndirectedEdge(adj2, 3, 4, 9);
    addUndirectedEdge(adj2, 3, 5, 14);
    addUndirectedEdge(adj2, 4, 5, 10);
    addUndirectedEdge(adj2, 5, 6, 2);
    addUndirectedEdge(adj2, 6, 7, 1);
    addUndirectedEdge(adj2, 6, 8, 6);
    addUndirectedEdge(adj2, 7, 8, 7);

    cout << "\n=== Test Case 2: 9-Vertex Classic Graph ===\n";
    vector<int> dist2 = dijkstra(V2, adj2, 0);
    printDistances(dist2, 0);

    // Test Case 3: Disconnected graph
    int V3 = 4;
    vector<vector<Edge>> adj3(V3);
    addUndirectedEdge(adj3, 0, 1, 2);
    addUndirectedEdge(adj3, 1, 2, 3);

    cout << "\n=== Test Case 3: Disconnected Graph ===\n";
    vector<int> dist3 = dijkstra(V3, adj3, 0);
    printDistances(dist3, 0);

    return 0;
}
```

---

## 6. LeetCode Pattern Takeaways

Whenever you see **"minimum cost/time/effort to reach a target in a weighted graph with non-negative transitions"**, reach for **Min-Heap + Dijkstra**:

- **LC 743 — Network Delay Time:** Direct application of Dijkstra from node `k`.
- **LC 1631 — Path With Minimum Effort:** Dijkstra where edge cost is `abs(height[u] - height[v])` and path cost is `max` instead of `sum`.
- **LC 778 — Swim in Rising Water:** Min-Heap Dijkstra on a 2D grid minimizing the maximum elevation seen along the path.
- **LC 1514 — Path with Maximum Probability:** Same pattern with a **Max-Heap** (multiplying probabilities in `[0, 1]`).
- **LC 787 — Cheapest Flights Within K Stops:** Modified state `{cost, node, stops}` in Dijkstra / BFS.
