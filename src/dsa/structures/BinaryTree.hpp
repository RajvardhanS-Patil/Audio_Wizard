#pragma once

#include <memory>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include "../Queue.hpp"

template <typename T>
struct BTNode {
    T value;
    std::unique_ptr<BTNode<T>> left;
    std::unique_ptr<BTNode<T>> right;

    explicit BTNode(const T& val) : value(val), left(nullptr), right(nullptr) {}
};

template <typename T>
class BinaryTree {
private:
    std::unique_ptr<BTNode<T>> m_root;

    int heightRecursive(const BTNode<T>* node) const {
        if (!node) return 0;
        return 1 + std::max(heightRecursive(node->left.get()), heightRecursive(node->right.get()));
    }

    void preorderRecursive(const BTNode<T>* node, std::vector<T>& result) const {
        if (!node) return;
        result.push_back(node->value);
        preorderRecursive(node->left.get(), result);
        preorderRecursive(node->right.get(), result);
    }

    void inorderRecursive(const BTNode<T>* node, std::vector<T>& result) const {
        if (!node) return;
        inorderRecursive(node->left.get(), result);
        result.push_back(node->value);
        inorderRecursive(node->right.get(), result);
    }

    void postorderRecursive(const BTNode<T>* node, std::vector<T>& result) const {
        if (!node) return;
        postorderRecursive(node->left.get(), result);
        postorderRecursive(node->right.get(), result);
        result.push_back(node->value);
    }

    bool searchRecursive(const BTNode<T>* node, const T& value) const {
        if (!node) return false;
        if (node->value == value) return true;
        return searchRecursive(node->left.get(), value) || searchRecursive(node->right.get(), value);
    }

public:
    BinaryTree() : m_root(nullptr) {}

    void setRoot(const T& value) {
        m_root = std::make_unique<BTNode<T>>(value);
    }

    bool insertLeft(BTNode<T>* parent, const T& value) {
        if (!parent || parent->left) return false;
        parent->left = std::make_unique<BTNode<T>>(value);
        return true;
    }

    bool insertRight(BTNode<T>* parent, const T& value) {
        if (!parent || parent->right) return false;
        parent->right = std::make_unique<BTNode<T>>(value);
        return true;
    }

    BTNode<T>* getRoot() { return m_root.get(); }
    const BTNode<T>* getRoot() const { return m_root.get(); }

    bool search(const T& value) const {
        return searchRecursive(m_root.get(), value);
    }

    int height() const {
        return heightRecursive(m_root.get());
    }

    bool empty() const {
        return m_root == nullptr;
    }

    void clear() {
        m_root.reset();
    }

    std::vector<T> preorder() const {
        std::vector<T> result;
        preorderRecursive(m_root.get(), result);
        return result;
    }

    std::vector<T> inorder() const {
        std::vector<T> result;
        inorderRecursive(m_root.get(), result);
        return result;
    }

    std::vector<T> postorder() const {
        std::vector<T> result;
        postorderRecursive(m_root.get(), result);
        return result;
    }

    std::vector<T> levelOrder() const {
        std::vector<T> result;
        if (!m_root) return result;

        Queue<const BTNode<T>*> q;
        q.enqueue(m_root.get());

        while (!q.empty()) {
            const BTNode<T>* current = q.front();
            q.dequeue();

            result.push_back(current->value);

            if (current->left) q.enqueue(current->left.get());
            if (current->right) q.enqueue(current->right.get());
        }

        return result;
    }
};
