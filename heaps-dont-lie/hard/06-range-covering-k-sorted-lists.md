# Range Covering K Sorted Lists

- **Difficulty:** Hard
- **Reference:** [GeeksforGeeks — Find Smallest Range Containing Elements from K Lists](https://www.geeksforgeeks.org/dsa/find-smallest-range-containing-elements-from-k-lists/)

---

## 1. Problem Statement

Given `k` lists of sorted integers in non-decreasing order, find the **smallest range `[a, b]`** that includes at least one number from each of the `k` lists.

Range `[a, b]` is smaller than `[c, d]` if `b - a < d - c`, or if `b - a == d - c` and `a < c`.

---

## 2. Pattern Recognition & Intuition

### Why think "Heap" here?
Pick one element from each of the `k` lists: $\{x_0, x_1, \dots, x_{k-1}\}$.
The tightest interval covering those $k$ chosen numbers is simply:
$$[\min_i x_i,\ \max_i x_i]$$

To shrink the span $\max - \min$, what can we do?
- Decreasing $\max$ is impossible because the lists are sorted in ascending order and moving forward only increases values.
- The **only** way to potentially tighten the interval is to **advance the minimum element** (`minEntry`) to the next value in its own list!

This turns the problem into a **K-Way Merge with a Min-Heap + Running Max**:
- A **Min-Heap of size $k$** gives us the current minimum across all $k$ lists in $O(\log k)$.
- A single scalar variable `currMax` tracks the current maximum across the $k$ elements in the heap in $O(1)$ (since elements only increase, `currMax = max(currMax, nextVal)` never needs shrinking!).

### Min-Heap vs. Max-Heap Decision Rule
- We must always locate and advance the **smallest** element in the active $k$-tuple $\rightarrow$ **Min-Heap** (`size == k`).
- We do **not** need a second Max-Heap because values only increase monotonically as we advance pointers; a single `int currMax` suffices!

### Why this beats naive approaches
- **Checking all $k$-tuples or merging all elements + sliding window:** Merging all $N$ elements into a sorted array of `(val, listIdx)` and running a sliding window takes $O(N \log N)$ and $O(N)$ extra space.
- **Min-Heap + `currMax`:** Takes $O(N \log k)$ time and only **$O(k)$ extra space**, never storing more than 1 element per list.

### Key Invariants & Edge Cases
- **Full-coverage invariant:** As long as the heap contains exactly one element from each of the $k$ lists, `[minHeap.getMin().val, currMax]` is guaranteed to cover all $k$ lists.
- **Termination rule:** The moment the list containing the current minimum runs out of elements (`nextIdx >= lists[listIdx].size()`), we **stop immediately**! Even if other lists have remaining elements, they can only increase `currMax`, never decrease `min`, so no smaller covering range is possible.
- **Overflow guard:** Compute `currMax - currStart` in `long long` when comparing range widths so large positive/negative coordinates (e.g., `[-10^5, 10^5]` or `INT_MIN`/`INT_MAX`) never overflow `int`.

---

## 3. Step-by-Step Trace

Using **Test 1** from `main()`:
- List 0: `[4, 10, 15, 24, 26]`
- List 1: `[0, 9, 12, 20]`
- List 2: `[5, 18, 22, 30]`

Initial seeding: push `(4, L0, 0)`, `(0, L1, 0)`, `(5, L2, 0)`. `currMax = 5`, `best = [0, 5]` (width `5`).

| Step | Pop `minEntry` `(val, L, idx)` | Candidate Range `[min, currMax]` | Best Range so far | Push Next Element | Updated `currMax` |
| :--- | :--- | :--- | :--- | :--- | :--- |
| 1 | `(0, L1, 0)` | `[0, 5]` (width `5`) | `[0, 5]` | `(9, L1, 1)` | `max(5, 9) = 9` |
| 2 | `(4, L0, 0)` | `[4, 9]` (width `5`, `4 > 0`) | `[0, 5]` | `(10, L0, 1)` | `max(9, 10) = 10` |
| 3 | `(5, L2, 0)` | `[5, 10]` (width `5`) | `[0, 5]` | `(18, L2, 1)` | `max(10, 18) = 18` |
| 4 | `(9, L1, 1)` | `[9, 18]` (width `9`) | `[0, 5]` | `(12, L1, 2)` | `18` |
| 5 | `(10, L0, 1)` | `[10, 18]` (width `8`) | `[0, 5]` | `(15, L0, 2)` | `18` |
| 6 | `(12, L1, 2)` | `[12, 18]` (width `6`) | `[0, 5]` | `(20, L1, 3)` | `max(18, 20) = 20` |
| 7 | `(15, L0, 2)` | `[15, 20]` (width `5`) | `[0, 5]` | `(24, L0, 3)` | `max(20, 24) = 24` |
| 8 | `(18, L2, 1)` | `[18, 24]` (width `6`) | `[0, 5]` | `(22, L2, 2)` | `24` |
| 9 | `(20, L1, 3)` | `[20, 24]` (**width `4`!**) | **`[20, 24]`** | List 1 exhausted! **STOP** | — |

Final Output: `[20, 24]` (contains `24` from List 0, `20` from List 1, `22` from List 2).

---

## 4. Time & Space Complexity

Let $N$ be the total number of elements across all $k$ lists.
- **Time Complexity:** $O(N \log k)$
  - Initial heap has $k$ elements: $O(k \log k)$.
  - Each loop iteration extracts once and inserts at most once ($O(\log k)$), running at most $N$ times.
- **Space Complexity:** $O(k)$
  - The `MinHeap` holds at most $k$ `ListEntry` items at all times.

---

## 5. C++ Implementation

See [06-range-covering-k-sorted-lists.cpp](06-range-covering-k-sorted-lists.cpp).

```cpp
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

struct ListEntry {
    int val;
    int listIdx;
    int elemIdx;
};

class MinHeap {
private:
    vector<ListEntry> items;
    int size;

    void heapifyUp(int curr) {
        ListEntry x = items[curr];
        while (curr / 2 > 0 && x.val < items[curr / 2].val) {
            items[curr] = items[curr / 2];
            curr = curr / 2;
        }
        items[curr] = x;
    }

    void minHeapify(int curr) {
        ListEntry x = items[curr];
        while (curr * 2 <= size) {
            int smallest = curr * 2;
            if (smallest + 1 <= size && items[smallest + 1].val < items[smallest].val) {
                smallest++;
            }
            if (items[smallest].val < x.val) {
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
        items.push_back({0, 0, 0});
        size = 0;
    }

    bool insert(ListEntry entry) {
        if (size + 1 < (int)items.size()) {
            items[++size] = entry;
        } else {
            items.push_back(entry);
            size++;
        }
        heapifyUp(size);
        return true;
    }

    ListEntry getMin() {
        if (size == 0) {
            return {0, -1, -1};
        }
        return items[1];
    }

    ListEntry extractMin() {
        if (size == 0) {
            return {0, -1, -1};
        }
        ListEntry top = items[1];
        items[1] = items[size];
        size--;
        if (size > 0) {
            minHeapify(1);
        }
        return top;
    }

    bool isEmpty() {
        return size == 0;
    }

    int getSize() {
        return size;
    }
};

vector<int> smallestRangeCoveringKLists(vector<vector<int>>& lists) {
    if (lists.empty()) {
        return {};
    }

    MinHeap heap;
    int currMax = lists[0][0];

    for (int i = 0; i < (int)lists.size(); i++) {
        if (lists[i].empty()) {
            return {};
        }
        int firstVal = lists[i][0];
        heap.insert({firstVal, i, 0});
        currMax = max(currMax, firstVal);
    }

    int bestStart = heap.getMin().val;
    int bestEnd = currMax;

    while (true) {
        ListEntry minEntry = heap.extractMin();
        int currStart = minEntry.val;

        if ((long long)currMax - currStart < (long long)bestEnd - bestStart ||
            ((long long)currMax - currStart == (long long)bestEnd - bestStart && currStart < bestStart)) {
            bestStart = currStart;
            bestEnd = currMax;
        }

        int nextIdx = minEntry.elemIdx + 1;
        if (nextIdx >= (int)lists[minEntry.listIdx].size()) {
            break; // One list exhausted; no further window covers all k lists
        }

        int nextVal = lists[minEntry.listIdx][nextIdx];
        heap.insert({nextVal, minEntry.listIdx, nextIdx});
        currMax = max(currMax, nextVal);
    }

    return {bestStart, bestEnd};
}

int main() {
    vector<vector<int>> lists1 = {
        {4, 10, 15, 24, 26},
        {0, 9, 12, 20},
        {5, 18, 22, 30}
    };
    vector<int> ans1 = smallestRangeCoveringKLists(lists1);
    cout << "Test 1 (Classic 3 lists): [" << ans1[0] << ", " << ans1[1] << "]\n";

    vector<vector<int>> lists2 = {
        {1, 2, 3},
        {1, 2, 3},
        {1, 2, 3}
    };
    vector<int> ans2 = smallestRangeCoveringKLists(lists2);
    cout << "Test 2 (Identical lists): [" << ans2[0] << ", " << ans2[1] << "]\n";

    vector<vector<int>> lists3 = {
        {-5, -1, 4},
        {-3, 2, 10},
        {0, 5, 8}
    };
    vector<int> ans3 = smallestRangeCoveringKLists(lists3);
    cout << "Test 3 (Lists with negative numbers): [" << ans3[0] << ", " << ans3[1] << "]\n";

    return 0;
}
```

---

## 6. LeetCode Pattern Takeaways

The **Min-Heap Frontier + Running Max** pattern appears in:
- **LC 632 — Smallest Range Covering Elements from K Lists** (exact problem)
- **LC 23 — Merge k Sorted Lists** (same frontier advancement without tracking `currMax`)
- **LC 1675 — Minimize Deviation in Array** (similar range-tightening between current min and max using a heap)
- **LC 76 — Minimum Window Substring** (conceptual cousin: shrink left boundary while maintaining full coverage)
