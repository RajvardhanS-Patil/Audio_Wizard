# Data Structures

All data structures (`Stack<T>`, `LinkedList<T>`, `Queue<T>`) are implemented genuinely using template-based C++ with full RAII node ownership and zero wrapper proxies.

## 1. Custom Generic Stack (`src/dsa/Stack.hpp`)

- **Representation:** Singly-linked node structure (`Node { T data; Node* next; }`).
- **Semantics:** LIFO (Last-In, First-Out).
- **Operations:** `push()`, `pop()`, `top()`, `peek()`, `isEmpty()`, `size()`, `clear()`, `toVector()`.
- **Gameplay Use:** Spell history recording and Undo spell (`[Q]`).

## 2. Custom Generic Linked List (`src/dsa/LinkedList.hpp`)

- **Representation:** Singly-linked list with head and tail pointers (`Node { T data; Node* next; }`).
- **Semantics:** Sequential node chain with full positional insertion/deletion and pointer reversal.
- **Operations:**
  - `insertFront(value)` / `insertBack(value)` / `insertAt(index, value)`: $O(1)$ front/back, $O(N)$ at index
  - `removeFront()` / `removeBack()` / `removeAt(index)`: $O(1)$ front, $O(N)$ back/index
  - `getAt(index)`: $O(N)$ element accessor
  - `find(value)` / `contains(value)`: $O(N)$ search
  - `reverse()`: In-place pointer reversal ($O(N)$ time, $O(1)$ extra space)
  - `size()` / `empty()` / `clear()` / `toVector()`: Container lifecycle
- **Gameplay Use:** Subterranean crystal chain repair puzzle and Chain Link ability (`[R]`).

## 3. Custom Generic Queue (`src/dsa/Queue.hpp`)

- **Representation:** Singly-linked list with front and back pointers (`Node { T data; Node* next; }`).
- **Semantics:** FIFO (First-In, First-Out).
- **Operations:**
  - `enqueue(value)`: $O(1)$ back insertion
  - `dequeue()`: $O(1)$ front deletion
  - `front()` / `back()`: $O(1)$ element inspection
  - `size()` / `empty()` / `clear()` / `toVector()`: Container lifecycle
- **Gameplay Use:** Echo Gate enemy wave queuing and Queue Surge wave pulse ability (`[T]`).

## 4. Binary Tree & Binary Search Tree (`src/dsa/structures/BinaryTree.hpp`, `src/dsa/structures/BinarySearchTree.hpp`)

- **Representation:** Node structure with left and right children (`TreeNode { T value; unique_ptr<TreeNode> left, right; }`).
- **Semantics:** Hierarchical node structure. Binary Search Tree enforces sorted ordering.
- **Operations:**
  - `insert(value)`: O(log N) average, O(N) worst case.
  - `search(value)` / `contains(value)`: O(log N) average, O(N) worst case.
  - `inorder()`, `preorder()`, `postorder()`, `levelOrder()`: O(N) traversals.
- **Gameplay Use:** Tree of Knowledge zone, Tree Guardian boss, Traversal Challenge.

## Unit Test Verification (`tests/TestRunner.cpp`)

Automated tests in `AudioWizardTests.exe` verify:
- `Stack<T>` LIFO ordering, peek, push/pop, deep copy, and move semantics.
- `LinkedList<T>` front/back/index insertion & removal, search, pointer reversal, and node traversal.
- `Queue<T>` FIFO ordering, enqueue/dequeue, front/back inspection, and memory cleanup.
- `BinaryTree<T>` and `BinarySearchTree<T>` correct insertion, correct properties, and all 4 traversal algorithms.