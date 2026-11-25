#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

struct Point {
    int x;
    int y;
    long long distSq;
};

class MaxHeap {
private:
    vector<Point> items;
    int size;

    void heapifyUp(int curr) {
        Point p = items[curr];
        while (curr / 2 > 0 && p.distSq > items[curr / 2].distSq) {
            items[curr] = items[curr / 2];
            curr = curr / 2;
        }
        items[curr] = p;
    }

    void maxHeapify(int curr) {
        Point p = items[curr];
        while (curr * 2 <= size) {
            int child = curr * 2;
            if (child + 1 <= size && items[child + 1].distSq > items[child].distSq) {
                child++;
            }
            if (items[child].distSq > p.distSq) {
                items[curr] = items[child];
                curr = child;
            } else {
                break;
            }
        }
        items[curr] = p;
    }

public:
    MaxHeap() {
        items.push_back({0, 0, 0});
        size = 0;
    }

    void insert(Point p) {
        size++;
        if (size < (int)items.size()) {
            items[size] = p;
        } else {
            items.push_back(p);
        }
        heapifyUp(size);
    }

    Point getMax() {
        if (size == 0) return {0, 0, 0};
        return items[1];
    }

    Point extractMax() {
        if (size == 0) return {0, 0, 0};
        Point topPoint = items[1];
        items[1] = items[size];
        size--;
        if (size > 0) {
            maxHeapify(1);
        }
        return topPoint;
    }

    bool isEmpty() {
        return size == 0;
    }

    int getSize() {
        return size;
    }
};

vector<Point> kClosestPoints(vector<Point>& points, int k) {
    vector<Point> result;
    if (k <= 0 || points.empty()) return result;

    MaxHeap maxHeap;
    for (int i = 0; i < (int)points.size(); i++) {
        Point p = points[i];
        p.distSq = 1LL * p.x * p.x + 1LL * p.y * p.y;

        if (maxHeap.getSize() < k) {
            maxHeap.insert(p);
        } else if (p.distSq < maxHeap.getMax().distSq) {
            maxHeap.extractMax();
            maxHeap.insert(p);
        }
    }

    while (!maxHeap.isEmpty()) {
        result.push_back(maxHeap.extractMax());
    }
    reverse(result.begin(), result.end());
    return result;
}

void printPoints(vector<Point>& pts) {
    cout << "[";
    for (int i = 0; i < (int)pts.size(); i++) {
        if (i > 0) cout << ", ";
        cout << "(" << pts[i].x << ", " << pts[i].y << ")";
    }
    cout << "]";
}

int main() {
    vector<Point> p1 = {{1, 3, 0}, {-2, 2, 0}};
    int k1 = 1;
    vector<Point> res1 = kClosestPoints(p1, k1);
    cout << "Test 1 (k = " << k1 << "): ";
    printPoints(res1);
    cout << "\n";

    vector<Point> p2 = {{3, 3, 0}, {5, -1, 0}, {-2, 4, 0}};
    int k2 = 2;
    vector<Point> res2 = kClosestPoints(p2, k2);
    cout << "Test 2 (k = " << k2 << "): ";
    printPoints(res2);
    cout << "\n";

    vector<Point> p3 = {{1, 0, 0}, {2, 1, 0}, {0, -1, 0}, {-3, 2, 0}, {1, 1, 0}};
    int k3 = 3;
    vector<Point> res3 = kClosestPoints(p3, k3);
    cout << "Test 3 (k = " << k3 << "): ";
    printPoints(res3);
    cout << "\n";

    return 0;
}
