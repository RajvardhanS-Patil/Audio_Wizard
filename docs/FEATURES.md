# Features

- **Musical Audio Spell System:** Play keyboard notes (A, S, D, F, G, H, J = C, D, E, F, G, A, B) to cast spells.
- **Spell Recipes:**
  - Fire Spell (`C + F + G`)
  - Healing Spell (`C + E + G`)
  - Shield Spell (`D + F + A`)
  - Dark Attack (`A + C + E`)
- **DSA Stack Integration:** Real `Stack<T>` data structure powers spell history and `[Q]` Undo Spell action (LIFO pop).
- **Interactive Stack Visualizer:** Live HUD diagram showing stack frames, pop animations, and educational explanations.
- **Dynamic Combat System:** Projectiles, particle trails, floating damage text, and combo multipliers.
- **AI-Driven Enemies:** State Pattern AI for Corrupted Grove Beast, Shadow Bats, Stone Crawlers, Sorting Serpent, and Tree Guardian.
- **World & Quest System:** Multiple zones including The Corrupted Grove, Chained Cave, Sorting Arena, and Tree of Knowledge with their respective quests and guardians.
- **Leveling & Progression:** XP gain, level ups, passive mana regeneration, spell power scaling, and DSA Mastery UI.
- **DSA Linked List & Queue Integration (Phase 4):** Real `LinkedList<T>` and `Queue<T>` for the Chain Link spell, subterreanean crystal puzzle, Echo Gate wave management, and Queue Surge ability.
- **DSA Arrays & Sorting (Phase 5):** `DynamicArray<T>` with Bubble Sort, Merge Sort, and Quick Sort abilities visually and mechanically integrated in the Sorting Arena.
- **DSA Trees & BST (Phase 6):** `BinaryTree<T>` and `BinarySearchTree<T>` power the Tree of Knowledge and Traversal Challenge (Inorder, Preorder, Postorder, Level-order).
- **DSA Graph & BFS/DFS (Phase 7):** Generic `Graph<T>` representation using Adjacency List powers Graph City, where players restore corrupted edges and vertices to reconnect the city.
- **Decoupled Audio Architecture:** Procedural 16-bit PCM WAV assets for all notes, spells, ambience, and creature sounds.