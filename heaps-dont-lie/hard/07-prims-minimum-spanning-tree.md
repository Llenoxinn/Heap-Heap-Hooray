# Prim's Minimum Spanning Tree (MST)

- **Difficulty:** Hard
- **Reference:** [GeeksforGeeks — Prim's Minimum Spanning Tree (MST)](https://www.geeksforgeeks.org/dsa/prims-minimum-spanning-tree-mst-greedy-algo-5/)

---

## 1. Problem Statement

Given a connected, weighted, undirected graph with `V` vertices (`0` to `V - 1`) and undirected edges `(u, v, weight)`, find a **Minimum Spanning Tree (MST)** — a subset of `V - 1` edges connecting all `V` vertices with the minimum possible total edge weight. Output both the total MST weight and the chosen edges.

---

## 2. Pattern Recognition & Intuition

### Why think "Heap" here?
Prim's algorithm grows a tree `T` starting from a single root vertex (`0`):
- At each step, consider all **cut edges** crossing from vertices already inside `T` (`inMST == true`) to vertices outside `T` (`inMST == false`).
- By the **Cut Property** of MSTs, the **lightest edge** crossing this cut is guaranteed to belong to an MST.
- Finding the minimum-weight crossing edge repeatedly is a classic **dynamic minimum extraction** problem $\rightarrow$ **Adjacency List + Min-Heap**.

### Min-Heap vs. Max-Heap Decision Rule
- We want the **minimum-weight** edge connecting the current tree to a new unvisited vertex $\rightarrow$ **Min-Heap** ordered by `weight`.
- (A Max-Heap would compute a *Maximum* Spanning Tree instead.)

### Why this beats naive approaches
- **Array scan Prim's:** Scanning a `key[]` array across all `V` vertices at each of `V` steps takes $O(V^2)$ — fine for dense graphs ($E \approx V^2$), slow for sparse graphs ($E \approx V$).
- **Adjacency List + Min-Heap Prim's:** Extracts the lightest frontier edge in $O(\log E)$ time, yielding $O((V + E) \log V)$ time.

### Key Invariants & Lazy Stale-Entry Skipping
Notice how clean Prim's becomes when we skip `decreaseKey` and use **lazy skipping**:
1. Whenever a vertex `v` is added to the MST (`inMST[v] = true`), push `{edge.weight, edge.to, v}` for every neighbor `edge.to` not yet in the MST.
2. Because an outside vertex `u` might be reachable from multiple tree vertices, multiple entries for `u` with different weights can live in the heap simultaneously.
3. The **first** time `u` is popped from `MinHeap`, it is guaranteed to arrive via the **cheapest** crossing edge to `u`. We mark `inMST[u] = true` and ignore any heavier stale entries for `u` popped later (`if (inMST[curr.vertex]) continue;`).

### Edge Cases
- **Disconnected graph:** If `visitedCount < numVertices` when the heap empties, no spanning tree exists (`primMST` returns `-1`).
- **Parallel edges / self-loops:** Handled naturally because `if (inMST[curr.vertex]) continue;` discards redundant or self-referential edges.
- **Negative edge weights:** Prim's MST works seamlessly with negative weights (unlike Dijkstra's shortest paths!).

---

## 3. Step-by-Step Trace

Using **Test 1** from `main()` (`V = 5`):
Edges: `(0,1,2), (0,3,6), (1,2,3), (1,3,8), (1,4,5), (2,4,7), (3,4,9)`

Start at vertex `0`: `heap = [(w=0, v=0, p=-1)]`.

| Step | Pop `(w, v, p)` | `inMST[v]` before? | Action | Neighbors pushed (`!inMST`) | `mstEdges` & `totalWeight` |
| :--- | :--- | :--- | :--- | :--- | :--- |
| 1 | `(0, 0, -1)` | `false` | Add `0` to MST | `(2, 1, 0)`, `(6, 3, 0)` | `weight = 0` |
| 2 | `(2, 1, 0)` | `false` | Add `1`, edge `0-1 (2)` | `(3, 2, 1)`, `(8, 3, 1)`, `(5, 4, 1)` | `[(0,1,2)]`, `weight = 2` |
| 3 | `(3, 2, 1)` | `false` | Add `2`, edge `1-2 (3)` | `(7, 4, 2)` | `[(0,1,2), (1,2,3)]`, `weight = 5` |
| 4 | `(5, 4, 1)` | `false` | Add `4`, edge `1-4 (5)` | `(9, 3, 4)` | `[(0,1,2), (1,2,3), (1,4,5)]`, `weight = 10` |
| 5 | `(6, 3, 0)` | `false` | Add `3`, edge `0-3 (6)` | None (`!inMST` empty) | `[(0,1,2), (1,2,3), (1,4,5), (0,3,6)]`, `weight = 16` |
| 6–8 | `(7,4,2)`, `(8,3,1)`, `(9,3,4)` | `true` | Skipped (stale) | — | Done (`visitedCount = 5`) |

Final MST Weight: **`16`** with edges `0-1 (2)`, `1-2 (3)`, `1-4 (5)`, `0-3 (6)`.

---

## 4. Time & Space Complexity

Let $V$ be the number of vertices and $E$ be the number of undirected edges.
- **Time Complexity:** $O((V + E) \log E) = O((V + E) \log V)$
  - Each edge is pushed into the `MinHeap` at most twice (once from each endpoint), so the heap holds at most $2E$ items and `log(2E) = O(log V)`.
  - Every insertion and extraction takes $O(\log V)$.
