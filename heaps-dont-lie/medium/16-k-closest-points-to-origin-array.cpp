#include <iostream>
#define MAX 100
using namespace std;

struct Point {
    int x;
    int y;
    long long distSq;
};

class MaxHeap {
private:
    Point items[MAX];
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
        size = 0;
    }

    bool insert(Point p) {
        if (size < MAX - 1) {
            items[++size] = p;
            heapifyUp(size);
            return true;
        }
        return false;
    }

    Point getMax() {
        if (size == 0) {
            Point empty = {0, 0, 0};
            return empty;
        }
        return items[1];
    }

    Point extractMax() {
        if (size == 0) {
            Point empty = {0, 0, 0};
            return empty;
        }
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

    void display() {
        cout << "size: " << size << endl;
        for (int i = 1; i <= size; i++) {
            cout << "(" << items[i].x << ", " << items[i].y << ") ";
        }
        cout << endl;
    }
};

void reversePoints(Point pts[], int n) {
    int left = 0;
    int right = n - 1;
    while (left < right) {
        Point temp = pts[left];
        pts[left] = pts[right];
        pts[right] = temp;
        left++;
        right--;
    }
}

int kClosestPoints(Point points[], int n, int k, Point result[]) {
    if (k <= 0 || n <= 0) return 0;

    MaxHeap maxHeap;
    for (int i = 0; i < n; i++) {
        Point p = points[i];
        p.distSq = 1LL * p.x * p.x + 1LL * p.y * p.y;

        if (maxHeap.getSize() < k) {
            maxHeap.insert(p);
        } else if (p.distSq < maxHeap.getMax().distSq) {
            maxHeap.extractMax();
            maxHeap.insert(p);
        }
    }

    int resCount = 0;
    while (!maxHeap.isEmpty() && resCount < MAX) {
        result[resCount++] = maxHeap.extractMax();
    }
    reversePoints(result, resCount);
    return resCount;
}

void printPoints(Point pts[], int n) {
    cout << "[";
    for (int i = 0; i < n; i++) {
        if (i > 0) cout << ", ";
        cout << "(" << pts[i].x << ", " << pts[i].y << ")";
    }
    cout << "]";
}

int main() {
    Point p1[MAX] = {{1, 3, 0}, {-2, 2, 0}};
    int n1 = 2;
    int k1 = 1;
    Point res1[MAX];
    int resLen1 = kClosestPoints(p1, n1, k1, res1);
    cout << "Test 1 (k = " << k1 << "): ";
    printPoints(res1, resLen1);
    cout << "\n";

    Point p2[MAX] = {{3, 3, 0}, {5, -1, 0}, {-2, 4, 0}};
    int n2 = 3;
    int k2 = 2;
    Point res2[MAX];
    int resLen2 = kClosestPoints(p2, n2, k2, res2);
    cout << "Test 2 (k = " << k2 << "): ";
    printPoints(res2, resLen2);
    cout << "\n";

    Point p3[MAX] = {{1, 0, 0}, {2, 1, 0}, {0, -1, 0}, {-3, 2, 0}, {1, 1, 0}};
    int n3 = 5;
    int k3 = 3;
    Point res3[MAX];
    int resLen3 = kClosestPoints(p3, n3, k3, res3);
    cout << "Test 3 (k = " << k3 << "): ";
    printPoints(res3, resLen3);
    cout << "\n";

    return 0;
}
