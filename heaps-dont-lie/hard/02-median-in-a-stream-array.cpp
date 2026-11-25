#include <iomanip>
#include <iostream>
#define MAX 100
using namespace std;

class MaxHeap {
private:
    int items[MAX];
    int size;

    void heapifyUp(int curr) {
        int x = items[curr];
        while (curr / 2 > 0 && x > items[curr / 2]) {
            items[curr] = items[curr / 2];
            curr = curr / 2;
        }
        items[curr] = x;
    }

    void maxHeapify(int curr) {
        int x = items[curr];
        while (curr * 2 <= size) {
            int largest = curr * 2;
            if (largest + 1 <= size && items[largest + 1] > items[largest]) {
                largest++;
            }
            if (items[largest] > x) {
                items[curr] = items[largest];
                curr = largest;
            } else {
                break;
            }
        }
        items[curr] = x;
    }

public:
    MaxHeap() {
        size = 0;
    }

    bool insert(int val) {
        if (size < MAX - 1) {
            items[++size] = val;
            heapifyUp(size);
            return true;
        }
        return false;
    }

    int getMax() {
        if (size == 0) {
            return -1;
        }
        return items[1];
    }

    int extractMax() {
        if (size == 0) {
            return -1;
        }
        int maxVal = items[1];
        items[1] = items[size];
        size--;
        if (size > 0) {
            maxHeapify(1);
        }
        return maxVal;
    }

    bool remove() {
        if (size == 0) {
            return false;
        }
        extractMax();
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
            cout << items[i] << " ";
        }
        cout << endl;
    }
};

class MinHeap {
private:
    int items[MAX];
    int size;

    void heapifyUp(int curr) {
        int x = items[curr];
        while (curr / 2 > 0 && x < items[curr / 2]) {
            items[curr] = items[curr / 2];
            curr = curr / 2;
        }
        items[curr] = x;
    }

    void minHeapify(int curr) {
        int x = items[curr];
        while (curr * 2 <= size) {
            int smallest = curr * 2;
            if (smallest + 1 <= size && items[smallest + 1] < items[smallest]) {
                smallest++;
            }
            if (items[smallest] < x) {
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

    bool insert(int val) {
        if (size < MAX - 1) {
            items[++size] = val;
            heapifyUp(size);
            return true;
        }
        return false;
    }

    int getMin() {
        if (size == 0) {
            return -1;
        }
        return items[1];
    }

    int extractMin() {
        if (size == 0) {
            return -1;
        }
        int minVal = items[1];
        items[1] = items[size];
        size--;
        if (size > 0) {
            minHeapify(1);
        }
        return minVal;
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
            cout << items[i] << " ";
        }
        cout << endl;
    }
};

class MedianFinder {
private:
    MaxHeap lowerHalf; // Stores smaller half of stream
    MinHeap upperHalf; // Stores larger half of stream

public:
    void addNum(int num) {
        if (lowerHalf.isEmpty() || num <= lowerHalf.getMax()) {
            lowerHalf.insert(num);
        } else {
            upperHalf.insert(num);
        }

        // Maintain size invariant: lowerHalf.size == upperHalf.size or +1
        if (lowerHalf.getSize() > upperHalf.getSize() + 1) {
            upperHalf.insert(lowerHalf.extractMax());
        } else if (upperHalf.getSize() > lowerHalf.getSize()) {
            lowerHalf.insert(upperHalf.extractMin());
        }
    }

    double findMedian() {
        if (lowerHalf.isEmpty()) {
            return 0.0;
        }
        if (lowerHalf.getSize() > upperHalf.getSize()) {
            return (double)lowerHalf.getMax();
        }
        return ((double)lowerHalf.getMax() + (double)upperHalf.getMin()) / 2.0;
    }
};

void runStreamTest(int stream[], int n) {
    MedianFinder mf;
    for (int i = 0; i < n; i++) {
        mf.addNum(stream[i]);
        cout << "  After inserting " << stream[i]
             << " -> median = " << fixed << setprecision(1) << mf.findMedian() << "\n";
    }
}

int main() {
    int stream1[6] = {5, 15, 1, 3, 2, 8};
    cout << "Test 1 (Classic integer stream):\n";
    runStreamTest(stream1, 6);

    int stream2[5] = {-5, -10, -5, 0, 5};
    cout << "Test 2 (Negatives and duplicates):\n";
    runStreamTest(stream2, 5);

    return 0;
}
