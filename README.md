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
- [ ] Bottom-up heap construction (`heapify` in $O(n)$) & Heap Sort
- [ ] Priority Queue applications & `std::priority_queue`
