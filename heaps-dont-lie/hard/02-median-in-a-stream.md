# Median in a Stream

- **Difficulty:** Hard
- **Reference:** [GeeksforGeeks — Median of Stream of Running Integers](https://www.geeksforgeeks.org/dsa/median-of-stream-of-integers-running-integers/)

---

## 1. Problem Statement

Given an input stream of integers arriving one at a time, maintain the data structure so that after every insertion you can report the **median** of all numbers seen so far:
- If the total count of elements is **odd**, the median is the single middle element.
- If the total count of elements is **even**, the median is the average of the two middle elements: $\frac{m_1 + m_2}{2}$.

---

## 2. Pattern Recognition & Intuition

### Why think "Heap" here?
The median only depends on the **boundary between the smaller half and the larger half** of the data:
- You do **not** need the entire array sorted.
- You only need instant $O(1)$ access to the **largest element of the lower half** and the **smallest element of the upper half**.

That immediately calls for the **Two-Heap Pattern**:
- **`lowerHalf` (Max-Heap):** Holds the smaller half of elements; its root is the largest value in the lower half.
- **`upperHalf` (Min-Heap):** Holds the larger half of elements; its root is the smallest value in the upper half.

### Min-Heap vs. Max-Heap Decision Rule
- `lowerHalf` needs the **maximum** of the left half $\rightarrow$ **Max-Heap**.
- `upperHalf` needs the **minimum** of the right half $\rightarrow$ **Min-Heap**.
Together, the two heap roots sit back-to-back right at the median!

### Why this beats naive approaches
- **Re-sorting after each arrival:** $O(N^2 \log N)$ total.
- **Insertion sort into a dynamic array:** $O(N^2)$ total due to shifting elements.
- **Two Heaps:** Each insertion takes $O(\log N)$ and each median query takes $O(1)$, giving $O(N \log N)$ total across $N$ elements.

### Key Invariants
Maintain these two invariants after every `addNum(num)`:
1. **Order invariant:** $\max(\text{lowerHalf}) \le \min(\text{upperHalf})$.
   - When `num` arrives, place it in `lowerHalf` if `lowerHalf.isEmpty() || num <= lowerHalf.getMax()`, otherwise in `upperHalf`.
2. **Balance invariant:** Either `lowerHalf.getSize() == upperHalf.getSize()` or `lowerHalf.getSize() == upperHalf.getSize() + 1`.
   - If `lowerHalf` grows by 2 ahead of `upperHalf`, move `lowerHalf.extractMax()` into `upperHalf`.
   - If `upperHalf` becomes larger than `lowerHalf`, move `upperHalf.extractMin()` into `lowerHalf`.

Because of the balance invariant, whenever total count is odd, the median is guaranteed to be `lowerHalf.getMax()`. Whenever total count is even, the median is `(lowerHalf.getMax() + upperHalf.getMin()) / 2.0`.

### Edge Cases
- **First element:** `lowerHalf` is empty, so `num` goes into `lowerHalf`, giving size `(1, 0)` — invariant satisfied.
- **Negative numbers & duplicates:** Comparisons use `<=` cleanly; duplicates can live on either side without breaking the order invariant.
- **Integer overflow on even median:** Cast both middle values to `double` before adding (`((double)a + (double)b) / 2.0`) so `INT_MAX + INT_MAX` cannot overflow `int`.

---

## 3. Step-by-Step Trace

Using **Test 1** from `main()`: `stream = [5, 15, 1, 3, 2, 8]`

| Step | Insert | Initial Placement | Rebalance Needed? | `lowerHalf` (MaxHeap) | `upperHalf` (MinHeap) | Median Calculation | Output |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| 1 | `5` | `lowerHalf` (empty) | No (1 vs 0) | `[5]` (top `5`) | `[]` | Odd: `lower.getMax()` | **`5.0`** |
| 2 | `15` | `15 > 5` $\rightarrow$ `upperHalf` | No (1 vs 1) | `[5]` (top `5`) | `[15]` (top `15`) | Even: `(5 + 15) / 2.0` | **`10.0`** |
| 3 | `1` | `1 <= 5` $\rightarrow$ `lowerHalf` | No (2 vs 1) | `[5, 1]` (top `5`) | `[15]` (top `15`) | Odd: `lower.getMax()` | **`5.0`** |
| 4 | `3` | `3 <= 5` $\rightarrow$ `lowerHalf` | Yes (3 vs 1) $\rightarrow$ move `5` to `upper` | `[3, 1]` (top `3`) | `[5, 15]` (top `5`) | Even: `(3 + 5) / 2.0` | **`4.0`** |
| 5 | `2` | `2 <= 3` $\rightarrow$ `lowerHalf` | No (3 vs 2) | `[3, 1, 2]` (top `3`) | `[5, 15]` (top `5`) | Odd: `lower.getMax()` | **`3.0`** |
| 6 | `8` | `8 > 3` $\rightarrow$ `upperHalf` | No (3 vs 3) | `[3, 1, 2]` (top `3`) | `[5, 15, 8]` (top `5`) | Even: `(3 + 5) / 2.0` | **`4.0`** |

---

## 4. Time & Space Complexity

- **Time Complexity:**
  - `addNum(num)`: $O(\log N)$ — at most 2 heap insertions and 1 heap extraction.
  - `findMedian()`: $O(1)$ — reads `getMax()` and/or `getMin()`.
  - Overall for $N$ stream elements: $O(N \log N)$.
- **Space Complexity:** $O(N)$ — every element from the stream is stored in either `lowerHalf` or `upperHalf`.

---

## 5. C++ Implementation

See [02-median-in-a-stream.cpp](02-median-in-a-stream.cpp).

```cpp
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
```

---

## 6. LeetCode Pattern Takeaways

The **Two-Heap Partition (Max-Heap + Min-Heap)** pattern powers:
- **LC 295 — Find Median from Data Stream** (exact problem)
- **LC 480 — Sliding Window Median** (Two Heaps + lazy deletion)
- **LC 502 — IPO** (sort/Min-Heap by capital required + Max-Heap of available profits)
- **LC 2102 — Sequentially Ordinal Rank Tracker** (Max-Heap + Min-Heap tracking the $i$-th ranked item as $i$ increments)
- **LC 1675 — Minimize Deviation in Array** / **LC 3013 — Divide an Array Into Subarrays With Minimum Cost II** (dynamic $k$-th boundary tracking)
