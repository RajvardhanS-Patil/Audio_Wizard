#pragma once

#include <memory>
#include <vector>
#include <algorithm>
#include <functional>
#include "../Queue.hpp"

template <typename T>
struct TreeNode {
    T value;
    std::unique_ptr<TreeNode<T>> left;
    std::unique_ptr<TreeNode<T>> right;

    explicit TreeNode(const T& val) : value(val), left(nullptr), right(nullptr) {}
};

template <typename T>
class BinarySearchTree {
private:
    std::unique_ptr<TreeNode<T>> m_root;

    void insertRecursive(std::unique_ptr<TreeNode<T>>& node, const T& value) {
        if (!node) {
            node = std::make_unique<TreeNode<T>>(value);
            return;
        }
        if (value < node->value) {
            insertRecursive(node->left, value);
        } else if (value > node->value) {
            insertRecursive(node->right, value);
        }
        // Ignores duplicates
    }

    bool searchRecursive(const TreeNode<T>* node, const T& value) const {
        if (!node) return false;
        if (node->value == value) return true;
        if (value < node->value) return searchRecursive(node->left.get(), value);
        return searchRecursive(node->right.get(), value);
    }

    int heightRecursive(const TreeNode<T>* node) const {
        if (!node) return 0;
        return 1 + std::max(heightRecursive(node->left.get()), heightRecursive(node->right.get()));
    }

    std::unique_ptr<TreeNode<T>> removeRecursive(std::unique_ptr<TreeNode<T>>& node, const T& value) {
        if (!node) return nullptr;

        if (value < node->value) {
            node->left = removeRecursive(node->left, value);
        } else if (value > node->value) {
            node->right = removeRecursive(node->right, value);
        } else {
            // Node found
            if (!node->left) {
                return std::move(node->right);
            } else if (!node->right) {
                return std::move(node->left);
            }

            // Node with two children
            TreeNode<T>* minNode = node->right.get();
            while (minNode->left) {
                minNode = minNode->left.get();
            }
            node->value = minNode->value;
            node->right = removeRecursive(node->right, minNode->value);
        }
        return std::move(node);
    }

    void preorderRecursive(const TreeNode<T>* node, std::vector<T>& result) const {
        if (!node) return;
        result.push_back(node->value);
        preorderRecursive(node->left.get(), result);
        preorderRecursive(node->right.get(), result);
    }

    void inorderRecursive(const TreeNode<T>* node, std::vector<T>& result) const {
        if (!node) return;
        inorderRecursive(node->left.get(), result);
        result.push_back(node->value);
        inorderRecursive(node->right.get(), result);
    }

    void postorderRecursive(const TreeNode<T>* node, std::vector<T>& result) const {
        if (!node) return;
        postorderRecursive(node->left.get(), result);
        postorderRecursive(node->right.get(), result);
        result.push_back(node->value);
    }

public:
    BinarySearchTree() : m_root(nullptr) {}

    void insert(const T& value) {
        insertRecursive(m_root, value);
    }

    bool search(const T& value) const {
        return searchRecursive(m_root.get(), value);
    }

    bool contains(const T& value) const {
        return search(value);
    }

    void remove(const T& value) {
        m_root = removeRecursive(m_root, value);
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

        Queue<const TreeNode<T>*> q;
        q.enqueue(m_root.get());

        while (!q.empty()) {
            const TreeNode<T>* current = q.front();
            q.dequeue();

            result.push_back(current->value);

            if (current->left) q.enqueue(current->left.get());
            if (current->right) q.enqueue(current->right.get());
        }

        return result;
    }

    // Expose root for visualization purposes (read-only)
    const TreeNode<T>* getRoot() const {
        return m_root.get();
    }
};
