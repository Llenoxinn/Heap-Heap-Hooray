# Heaps Don't Lie — DSA & LeetCode Heap Study Playbook

A comprehensive C++ study collection of **34 classic Heap problems** (Easy, Medium, and Hard), designed to build **LeetCode pattern recognition**, trace intuition, and clean from-scratch heap implementations.

Every problem provides **two standalone C++ solutions**:
1. **Standard C++ (`<slug>.cpp`)**: Implements its own custom `MinHeap`, `MaxHeap`, or `heapify` operations from scratch (no `std::priority_queue`) using `std::vector` for dynamic sizing.
2. **Pure Raw-Array C++ (`<slug>-array.cpp`)**: Zero STL containers (`vector`, `list`, `map`, `set`, `queue`, `algorithm`, `string`) — uses fixed-size raw arrays (`#define MAX ...`, `items[MAX]`, `int size`, `int arr[]`, `int n`) directly matching [`Heap-Implementation-OPTIMIZED.cpp`](../Heap-Implementation-OPTIMIZED.cpp) and [`heapSort.cpp`](../heapSort.cpp).

All `.cpp` solutions:
- Use **no global variables**, **no `const`**, and **no `static`**.
- Compile cleanly with `g++ -std=c++17 -Wall -Wextra` and include runnable `main()` test cases.

---

## LeetCode Pattern Recognition Cheat Sheet

When reading a problem prompt, look for these triggers:

| Pattern | Prompt Triggers | Which Heap? |
| :--- | :--- | :--- |
| **1. Top-K / K-th Element** | *"K-th largest"*, *"K-th smallest"*, *"Top K frequent"*, *"K closest"* | **Opposite heap of size $K$**: Min-Heap of size $K$ for K-th *largest*; Max-Heap of size $K$ for K-th *smallest*. |
| **2. K-Way Merge** | *"Merge K sorted lists/arrays"*, *"Smallest range across K lists"*, *"K max sum combinations"* | **Min-Heap** (or Max-Heap) holding 1 active frontier element per list/stream. |
| **3. Two Heaps** | *"Median in a stream"*, *"Sliding window median"*, *"Split into min/max halves"* | **Max-Heap** (lower half) + **Min-Heap** (upper half), balanced within size $\pm 1$. |
| **4. Greedy Combination** | *"Minimum cost to connect ropes"*, *"Huffman coding"*, *"Largest derangement"* | **Min-Heap** when always combining the 2 smallest; **Max-Heap** when placing the largest available first. |
| **5. Graphs & Sweep-Line** | *"Shortest path (non-negative weights)"*, *"Minimum spanning tree"*, *"Skyline / active intervals"* | **Min-Heap** for Dijkstra/Prim; **Max-Heap** (with lazy deletion) for active height/interval max. |
| **6. Structural & In-Place Heap** | *"Heap sort"*, *"Check if tree/array is heap"*, *"Convert Min-Heap to Max-Heap"*, *"BST to Min-Heap"* | Direct **0-indexed / 1-indexed** parent-child formulas and $O(n)$ bottom-up `heapify`. |

---

## Easy Problems (5)

| # | Problem | Guide & Trace | C++ Solution | Raw-Array C++ | Key Pattern |
| :---: | :--- | :--- | :--- | :--- | :--- |
| 01 | Check if an array is Heap | [Guide](easy/01-check-if-array-is-heap.md) | [Code](easy/01-check-if-array-is-heap.cpp) | [Raw Array](easy/01-check-if-array-is-heap-array.cpp) | Parent-child invariant check |
| 02 | K'th Smallest & K'th Largest | [Guide](easy/02-kth-smallest-and-kth-largest.md) | [Code](easy/02-kth-smallest-and-kth-largest.cpp) | [Raw Array](easy/02-kth-smallest-and-kth-largest-array.cpp) | Opposite heap of size $K$ |
| 03 | Heap Sort (Ascending) | [Guide](easy/03-heap-sort.md) | [Code](easy/03-heap-sort.cpp) | [Raw Array](easy/03-heap-sort-array.cpp) | Build Max-Heap + extract to end |
| 04 | Heap Sort (Decreasing Order) | [Guide](easy/04-heap-sort-decreasing-order.md) | [Code](easy/04-heap-sort-decreasing-order.cpp) | [Raw Array](easy/04-heap-sort-decreasing-order-array.cpp) | Build Min-Heap + extract to end |
| 05 | Height of a Heap with $N$ Nodes | [Guide](easy/05-height-of-heap-with-n-nodes.md) | [Code](easy/05-height-of-heap-with-n-nodes.cpp) | [Raw Array](easy/05-height-of-heap-with-n-nodes-array.cpp) | Complete binary tree level math |

---

## Medium Problems (16)

