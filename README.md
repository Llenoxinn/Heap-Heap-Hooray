# Heap-Heap-Hooray

A study repository for learning and practicing **Heap** data structures and algorithms in **C++**.

## Overview

This repository covers heap fundamentals, implementations, and related algorithms for Data Structures & Algorithms (DSA) study.

### Current Contents

- [`heapNotes.md`](heapNotes.md) — Study notes covering complete binary trees, min/max-heap properties, why heaps are used for priority queues, level-order array representation, and 1-indexed vs. 0-indexed parent/child index formulas.
- [`Heap-Implementation.cpp`](Heap-Implementation.cpp) — Array-based **1-indexed Min-Heap** implementation in C++ (`MAX = 10`, storing up to 9 elements) with:
  - `insert(int x)` — Inserts a value and restores the min-heap property via bottom-up heapify (percolate up).
  - `remove()` — Removes the minimum (root) element and restores the min-heap property via top-down heapify (percolate down).
  - `display()` — Prints the current heap size and array contents.
- [`Heap-Implementation-OPTIMIZED.cpp`](Heap-Implementation-OPTIMIZED.cpp) — Expanded **1-indexed Min-Heap** implementation refactoring percolation into `heapifyUp` and `minHeapify` helpers and adding `getMin`, `extractMin`, `decreaseKey`, `isEmpty`, `getSize`, and `keyAt` *(work in progress)*.
- [`heapSort.cpp`](heapSort.cpp) — **0-indexed Heap Sort** implementation featuring `maxheapify`, `minheapify`, and `decHeapSort` *(work in progress)*.
- [`heaps-dont-lie/`](heaps-dont-lie/README.md) — **DSA & LeetCode Heap Playbook**: 34 Easy, Medium, and Hard heap problems with pattern recognition guides, step-by-step traces, and from-scratch C++ implementations (no globals, `const`, `static`, or `std::priority_queue`).

## Complexity Reference (Binary Heap)

| Operation | Time Complexity |
| --- | --- |
| Peek Min / Max (`top`) | $O(1)$ |
| Insert (`push`) | $O(\log n)$ |
| Delete Root (`pop`) | $O(\log n)$ |
| Decrease / Increase Key | $O(\log n)$ |
| Build Heap (Floyd's algorithm) | $O(n)$ |
| Heap Sort | $O(n \log n)$ |

Space Complexity: $O(n)$ (or $O(1)$ auxiliary space for in-place heapsort).

## Build & Run

Compile and run using `g++` (or any C++ compiler):

```bash
g++ -std=c++17 -Wall -Wextra Heap-Implementation.cpp -o Heap-Implementation
./Heap-Implementation
```

On Windows (PowerShell):

```powershell
g++ -std=c++17 -Wall -Wextra Heap-Implementation.cpp -o Heap-Implementation.exe
.\Heap-Implementation.exe
```

## Study Topics

- [x] Heap properties & array index math (0-indexed vs. 1-indexed)
- [x] Array-based 1-indexed Min-Heap (`insert`, `remove`)
- [ ] Extended Min-Heap operations (`getMin`, `extractMin`, `decreaseKey`)
- [ ] Max-Heap implementation
- [x] Bottom-up heap construction (`heapify` in $O(n)$) & Heap Sort
- [x] 34 DSA & LeetCode Heap Problems ([`heaps-dont-lie/`](heaps-dont-lie/README.md))

## Related Repositories

Part of a connected Data Structures & Algorithms study series:

| Repository | Topic | Key Connections |
| --- | --- | --- |
| [**Data-Structures-Machine-Problems**](https://github.com/Llenoxinn/Data-Structures-Machine-Problems) | Coursework MPs, Assignments & 2nd LE Reviewer | See [**MP 13: Min-Heap Array Validator**](https://github.com/Llenoxinn/Data-Structures-Machine-Problems/tree/main/MP%2013) and the [Topics/Heaps guide](https://github.com/Llenoxinn/Data-Structures-Machine-Problems/tree/main/Topics/Heaps) |
| [**HashbyHash**](https://github.com/Llenoxinn/HashbyHash) | Hashing & Hash Tables | Companion topic notes (`hashnotes.md`), BST-chained `HashTable` (`hash.cpp`), and the [`hashuh/`](https://github.com/Llenoxinn/HashbyHash/tree/main/hashuh) playbook modeled after `heaps-dont-lie` |
