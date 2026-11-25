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