| # | Problem | Guide & Trace | C++ Solution | Raw-Array C++ | Key Pattern |
| :---: | :--- | :--- | :--- | :--- | :--- |
| 01 | Nearly Sorted ($k$-Sorted) Array | [Guide](medium/01-nearly-sorted-array.md) | [Code](medium/01-nearly-sorted-array.cpp) | [Raw Array](medium/01-nearly-sorted-array-array.cpp) | Sliding window Min-Heap of size $k+1$ |
| 02 | K Largest & K Smallest Elements | [Guide](medium/02-k-largest-and-k-smallest-elements.md) | [Code](medium/02-k-largest-and-k-smallest-elements.cpp) | [Raw Array](medium/02-k-largest-and-k-smallest-elements-array.cpp) | Opposite heap of size $K$ |
| 03 | $k$ Closest Elements | [Guide](medium/03-k-closest-elements.md) | [Code](medium/03-k-closest-elements.cpp) | [Raw Array](medium/03-k-closest-elements-array.cpp) | Max-Heap of `(distance, value)` size $k$ |
| 04 | Max Diff Between Two $m$-Sized Subsets | [Guide](medium/04-max-diff-between-two-m-sized-subsets.md) | [Code](medium/04-max-diff-between-two-m-sized-subsets.cpp) | [Raw Array](medium/04-max-diff-between-two-m-sized-subsets-array.cpp) | Min-Heap + Max-Heap subset selection |
| 05 | Check if Binary Tree is Heap | [Guide](medium/05-check-if-binary-tree-is-heap.md) | [Code](medium/05-check-if-binary-tree-is-heap.cpp) | [Raw Array](medium/05-check-if-binary-tree-is-heap-array.cpp) | Completeness + heap order check |
| 06 | Huffman Coding | [Guide](medium/06-huffman-coding.md) | [Code](medium/06-huffman-coding.cpp) | [Raw Array](medium/06-huffman-coding-array.cpp) | Greedy Min-Heap tree merging |
| 07 | Nodes Less Than Value in Min-Heap | [Guide](medium/07-nodes-less-than-value-in-min-heap.md) | [Code](medium/07-nodes-less-than-value-in-min-heap.cpp) | [Raw Array](medium/07-nodes-less-than-value-in-min-heap-array.cpp) | Pruned DFS on heap array |
| 08 | Connect $n$ Ropes with Minimum Cost | [Guide](medium/08-connect-n-ropes-minimum-cost.md) | [Code](medium/08-connect-n-ropes-minimum-cost.cpp) | [Raw Array](medium/08-connect-n-ropes-minimum-cost-array.cpp) | Greedy Min-Heap combine-two-smallest |
| 09 | $K$ Maximum Sum Combinations | [Guide](medium/09-k-maximum-sum-combinations.md) | [Code](medium/09-k-maximum-sum-combinations.cpp) | [Raw Array](medium/09-k-maximum-sum-combinations-array.cpp) | Max-Heap + visited `(i, j)` frontier |
| 10 | K'th Largest in a Stream | [Guide](medium/10-kth-largest-in-a-stream.md) | [Code](medium/10-kth-largest-in-a-stream.cpp) | [Raw Array](medium/10-kth-largest-in-a-stream-array.cpp) | Fixed-size $K$ Min-Heap |
| 11 | Largest Triplet Product in a Stream | [Guide](medium/11-largest-triplet-product-in-a-stream.md) | [Code](medium/11-largest-triplet-product-in-a-stream.cpp) | [Raw Array](medium/11-largest-triplet-product-in-a-stream-array.cpp) | Top-3 Min-Heap in a stream |
| 12 | $k$ Most Frequent Elements | [Guide](medium/12-k-most-frequent.md) | [Code](medium/12-k-most-frequent.cpp) | [Raw Array](medium/12-k-most-frequent-array.cpp) | Frequency count + Min-Heap of size $k$ |
| 13 | Convert Min-Heap to Max-Heap | [Guide](medium/13-min-heap-to-max-heap.md) | [Code](medium/13-min-heap-to-max-heap.cpp) | [Raw Array](medium/13-min-heap-to-max-heap-array.cpp) | $O(n)$ bottom-up `maxHeapify` |
| 14 | Check Min-Heap from Level Order | [Guide](medium/14-check-min-heap-from-level-order.md) | [Code](medium/14-check-min-heap-from-level-order.cpp) | [Raw Array](medium/14-check-min-heap-from-level-order-array.cpp) | Level-order parent-child invariant |
| 15 | Convert BST to Min-Heap | [Guide](medium/15-bst-to-min-heap.md) | [Code](medium/15-bst-to-min-heap.cpp) | [Raw Array](medium/15-bst-to-min-heap-array.cpp) | Inorder sorted array + Preorder fill |
| 16 | $K$ Closest Points to the Origin | [Guide](medium/16-k-closest-points-to-origin.md) | [Code](medium/16-k-closest-points-to-origin.cpp) | [Raw Array](medium/16-k-closest-points-to-origin-array.cpp) | Max-Heap of size $K$ by $x^2 + y^2$ |

---

## Hard Problems (13)

