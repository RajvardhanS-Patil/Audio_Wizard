#include <iostream>
#include <cassert>
#include <string>
#include <vector>

#include "../src/dsa/Stack.hpp"
#include "../src/dsa/LinkedList.hpp"
#include "../src/dsa/Queue.hpp"
#include "../src/spells/AudioNote.hpp"
#include "../src/spells/SpellCombination.hpp"
#include "../src/spells/SpellManager.hpp"
#include "../src/spells/FireSpell.hpp"
#include "../src/spells/HealingSpell.hpp"
#include "../src/spells/ShieldSpell.hpp"
#include "../src/spells/DarkAttackSpell.hpp"
#include "../src/combat/DamageSystem.hpp"
#include "../src/player/PlayerStats.hpp"

void testStack() {
    std::cout << "[TEST] Running Stack<T> Tests..." << std::endl;

    Stack<int> s;
    assert(s.isEmpty());
    assert(s.empty());
    assert(s.size() == 0);

    // Push test
    s.push(10);
    s.push(20);
    s.push(30);
    assert(!s.isEmpty());
    assert(s.size() == 3);

    // Peek / Top test
    assert(s.peek() == 30);
    assert(s.top() == 30);

    // Pop test (LIFO order)
    s.pop();
    assert(s.size() == 2);
    assert(s.peek() == 20);

    s.pop();
    assert(s.size() == 1);
    assert(s.peek() == 10);

    s.pop();
    assert(s.isEmpty());
    assert(s.size() == 0);

    // Copy Constructor & Deep Copy
    s.push(100);
    s.push(200);
    s.push(300);
    Stack<int> sCopy = s;
    assert(sCopy.size() == 3);
    assert(sCopy.top() == 300);
    sCopy.pop();
    assert(sCopy.top() == 200);
    assert(s.top() == 300); // Original unmodified

    // Move constructor
    Stack<int> sMoved = std::move(sCopy);
    assert(sMoved.size() == 2);
    assert(sMoved.top() == 200);

    // Clear test
    sMoved.clear();
    assert(sMoved.isEmpty());

    std::cout << "[PASS] Stack<T> Tests Passed!" << std::endl;
}

void testLinkedList() {
    std::cout << "[TEST] Running LinkedList<T> Tests..." << std::endl;

    LinkedList<std::string> list;
    assert(list.empty());
    assert(list.size() == 0);

    // 1. Insert Front
    list.insertFront("Crystal A");
    assert(list.size() == 1);
    assert(list.getAt(0) == "Crystal A");

    // 2. Insert Back
    list.insertBack("Crystal C");
    assert(list.size() == 2);
    assert(list.getAt(1) == "Crystal C");

    // 3. Insert At index 1
    list.insertAt(1, "Crystal B");
    assert(list.size() == 3);
    assert(list.getAt(0) == "Crystal A");
    assert(list.getAt(1) == "Crystal B");
    assert(list.getAt(2) == "Crystal C");

    // 4. Find & Contains
    assert(list.contains("Crystal B"));
    assert(list.find("Crystal B") == 1);
    assert(list.find("NonExistent") == -1);

    // 5. Reverse
    list.reverse();
    assert(list.getAt(0) == "Crystal C");
    assert(list.getAt(1) == "Crystal B");
    assert(list.getAt(2) == "Crystal A");

    // 6. Remove At index 1
    list.removeAt(1); // removes Crystal B
    assert(list.size() == 2);
    assert(list.getAt(0) == "Crystal C");
    assert(list.getAt(1) == "Crystal A");

    // 7. Remove Front & Remove Back
    list.removeFront();
    assert(list.size() == 1);
    assert(list.getAt(0) == "Crystal A");

    list.removeBack();
    assert(list.empty());

    // 8. Copy Constructor & Deep Copy
    list.insertBack("Alpha");
    list.insertBack("Beta");
    LinkedList<std::string> copyList = list;
    assert(copyList.size() == 2);
    assert(copyList.getAt(0) == "Alpha");
    copyList.removeFront();
    assert(copyList.size() == 1);
    assert(list.size() == 2); // Original intact

    // 9. Move Constructor
    LinkedList<std::string> movedList = std::move(copyList);
    assert(movedList.size() == 1);
    assert(movedList.getAt(0) == "Beta");

    std::cout << "[PASS] LinkedList<T> Tests Passed!" << std::endl;
}

