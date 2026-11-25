#include <iomanip>
#include <iostream>
#include <vector>
using namespace std;

class MaxHeap {
private:
    vector<int> items;
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
        items.push_back(0);
        size = 0;
    }

    bool insert(int val) {
        if (size + 1 < (int)items.size()) {
            items[++size] = val;
        } else {
            items.push_back(val);
            size++;
        }
        heapifyUp(size);
        return true;
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

    bool isEmpty() {
        return size == 0;
    }

    int getSize() {
        return size;
    }
};

class MinHeap {
private:
    vector<int> items;
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
        items.push_back(0);
        size = 0;
    }

    bool insert(int val) {
        if (size + 1 < (int)items.size()) {
            items[++size] = val;
        } else {
            items.push_back(val);
            size++;
        }
        heapifyUp(size);
        return true;
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

    bool isEmpty() {
        return size == 0;
    }

    int getSize() {
        return size;
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

void runStreamTest(vector<int>& stream, string label) {
    MedianFinder mf;
    cout << label << ":\n";
    for (int i = 0; i < (int)stream.size(); i++) {
        mf.addNum(stream[i]);
        cout << "  After inserting " << stream[i]
             << " -> median = " << fixed << setprecision(1) << mf.findMedian() << "\n";
    }
}

int main() {
    vector<int> stream1 = {5, 15, 1, 3, 2, 8};
    runStreamTest(stream1, "Test 1 (Classic integer stream)");

    vector<int> stream2 = {-5, -10, -5, 0, 5};
    runStreamTest(stream2, "Test 2 (Negatives and duplicates)");

    return 0;
}