- **Space Complexity:** $O(V + E)$
  - Adjacency list takes $O(V + E)$, `inMST` takes $O(V)$, and the `MinHeap` stores at most $O(E)$ entries.

---

## 5. C++ Implementation

See [07-prims-minimum-spanning-tree.cpp](07-prims-minimum-spanning-tree.cpp).

```cpp
#include <iostream>
#include <vector>
using namespace std;

struct AdjEdge {
    int to;
    int weight;
};

struct MSTEdge {
    int u;
    int v;
    int weight;
};

struct HeapNode {
    int weight;
    int vertex;
    int parent;
};

class MinHeap {
private:
    vector<HeapNode> items;
    int size;

    void heapifyUp(int curr) {
        HeapNode x = items[curr];
        while (curr / 2 > 0 && x.weight < items[curr / 2].weight) {
            items[curr] = items[curr / 2];
            curr = curr / 2;
        }
        items[curr] = x;
    }

    void minHeapify(int curr) {
        HeapNode x = items[curr];
        while (curr * 2 <= size) {
            int smallest = curr * 2;
            if (smallest + 1 <= size && items[smallest + 1].weight < items[smallest].weight) {
                smallest++;
            }
            if (items[smallest].weight < x.weight) {
                items[curr] = items[smallest];
                curr = smallest;
            } else {
                break;
            }
        }
        items[curr] = x;
    }

public:
    MinHeap() {
        items.push_back({0, 0, -1});
        size = 0;
    }

    bool insert(HeapNode node) {
        if (size + 1 < (int)items.size()) {
            items[++size] = node;
        } else {
            items.push_back(node);
            size++;
        }
        heapifyUp(size);
        return true;
    }

    HeapNode getMin() {
        if (size == 0) {
            return {0, -1, -1};
        }
        return items[1];
    }

    HeapNode extractMin() {
        if (size == 0) {
            return {0, -1, -1};
        }
        HeapNode top = items[1];
        items[1] = items[size];
        size--;
        if (size > 0) {
            minHeapify(1);
        }
        return top;
    }

    bool isEmpty() {
        return size == 0;
    }

    int getSize() {
        return size;
    }
};

int primMST(int numVertices, vector<vector<AdjEdge>>& adj, vector<MSTEdge>& mstEdges) {
    vector<bool> inMST(numVertices, false);
    MinHeap heap;
    heap.insert({0, 0, -1});

    int totalWeight = 0;
    int visitedCount = 0;
    mstEdges.clear();

    while (!heap.isEmpty()) {
        HeapNode curr = heap.extractMin();
        if (inMST[curr.vertex]) {
            continue; // Skip stale entry for an already-included vertex
        }

        inMST[curr.vertex] = true;
        visitedCount++;
        totalWeight += curr.weight;

        if (curr.parent != -1) {
            mstEdges.push_back({curr.parent, curr.vertex, curr.weight});
        }

        for (int i = 0; i < (int)adj[curr.vertex].size(); i++) {
            AdjEdge edge = adj[curr.vertex][i];
            if (!inMST[edge.to]) {
                heap.insert({edge.weight, edge.to, curr.vertex});
            }
        }
    }

    if (visitedCount != numVertices) {
        return -1; // Graph is disconnected
    }
    return totalWeight;
}

void addUndirectedEdge(vector<vector<AdjEdge>>& adj, int u, int v, int w) {
    adj[u].push_back({v, w});
    adj[v].push_back({u, w});
}

void runPrimTest(int V, vector<MSTEdge>& edges, string label) {
    vector<vector<AdjEdge>> adj(V);
    for (int i = 0; i < (int)edges.size(); i++) {
        addUndirectedEdge(adj, edges[i].u, edges[i].v, edges[i].weight);
    }

    vector<MSTEdge> mstEdges;
    int totalWeight = primMST(V, adj, mstEdges);

    cout << label << ":\n";
    cout << "  Total MST weight = " << totalWeight << "\n";
    cout << "  Edges in MST:\n";
    for (int i = 0; i < (int)mstEdges.size(); i++) {
        cout << "    " << mstEdges[i].u << " - " << mstEdges[i].v
             << " (weight " << mstEdges[i].weight << ")\n";
    }
}

int main() {
    vector<MSTEdge> graph1 = {
        {0, 1, 2},
        {0, 3, 6},
        {1, 2, 3},
        {1, 3, 8},
        {1, 4, 5},
        {2, 4, 7},
        {3, 4, 9}
    };
    runPrimTest(5, graph1, "Test 1 (Classic 5-vertex graph)");

    vector<MSTEdge> graph2 = {
        {0, 1, 4},
        {0, 2, 4},
        {1, 2, 2},
        {1, 3, 6},
        {2, 3, 5}
    };
    runPrimTest(4, graph2, "Test 2 (4-vertex diamond graph)");

    return 0;
}
```

---

## 6. LeetCode Pattern Takeaways

The **Graph + Min-Heap with Lazy Stale Skipping** pattern powers:
- **LC 1584 — Min Cost to Connect All Points** (Prim's MST on complete coordinate graph)
- **LC 1135 — Connecting Cities With Minimum Cost** (Prim's or Kruskal's MST)
- **LC 1489 — Find Critical and Pseudo-Critical Edges in Minimum Spanning Tree**
- **LC 743 — Network Delay Time** (Dijkstra's shortest path — identical lazy-skipping Min-Heap structure, just accumulating `dist[u] + w` instead of `w`)
- **LC 778 — Swim in Rising Water** (minimax path via Prim/Dijkstra Min-Heap)