void testQueue() {
    std::cout << "[TEST] Running Queue<T> Tests..." << std::endl;

    Queue<std::string> q;
    assert(q.empty());
    assert(q.size() == 0);

    // 1. Enqueue (FIFO Order)
    q.enqueue("Enemy 1: Shadow Bat");
    q.enqueue("Enemy 2: Stone Crawler");
    q.enqueue("Enemy 3: Shadow Bat");
    assert(q.size() == 3);

    // 2. Front & Back inspection
    assert(q.front() == "Enemy 1: Shadow Bat");
    assert(q.back() == "Enemy 3: Shadow Bat");

    // 3. Dequeue in strict FIFO order
    assert(q.front() == "Enemy 1: Shadow Bat");
    q.dequeue();
    assert(q.size() == 2);
    assert(q.front() == "Enemy 2: Stone Crawler");

    assert(q.front() == "Enemy 2: Stone Crawler");
    q.dequeue();
    assert(q.size() == 1);
    assert(q.front() == "Enemy 3: Shadow Bat");

    assert(q.front() == "Enemy 3: Shadow Bat");
    q.dequeue();
    assert(q.empty());
    assert(q.size() == 0);

    // 4. Copy & Move
    q.enqueue("Task 1");
    q.enqueue("Task 2");
    Queue<std::string> qCopy = q;
    assert(qCopy.size() == 2);
    assert(qCopy.front() == "Task 1");
    qCopy.dequeue();
    assert(q.front() == "Task 1"); // Original untouched

    Queue<std::string> qMoved = std::move(qCopy);
    assert(qMoved.size() == 1);
    assert(qMoved.front() == "Task 2");

    std::cout << "[PASS] Queue<T> Tests Passed!" << std::endl;
}

void testSpellMechanics() {
    std::cout << "[TEST] Running Spell Mechanics & Recognition Tests..." << std::endl;

    SpellManager manager;

    // 1. Valid Combination Recognition: C + F + G -> Fire Spell
    std::vector<NotePitch> fireSeq = {NotePitch::C, NotePitch::F, NotePitch::G};
    Spell* fire = manager.findMatchingSpell(fireSeq);
    assert(fire != nullptr);
    assert(fire->getName() == "Fire Spell");
    assert(fire->getManaCost() == 20);

    // 2. Valid Combination Recognition: C + E + G -> Healing Spell
    std::vector<NotePitch> healSeq = {NotePitch::C, NotePitch::E, NotePitch::G};
    Spell* heal = manager.findMatchingSpell(healSeq);
    assert(heal != nullptr);
    assert(heal->getName() == "Healing Spell");
    assert(heal->getManaCost() == 15);

    // 3. Valid Combination Recognition: D + F + A -> Shield Spell
    std::vector<NotePitch> shieldSeq = {NotePitch::D, NotePitch::F, NotePitch::A};
    Spell* shield = manager.findMatchingSpell(shieldSeq);
    assert(shield != nullptr);
    assert(shield->getName() == "Shield Spell");
    assert(shield->getManaCost() == 10);

    // 4. Valid Combination Recognition: A + C + E -> Dark Attack
    std::vector<NotePitch> darkSeq = {NotePitch::A, NotePitch::C, NotePitch::E};
    Spell* dark = manager.findMatchingSpell(darkSeq);
    assert(dark != nullptr);
    assert(dark->getName() == "Dark Attack");
    assert(dark->getManaCost() == 25);

    // 5. Invalid Sequence Recognition: C + D + E
    std::vector<NotePitch> invalidSeq = {NotePitch::C, NotePitch::D, NotePitch::E};
    Spell* none = manager.findMatchingSpell(invalidSeq);
    assert(none == nullptr);

    // 6. Mana Check
    PlayerStats stats;
    assert(fire->canCast(stats)); // 100 mana >= 20

    stats.consumeMana(90); // 10 mana remaining
    assert(!fire->canCast(stats)); // 10 mana < 20
    assert(shield->canCast(stats)); // 10 mana >= 10

    // 7. Cooldown Check
    stats.restoreMana(100);
    assert(fire->isReady());
    fire->triggerCooldown();
    assert(!fire->isReady());
    assert(!fire->canCast(stats)); // cannot cast while on cooldown

    fire->updateCooldown(1.0f);
    assert(!fire->isReady()); // 1.5s cooldown - 1.0s = 0.5s left
    fire->updateCooldown(0.6f);
    assert(fire->isReady()); // now ready
    assert(fire->canCast(stats));

    std::cout << "[PASS] Spell Mechanics & Recognition Tests Passed!" << std::endl;
}

