#include <iostream>
#define MAX 100
using namespace std;

bool isMinHeapFromLevelOrder(int level[], int n) {
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

void printArray(int arr[], int n) {
    cout << "[";
    for (int i = 0; i < n; i++) {
        if (i > 0) cout << ", ";
        cout << arr[i];
    }
    cout << "]";
}

int main() {
    int t1[MAX] = {10, 15, 14, 25, 30};
    int n1 = 5;
    cout << "Test 1 ";
    printArray(t1, n1);
    cout << " -> " << (isMinHeapFromLevelOrder(t1, n1) ? "Yes (Min-Heap)" : "No (Not a Min-Heap)") << "\n";

    int t2[MAX] = {30, 56, 22, 49, 30, 51, 2};
    int n2 = 7;
    cout << "Test 2 ";
    printArray(t2, n2);
    cout << " -> " << (isMinHeapFromLevelOrder(t2, n2) ? "Yes (Min-Heap)" : "No (Not a Min-Heap)") << "\n";

    int t3[MAX] = {1, 3, 6, 5, 9, 8};
    int n3 = 6;
    cout << "Test 3 ";
    printArray(t3, n3);
    cout << " -> " << (isMinHeapFromLevelOrder(t3, n3) ? "Yes (Min-Heap)" : "No (Not a Min-Heap)") << "\n";

    return 0;
}
