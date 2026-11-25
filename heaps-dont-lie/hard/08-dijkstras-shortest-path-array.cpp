#include <climits>
#include <iostream>
#define MAX_V 50
#define MAX_E 50
#define MAX 500
using namespace std;

struct Edge {
    int to;
    int weight;
};

struct Graph {
    int V;
    Edge adj[MAX_V][MAX_E];
    int deg[MAX_V];
};

struct HeapNode {
    int dist;
    int vertex;
};

class MinHeap {
private:
    HeapNode items[MAX];
    int size;

    void heapifyUp(int curr);
    void minHeapify(int curr);

public:
    MinHeap();
    bool insert(HeapNode node);
    HeapNode getMin();
    HeapNode extractMin();
    bool isEmpty();
    int getSize();
    void display();
};

MinHeap::MinHeap() {
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

bool MinHeap::insert(HeapNode node) {
    if (size < MAX - 1) {
        items[++size] = node;
        heapifyUp(size);
        return true;
    }
    return false;
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

void MinHeap::display() {
    cout << "size: " << size << endl;
    for (int i = 1; i <= size; i++) {
        cout << "(" << items[i].dist << "," << items[i].vertex << ") ";
    }
    cout << endl;
}

void initGraph(Graph& g, int V) {
    g.V = V;
    for (int i = 0; i < V; i++) {
        g.deg[i] = 0;
    }
}

void addUndirectedEdge(Graph& g, int u, int v, int w) {
    if (g.deg[u] < MAX_E) {
        g.adj[u][g.deg[u]++] = {v, w};
    }
    if (g.deg[v] < MAX_E) {
        g.adj[v][g.deg[v]++] = {u, w};
    }
}

void dijkstra(Graph& g, int src, int dist[]) {
    for (int i = 0; i < g.V; i++) {
        dist[i] = INT_MAX;
    }
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

        for (int i = 0; i < g.deg[u]; i++) {
            int v = g.adj[u][i].to;
            int w = g.adj[u][i].weight;
            if (dist[u] != INT_MAX && dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pq.insert({dist[v], v});
            }
        }
    }
}

void printDistances(int dist[], int V, int src) {
    cout << "Shortest distances from source " << src << ":\n";
    for (int i = 0; i < V; i++) {
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
    Graph g1;
    initGraph(g1, 5);
    addUndirectedEdge(g1, 0, 1, 4);
    addUndirectedEdge(g1, 0, 2, 1);
    addUndirectedEdge(g1, 2, 1, 2);
    addUndirectedEdge(g1, 1, 3, 1);
    addUndirectedEdge(g1, 2, 3, 5);
    addUndirectedEdge(g1, 3, 4, 3);

    int dist1[MAX_V];
    cout << "=== Test Case 1: 5-Vertex Graph ===\n";
    dijkstra(g1, 0, dist1);
    printDistances(dist1, g1.V, 0);

    // Test Case 2: Classic 9-vertex graph
    Graph g2;
    initGraph(g2, 9);
    addUndirectedEdge(g2, 0, 1, 4);
    addUndirectedEdge(g2, 0, 7, 8);
    addUndirectedEdge(g2, 1, 2, 8);
    addUndirectedEdge(g2, 1, 7, 11);
    addUndirectedEdge(g2, 2, 3, 7);
    addUndirectedEdge(g2, 2, 8, 2);
    addUndirectedEdge(g2, 2, 5, 4);
    addUndirectedEdge(g2, 3, 4, 9);
    addUndirectedEdge(g2, 3, 5, 14);
    addUndirectedEdge(g2, 4, 5, 10);
    addUndirectedEdge(g2, 5, 6, 2);
    addUndirectedEdge(g2, 6, 7, 1);
    addUndirectedEdge(g2, 6, 8, 6);
    addUndirectedEdge(g2, 7, 8, 7);

    int dist2[MAX_V];
    cout << "\n=== Test Case 2: 9-Vertex Classic Graph ===\n";
    dijkstra(g2, 0, dist2);
    printDistances(dist2, g2.V, 0);

    // Test Case 3: Disconnected graph
    Graph g3;
    initGraph(g3, 4);
    addUndirectedEdge(g3, 0, 1, 2);
    addUndirectedEdge(g3, 1, 2, 3);

    int dist3[MAX_V];
    cout << "\n=== Test Case 3: Disconnected Graph ===\n";
    dijkstra(g3, 0, dist3);
    printDistances(dist3, g3.V, 0);

    return 0;
}