void testCombatAndProgression() {
    std::cout << "[TEST] Running Combat & Progression Tests..." << std::endl;

    // 1. Damage calculation with defense mitigation
    int rawDamage = 40;
    float spellPower = 1.0f;
    float comboMult = 1.2f;
    int defense = 10;
    int dmg = DamageSystem::calculateDamage(rawDamage, spellPower, comboMult, defense);
    assert(dmg > 0);
    assert(dmg < 48); // Mitigated by defense

    // 2. Healing calculation
    int healing = DamageSystem::calculateHealing(30, 1.2f);
    assert(healing == 36);

    // 3. Player Stats & Level Up progression
    PlayerStats stats;
    assert(stats.getLevel() == 1);
    assert(stats.getXP() == 0);
    int startingMaxHp = stats.getMaxHealth();

    stats.addExperience(50);
    assert(stats.getXP() == 50);
    assert(stats.getLevel() == 1);

    // Add 100 XP -> Level Up!
    stats.addExperience(50);
    assert(stats.getLevel() == 2);
    assert(stats.getMaxHealth() > startingMaxHp);

    std::cout << "[PASS] Combat & Progression Tests Passed!" << std::endl;
}

#include "../src/dsa/SortingAlgorithms.hpp"
#include "../src/dsa/visualization/SortingVisualizer.hpp"

