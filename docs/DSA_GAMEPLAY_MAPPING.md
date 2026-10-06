# DSA Gameplay Mapping

- **Arrays:** Crystal Inventory Magic
- **Linked Lists:** Crystal Chain Puzzle & Chain Link Ability (`[R]`)
- **Stacks:** Undo Spell & Spell History (`[Q]`)
- **Queues:** Echo Gate Wave Management & Queue Surge Ability (`[T]`)
- **Trees:** Skill Tree Abilities
- **Graphs:** Magical Pathfinding
- **Sorting:** Spell organization

## Phase 3 Implementation: The Stack (LIFO)

Phase 3 introduced **The Stack** integrated into combat and spellcasting (`Stack<T>`), allowing players to undo their latest spell cast (`[Q]`) under the Last-In, First-Out (LIFO) principle.

## Phase 4 Implementation: Linked Lists & Queues

Phase 4 introduces **Linked List** (`LinkedList<T>`) and **Queue** (`Queue<T>`) as real, interactive gameplay mechanics in **The Chained Cave**.

### 1. Linked List Gameplay (`LinkedList<T>`)

- **Subterranean Crystal Chain Puzzle:**
  - The ancient cave contains glowing crystal nodes connected sequentially (`Crystal A -> Crystal B -> Crystal C -> Crystal D`).
  - The player interacts with nodes (`[E]`) to insert new crystals (`insertBack`), disconnect crystals (`removeBack`), or reverse the chain direction (`reverse`).
  - Pointer updates are rendered visually on screen via `LinkedListVisualizer`.
- **Chain Link Ability (`Key R`):**
  - Uses `LinkedList<Vector2>` internally to build node coordinates connecting the Wizard to nearby enemies with an electric energy beam.
  - Deals continuous damage across all connected nodes in the chain.

### 2. Queue Gameplay (`Queue<T>`)

- **The Echo Gate Wave System:**
  - Enemies waiting to breach the cave are stored in a FIFO queue (`Queue<CaveEnemySpawn>`).
  - Enemies spawn sequentially: the first enemy queued is the first enemy dispatched (`dequeue`).
  - Displays queue state via `QueueVisualizer`:
    ```
    FRONT -> [Shadow Bat] -> [Stone Crawler] -> [Shadow Bat] -> REAR
    ```
- **Queue Surge Ability (`Key T`):**
  - Uses `Queue<SurgePulse>` to store arcane pulses and fire them sequentially in FIFO order, delivering devastating wave blasts.

## Phase 5 Implementation: Arrays & Sorting

Phase 5 introduced **Dynamic Arrays** (`DynamicArray<T>`) alongside **Sorting Algorithms** (Bubble, Merge, Quick) implemented as strategies, mapped to the **Sorting Arena** and magical sorting spells.

## Phase 6 Implementation: Trees (Binary & BST)

Phase 6 introduces **Binary Tree** (`BinaryTree<T>`) and **Binary Search Tree** (`BinarySearchTree<T>`) alongside their traversal algorithms in the **Tree of Knowledge**.

### 1. Tree Gameplay (`BinarySearchTree<T>`)

- **Tree of Knowledge Traversal Challenge:**
  - The Wizard approaches the ancient Tree of Knowledge and must traverse the underlying structure in **Inorder** (Left-Root-Right).
  - Activating nodes in the correct sorted order completes the traversal challenge and unlocks advanced Tree mastery.
- **Tree Guardian Boss:**
  - Defeating the guardian relies on mastering the tree structures to protect the realm.