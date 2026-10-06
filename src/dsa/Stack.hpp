#pragma once

#include <cstddef>
#include <stdexcept>
#include <utility>
#include <vector>

/**
 * @brief Custom generic LIFO (Last-In, First-Out) Stack implementation.
 * Encapsulates dynamic memory management and linked nodes without relying on std::stack.
 */
template <typename T>
class Stack {
private:
    struct Node {
        T data;
        Node* next{nullptr};

        Node(const T& value, Node* nextNode = nullptr)
            : data(value), next(nextNode) {}

        Node(T&& value, Node* nextNode = nullptr)
            : data(std::move(value)), next(nextNode) {}
    };

    Node* m_top{nullptr};
    size_t m_size{0};

public:
    Stack() : m_top(nullptr), m_size(0) {}

    ~Stack() {
        clear();
    }

    // Copy Constructor (Deep Copy)
    Stack(const Stack& other) : m_top(nullptr), m_size(0) {
        if (!other.m_top) return;

        // Traverse other nodes into temporary vector to preserve bottom-to-top order
        std::vector<const T*> temp;
        Node* curr = other.m_top;
        while (curr) {
            temp.push_back(&(curr->data));
            curr = curr->next;
        }

        // Push in reverse order so top remains identical
        for (auto it = temp.rbegin(); it != temp.rend(); ++it) {
            push(**it);
        }
    }

    // Copy Assignment Operator
    Stack& operator=(const Stack& other) {
        if (this != &other) {
            clear();
            if (other.m_top) {
                std::vector<const T*> temp;
                Node* curr = other.m_top;
                while (curr) {
                    temp.push_back(&(curr->data));
                    curr = curr->next;
                }
                for (auto it = temp.rbegin(); it != temp.rend(); ++it) {
                    push(**it);
                }
            }
        }
        return *this;
    }

    // Move Constructor
    Stack(Stack&& other) noexcept
        : m_top(other.m_top), m_size(other.m_size) {
        other.m_top = nullptr;
        other.m_size = 0;
    }

    // Move Assignment Operator
    Stack& operator=(Stack&& other) noexcept {
        if (this != &other) {
            clear();
            m_top = other.m_top;
            m_size = other.m_size;
            other.m_top = nullptr;
            other.m_size = 0;
        }
        return *this;
    }

    // Push by const reference
    void push(const T& value) {
        m_top = new Node(value, m_top);
        ++m_size;
    }

    // Push by rvalue reference (move)
    void push(T&& value) {
        m_top = new Node(std::move(value), m_top);
        ++m_size;
    }

    // Pop the top element
    void pop() {
        if (isEmpty()) {
            throw std::underflow_error("Cannot pop from an empty Stack.");
        }
        Node* temp = m_top;
        m_top = m_top->next;
        delete temp;
        --m_size;
    }

    // Peek top element (modifiable)
    T& top() {
        if (isEmpty()) {
            throw std::underflow_error("Stack is empty.");
        }
        return m_top->data;
    }

    // Peek top element (const)
    const T& top() const {
        if (isEmpty()) {
            throw std::underflow_error("Stack is empty.");
        }
        return m_top->data;
    }

    // Alias for top()
    T& peek() { return top(); }
    const T& peek() const { return top(); }

    bool isEmpty() const { return m_top == nullptr; }
    bool empty() const { return isEmpty(); }
    size_t size() const { return m_size; }

    void clear() {
        while (m_top) {
            Node* temp = m_top;
            m_top = m_top->next;
            delete temp;
        }
        m_size = 0;
    }

    // Inspector method for rendering/visualization without breaking encapsulation
    std::vector<T> toVector() const {
        std::vector<T> elements;
        elements.reserve(m_size);
        Node* curr = m_top;
        while (curr) {
            elements.push_back(curr->data);
            curr = curr->next;
        }
        return elements;
    }
};
