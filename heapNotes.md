# Heap Notes

A heap is still a tree, specifically a **complete binary tree** data structure, and it must satisfy the **heap properties**:

1. **Min-Heap:** The value of each child is greater than or equal to its parent (`child >= parent`).
2. **Max-Heap:** The value of each child is less than or equal to its parent (`child <= parent`).

## Why do we need a heap data structure (and why not just use other data structures)?

- Because standard queues only process items in the order they arrive (**First-In, First-Out / FIFO**), whereas a heap (commonly used as a **priority queue**) always gives you the highest- or lowest-priority item immediately, regardless of when it was added.
- Efficient insertion and deletion operations.

## Array Representation

A heap is typically represented as an **ARRAY**!
The traversal method used to achieve the array representation is **LEVEL ORDER**.

For example:

```text
1 3 6 5 9 8
```

Represented as an array (1-indexed):

```text
Value:  X  1  3  6  5  9  8
Index:  0  1  2  3  4  5  6
```

At index `1`, that is your **root**.

### Finding Parent and Children (1-Indexed)

- **Parent:** `arr[i / 2]`
- **Left child:** `arr[2 * i]`
- **Right child:** `arr[(2 * i) + 1]`

### Finding Parent and Children (0-Indexed)

If you want to start at index `0`:

- **Parent:** `arr[(i - 1) / 2]`
- **Left child:** `arr[(2 * i) + 1]`
- **Right child:** `arr[(2 * i) + 2]`
