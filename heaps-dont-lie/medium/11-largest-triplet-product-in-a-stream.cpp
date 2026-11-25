#include <iostream>
#include <vector>
using namespace std;

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
            int child = curr * 2;
            if (child + 1 <= size && items[child + 1] < items[child]) {
                child++;
            }
            if (items[child] < x) {
                items[curr] = items[child];
                curr = child;
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

    void insert(int x) {
        size++;
        if (size < (int)items.size()) {
            items[size] = x;
        } else {
            items.push_back(x);
        }
        heapifyUp(size);
    }

    int getMin() {
        if (size == 0) return -1;
        return items[1];
    }

    int extractMin() {
        if (size == 0) return -1;
        int minVal = items[1];
        items[1] = items[size];
        size--;
        if (size > 0) {
            minHeapify(1);
        }
        return minVal;
    }

    int keyAt(int idx) {
        return items[idx];
    }

    bool isEmpty() {
        return size == 0;
    }

    int getSize() {
        return size;
    }
};

vector<long long> largestTripletProductStream(vector<int>& stream) {
    MinHeap topThree;
    vector<long long> result;

    for (int i = 0; i < (int)stream.size(); i++) {
        int val = stream[i];
        if (topThree.getSize() < 3) {
            topThree.insert(val);
        } else if (val > topThree.getMin()) {
            topThree.extractMin();
            topThree.insert(val);
        }

        if (topThree.getSize() < 3) {
            result.push_back(-1);
        } else {
            long long prod = 1LL * topThree.keyAt(1) * topThree.keyAt(2) * topThree.keyAt(3);
            result.push_back(prod);
        }
    }

    return result;
}

void printVector(vector<long long>& arr) {
    cout << "[";
    for (int i = 0; i < (int)arr.size(); i++) {
        if (i > 0) cout << ", ";
        cout << arr[i];
    }
    cout << "]";
}

int main() {
    vector<int> s1 = {1, 2, 3, 4, 5};
    vector<long long> res1 = largestTripletProductStream(s1);
    cout << "Test 1: ";
    printVector(res1);
    cout << "\n";

    vector<int> s2 = {10, 3, 5, 6, 20};
    vector<long long> res2 = largestTripletProductStream(s2);
    cout << "Test 2: ";
    printVector(res2);
    cout << "\n";

    return 0;
}
