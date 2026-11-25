#include <iostream>
#include <vector>
using namespace std;

bool isMinHeapFromLevelOrder(vector<int>& level) {
    int n = (int)level.size();
    for (int i = n / 2 - 1; i >= 0; i--) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < n && level[i] > level[left]) {
            return false;
        }
        if (right < n && level[i] > level[right]) {
            return false;
        }
    }
    return true;
}

void printVector(vector<int>& arr) {
    cout << "[";
    for (int i = 0; i < (int)arr.size(); i++) {
        if (i > 0) cout << ", ";
        cout << arr[i];
    }
    cout << "]";
}

int main() {
    vector<int> t1 = {10, 15, 14, 25, 30};
    cout << "Test 1 ";
    printVector(t1);
    cout << " -> " << (isMinHeapFromLevelOrder(t1) ? "Yes (Min-Heap)" : "No (Not a Min-Heap)") << "\n";

    vector<int> t2 = {30, 56, 22, 49, 30, 51, 2};
    cout << "Test 2 ";
    printVector(t2);
    cout << " -> " << (isMinHeapFromLevelOrder(t2) ? "Yes (Min-Heap)" : "No (Not a Min-Heap)") << "\n";

    vector<int> t3 = {1, 3, 6, 5, 9, 8};
    cout << "Test 3 ";
    printVector(t3);
    cout << " -> " << (isMinHeapFromLevelOrder(t3) ? "Yes (Min-Heap)" : "No (Not a Min-Heap)") << "\n";

    return 0;
}