| # | Problem | Guide & Trace | C++ Solution | Raw-Array C++ | Key Pattern |
| :---: | :--- | :--- | :--- | :--- | :--- |
| 01 | The Skyline Problem | [Guide](hard/01-the-skyline-problem.md) | [Code](hard/01-the-skyline-problem.cpp) | [Raw Array](hard/01-the-skyline-problem-array.cpp) | Sweep-line + Max-Heap lazy deletion |
| 02 | Median in a Stream | [Guide](hard/02-median-in-a-stream.md) | [Code](hard/02-median-in-a-stream.cpp) | [Raw Array](hard/02-median-in-a-stream-array.cpp) | Two Heaps (Max-Heap + Min-Heap) |
| 03 | K-th Largest Sum Subarray | [Guide](hard/03-kth-largest-sum-subarray.md) | [Code](hard/03-kth-largest-sum-subarray.cpp) | [Raw Array](hard/03-kth-largest-sum-subarray-array.cpp) | Prefix sums + Min-Heap of size $K$ |
| 04 | Sort Elements by Frequency | [Guide](hard/04-sort-by-frequency.md) | [Code](hard/04-sort-by-frequency.cpp) | [Raw Array](hard/04-sort-by-frequency-array.cpp) | Frequency count + Max-Heap tie-breaker |
| 05 | Merge $k$ Sorted Arrays & Linked Lists | [Guide](hard/05-merge-k-sorted-arrays-and-linked-lists.md) | [Code](hard/05-merge-k-sorted-arrays-and-linked-lists.cpp) | [Raw Array](hard/05-merge-k-sorted-arrays-and-linked-lists-array.cpp) | K-way merge Min-Heap |
| 06 | Range Covering $K$ Sorted Lists | [Guide](hard/06-range-covering-k-sorted-lists.md) | [Code](hard/06-range-covering-k-sorted-lists.cpp) | [Raw Array](hard/06-range-covering-k-sorted-lists-array.cpp) | K-way Min-Heap + running max |
| 07 | Prim's Minimum Spanning Tree | [Guide](hard/07-prims-minimum-spanning-tree.md) | [Code](hard/07-prims-minimum-spanning-tree.cpp) | [Raw Array](hard/07-prims-minimum-spanning-tree-array.cpp) | Greedy MST with edge Min-Heap |
| 08 | Dijkstra's Shortest Path | [Guide](hard/08-dijkstras-shortest-path.md) | [Code](hard/08-dijkstras-shortest-path.cpp) | [Raw Array](hard/08-dijkstras-shortest-path-array.cpp) | Shortest-distance Min-Heap |
| 09 | Sort Numbers from Different Machines | [Guide](hard/09-sort-from-different-machines.md) | [Code](hard/09-sort-from-different-machines.cpp) | [Raw Array](hard/09-sort-from-different-machines-array.cpp) | K-way merge across machine lists |
| 10 | Largest Derangement of a Sequence | [Guide](hard/10-largest-derangement-of-sequence.md) | [Code](hard/10-largest-derangement-of-sequence.cpp) | [Raw Array](hard/10-largest-derangement-of-sequence-array.cpp) | Greedy Max-Heap + last-two fix |
| 11 | Merge Two Binary Max Heaps | [Guide](hard/11-merge-two-binary-max-heaps.md) | [Code](hard/11-merge-two-binary-max-heaps.cpp) | [Raw Array](hard/11-merge-two-binary-max-heaps-array.cpp) | Concatenate + $O(N+M)$ `maxHeapify` |
| 12 | Sum Between $k_1$'th and $k_2$'th Smallest | [Guide](hard/12-sum-between-k1th-and-k2th-smallest.md) | [Code](hard/12-sum-between-k1th-and-k2th-smallest.cpp) | [Raw Array](hard/12-sum-between-k1th-and-k2th-smallest-array.cpp) | Min-Heap / Max-Heap range extraction |
| 13 | Data Structure with $O(1)$ Min & Max | [Guide](hard/13-data-structure-with-min-and-max.md) | [Code](hard/13-data-structure-with-min-and-max.cpp) | [Raw Array](hard/13-data-structure-with-min-and-max-array.cpp) | Synchronized Min-Heap + Max-Heap + DLL |

---

## Compiling & Running Any Problem

```powershell
# Standard C++ (vector-backed custom Heap)
g++ -std=c++17 -Wall -Wextra heaps-dont-lie/easy/01-check-if-array-is-heap.cpp -o check.exe
.\check.exe

# Pure Raw-Array C++ (no STL containers, Heap-Implementation-OPTIMIZED style)
g++ -std=c++17 -Wall -Wextra heaps-dont-lie/easy/01-check-if-array-is-heap-array.cpp -o check_raw.exe
.\check_raw.exe
```

---

## Companion Resources

- **Repository Root:** [`Heap-Heap-Hooray`](../README.md) · [`heapNotes.md`](../heapNotes.md)
- **Coursework Hub:** [`Data-Structures-Machine-Problems`](https://github.com/Llenoxinn/Data-Structures-Machine-Problems) (pairs with [`MP 13: Min-Heap Array Validator`](https://github.com/Llenoxinn/Data-Structures-Machine-Problems/tree/main/MP%2013))
- **Next Playbook:** [`HashbyHash / hashuh`](https://github.com/Llenoxinn/HashbyHash/tree/main/hashuh) — Hashing & Hash Table DSA/LeetCode Playbook