void testSortingAlgorithms() {
    std::cout << "[TEST] Running Sorting Algorithms & Phase 5 Tests..." << std::endl;

    BubbleSort bubble;
    MergeSort merge;
    QuickSort quick;

    // Helper lambda to check if array is in non-decreasing order
    auto isSorted = [](const DynamicArray<int>& arr) {
        for (size_t i = 1; i < arr.size(); ++i) {
            if (arr[i - 1] > arr[i]) return false;
        }
        return true;
    };

    // 1. BUBBLE SORT TESTS
    {
        DynamicArray<int> emptyArr;
        bubble.sort(emptyArr);
        assert(emptyArr.empty());

        DynamicArray<int> singleArr = {42};
        bubble.sort(singleArr);
        assert(singleArr.size() == 1 && singleArr[0] == 42);

        DynamicArray<int> sortedArr = {10, 20, 30, 40, 50};
        bubble.sort(sortedArr);
        assert(isSorted(sortedArr));

        DynamicArray<int> reverseArr = {50, 40, 30, 20, 10};
        bubble.sort(reverseArr);
        assert(isSorted(reverseArr));

        DynamicArray<int> dupsArr = {30, 10, 30, 20, 10, 50};
        bubble.sort(dupsArr);
        assert(isSorted(dupsArr));

        DynamicArray<int> normalArr = {35, 12, 48, 7, 26};
        bubble.sort(normalArr);
        assert(isSorted(normalArr));

        auto steps = bubble.generateSteps(DynamicArray<int>{40, 10, 30, 20});
        assert(!steps.empty());
        assert(steps.back().type == SortingStepType::SORTED);
    }

    // 2. MERGE SORT TESTS
    {
        DynamicArray<int> emptyArr;
        merge.sort(emptyArr);
        assert(emptyArr.empty());

        DynamicArray<int> singleArr = {99};
        merge.sort(singleArr);
        assert(singleArr.size() == 1 && singleArr[0] == 99);

        DynamicArray<int> sortedArr = {1, 2, 3, 4, 5};
        merge.sort(sortedArr);
        assert(isSorted(sortedArr));

        DynamicArray<int> reverseArr = {90, 70, 50, 30, 10};
        merge.sort(reverseArr);
        assert(isSorted(reverseArr));

        DynamicArray<int> dupsArr = {5, 1, 5, 2, 1, 9};
        merge.sort(dupsArr);
        assert(isSorted(dupsArr));

        DynamicArray<int> normalArr = {40, 10, 30, 20};
        merge.sort(normalArr);
        assert(isSorted(normalArr));

        auto steps = merge.generateSteps(DynamicArray<int>{40, 10, 30, 20});
        assert(!steps.empty());
        assert(steps.back().type == SortingStepType::SORTED);
    }

    // 3. QUICK SORT TESTS
    {
        DynamicArray<int> emptyArr;
        quick.sort(emptyArr);
        assert(emptyArr.empty());

        DynamicArray<int> singleArr = {77};
        quick.sort(singleArr);
        assert(singleArr.size() == 1 && singleArr[0] == 77);

        DynamicArray<int> sortedArr = {10, 20, 30, 40, 50};
        quick.sort(sortedArr);
        assert(isSorted(sortedArr));

        DynamicArray<int> reverseArr = {100, 80, 60, 40, 20};
        quick.sort(reverseArr);
        assert(isSorted(reverseArr));

        DynamicArray<int> dupsArr = {4, 2, 4, 1, 2, 8};
        quick.sort(dupsArr);
        assert(isSorted(dupsArr));

        DynamicArray<int> normalArr = {50, 20, 80, 10, 40};
        quick.sort(normalArr);
        assert(isSorted(normalArr));

        auto steps = quick.generateSteps(DynamicArray<int>{50, 20, 80, 10, 40});
        assert(!steps.empty());
        assert(steps.back().type == SortingStepType::SORTED);
    }

    // 4. SORTING VISUALIZER STEP INTEGRATION
    {
        SortingVisualizer visualizer;
        DynamicArray<int> testArr = {35, 12, 48, 7, 26};
        visualizer.setArray(testArr, bubble);
        assert(visualizer.getTotalSteps() > 0);
        assert(visualizer.getSpeed() == VisualizerSpeed::NORMAL);
        visualizer.cycleSpeed();
        assert(visualizer.getSpeed() == VisualizerSpeed::FAST);
    }

    std::cout << "[PASS] Sorting Algorithms & Phase 5 Tests Passed!" << std::endl;
}

#include "../src/dsa/structures/BinarySearchTree.hpp"
#include "../src/dsa/structures/BinaryTree.hpp"
#include "../src/dsa/structures/Graph.hpp"

