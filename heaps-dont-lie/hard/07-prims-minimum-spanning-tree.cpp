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
