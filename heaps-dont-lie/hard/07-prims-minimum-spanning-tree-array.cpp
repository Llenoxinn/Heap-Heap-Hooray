#include <iostream>
#define MAX 200
#define MAX_V 50
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
    HeapNode items[MAX];
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
        size = 0;
    }

    bool insert(HeapNode node) {
        if (size < MAX - 1) {
            items[++size] = node;
            heapifyUp(size);
            return true;
        }
        return false;
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

    bool remove() {
        if (size == 0) {
            return false;
        }
        extractMin();
        return true;
    }

    bool isEmpty() {
        return size == 0;
    }

    int getSize() {
        return size;
    }

    void display() {
        cout << "size: " << size << endl;
        for (int i = 1; i <= size; i++) {
            cout << "(v=" << items[i].vertex << ", w=" << items[i].weight << ") ";
        }
        cout << endl;
    }
};

int primMST(int numVertices, AdjEdge adj[MAX_V][MAX_V], int adjCount[],
            MSTEdge mstEdges[], int& mstEdgeCount) {
    bool inMST[MAX_V];
    for (int i = 0; i < numVertices; i++) {
        inMST[i] = false;
    }

    MinHeap heap;
    heap.insert({0, 0, -1});

    int totalWeight = 0;
    int visitedCount = 0;
    mstEdgeCount = 0;

    while (!heap.isEmpty()) {
        HeapNode curr = heap.extractMin();
        if (inMST[curr.vertex]) {
            continue; // Skip stale entry for an already-included vertex
        }

        inMST[curr.vertex] = true;
        visitedCount++;
        totalWeight += curr.weight;

        if (curr.parent != -1) {
            mstEdges[mstEdgeCount++] = {curr.parent, curr.vertex, curr.weight};
        }

        for (int i = 0; i < adjCount[curr.vertex]; i++) {
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

void addUndirectedEdge(AdjEdge adj[MAX_V][MAX_V], int adjCount[], int u, int v, int w) {
    adj[u][adjCount[u]++] = {v, w};
    adj[v][adjCount[v]++] = {u, w};
}

void runPrimTest(int V, MSTEdge edges[], int edgeCount) {
    AdjEdge adj[MAX_V][MAX_V];
    int adjCount[MAX_V];
    for (int i = 0; i < V; i++) {
        adjCount[i] = 0;
    }

    for (int i = 0; i < edgeCount; i++) {
        addUndirectedEdge(adj, adjCount, edges[i].u, edges[i].v, edges[i].weight);
    }

    MSTEdge mstEdges[MAX_V];
    int mstEdgeCount = 0;
    int totalWeight = primMST(V, adj, adjCount, mstEdges, mstEdgeCount);

    cout << "  Total MST weight = " << totalWeight << "\n";
    cout << "  Edges in MST:\n";
    for (int i = 0; i < mstEdgeCount; i++) {
        cout << "    " << mstEdges[i].u << " - " << mstEdges[i].v
             << " (weight " << mstEdges[i].weight << ")\n";
    }
}

int main() {
    MSTEdge graph1[7] = {
        {0, 1, 2},
        {0, 3, 6},
        {1, 2, 3},
        {1, 3, 8},
        {1, 4, 5},
        {2, 4, 7},
        {3, 4, 9}
    };
    cout << "Test 1 (Classic 5-vertex graph):\n";
    runPrimTest(5, graph1, 7);

    MSTEdge graph2[5] = {
        {0, 1, 4},
        {0, 2, 4},
        {1, 2, 2},
        {1, 3, 6},
        {2, 3, 5}
    };
    cout << "Test 2 (4-vertex diamond graph):\n";
    runPrimTest(4, graph2, 5);

    return 0;
}