void testTrees() {
    std::cout << "[TEST] Running Tree & Phase 6 Tests..." << std::endl;

    // 1. Binary Tree (Level-order insertion and traversals)
    {
        BinaryTree<int> bt;
        assert(bt.empty());
        bt.setRoot(1);
        auto* root = bt.getRoot();
        bt.insertLeft(root, 2);
        bt.insertRight(root, 3);
        bt.insertLeft(root->left.get(), 4);
        bt.insertRight(root->left.get(), 5);
        assert(!bt.empty());

        std::vector<int> expectedInorder = {4, 2, 5, 1, 3};
        std::vector<int> actualInorder = bt.inorder();
        assert(expectedInorder == actualInorder);

        std::vector<int> expectedPreorder = {1, 2, 4, 5, 3};
        std::vector<int> actualPreorder = bt.preorder();
        assert(expectedPreorder == actualPreorder);

        std::vector<int> expectedPostorder = {4, 5, 2, 3, 1};
        std::vector<int> actualPostorder = bt.postorder();
        assert(expectedPostorder == actualPostorder);

        std::vector<int> expectedLevel = {1, 2, 3, 4, 5};
        std::vector<int> actualLevel = bt.levelOrder();
        assert(expectedLevel == actualLevel);
        
        bt.clear();
        assert(bt.empty());
    }

    // 2. Binary Search Tree
    {
        BinarySearchTree<int> bst;
        assert(bst.empty());

        bst.insert(50);
        bst.insert(30);
        bst.insert(70);
        bst.insert(20);
        bst.insert(40);
        bst.insert(60);
        bst.insert(80);

        assert(!bst.empty());

        // Inorder for BST should be sorted
        std::vector<int> expectedInorder = {20, 30, 40, 50, 60, 70, 80};
        std::vector<int> actualInorder = bst.inorder();
        assert(expectedInorder == actualInorder);

        assert(bst.contains(40) == true);
        assert(bst.contains(90) == false);
        
        bst.clear();
        assert(bst.empty());
    }

    std::cout << "[PASS] Tree & Phase 6 Tests Passed!" << std::endl;
}

void testGraph() {
    std::cout << "[TEST] Running Graph & Phase 7 Tests..." << std::endl;

    // Undirected Graph Test
    {
        Graph<std::string> g(false);
        assert(g.empty());
        assert(g.vertexCount() == 0);
        
        g.addVertex("Castle");
        g.addVertex("Forest");
        g.addEdge("Castle", "Forest");
        
        assert(!g.empty());
        assert(g.vertexCount() == 2);
        assert(g.edgeCount() == 1);
        assert(g.hasEdge("Castle", "Forest"));
        assert(g.hasEdge("Forest", "Castle"));
        
        g.addEdge("Forest", "Cave");
        g.addEdge("Cave", "Market");
        g.addEdge("Market", "Library");
        g.addEdge("Library", "Tower");
        g.addEdge("Castle", "Library");

        assert(g.vertexCount() == 6);
        assert(g.edgeCount() == 6);
        
        // BFS Test from Castle
        std::vector<std::string> bfsOrder = g.bfs("Castle");
        assert(bfsOrder.size() == 6);
        assert(bfsOrder[0] == "Castle");
        
        // DFS Test from Castle
        std::vector<std::string> dfsOrder = g.dfs("Castle");
        assert(dfsOrder.size() == 6);
        assert(dfsOrder[0] == "Castle");
    }

    // Directed Graph Test
    {
        Graph<int> dg(true);
        dg.addEdge(1, 2);
        dg.addEdge(2, 3);
        dg.addEdge(1, 3);

        assert(dg.vertexCount() == 3);
        assert(dg.edgeCount() == 3);
        assert(dg.hasEdge(1, 2));
        assert(!dg.hasEdge(2, 1));
        
        std::vector<int> bfsOrder = dg.bfs(1);
        assert(bfsOrder.size() == 3);
    }

    std::cout << "[PASS] Graph & Phase 7 Tests Passed!" << std::endl;
}

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "AUDIO WIZARD - PHASE 5 TEST SUITE" << std::endl;
    std::cout << "========================================" << std::endl;

    try {
        testStack();
        testLinkedList();
        testQueue();
        testSpellMechanics();
        testCombatAndProgression();
        testSortingAlgorithms();
        testTrees();
        testGraph();
        std::cout << "========================================" << std::endl;
        std::cout << "ALL PHASE 7 TESTS COMPLETED SUCCESSFULLY!" << std::endl;
        std::cout << "========================================" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "[TEST FAILURE] Exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

