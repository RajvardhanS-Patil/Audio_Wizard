#pragma once

#include <cstddef>
#include <stdexcept>
#include <utility>
#include <vector>

/**
 * @brief Custom generic FIFO (First-In, First-Out) Queue data structure.
 * Encapsulates dynamic node memory with O(1) enqueue and dequeue operations and RAII.
 */
template <typename T>
class Queue {
private:
    struct Node {
        T data;
        Node* next{nullptr};

        Node(const T& value, Node* nextNode = nullptr)
            : data(value), next(nextNode) {}

        Node(T&& value, Node* nextNode = nullptr)
            : data(std::move(value)), next(nextNode) {}
    };

    Node* m_front{nullptr};
    Node* m_back{nullptr};
    size_t m_size{0};

public:
    Queue() : m_front(nullptr), m_back(nullptr), m_size(0) {}

    ~Queue() {
        clear();
    }

    // Copy Constructor (Deep Copy)
    Queue(const Queue& other) : m_front(nullptr), m_back(nullptr), m_size(0) {
        Node* curr = other.m_front;
        while (curr) {
            enqueue(curr->data);
            curr = curr->next;
        }
    }

    // Copy Assignment Operator
    Queue& operator=(const Queue& other) {
        if (this != &other) {
            clear();
            Node* curr = other.m_front;
            while (curr) {
                enqueue(curr->data);
                curr = curr->next;
            }
        }
        return *this;
    }

    // Move Constructor
    Queue(Queue&& other) noexcept
        : m_front(other.m_front), m_back(other.m_back), m_size(other.m_size) {
        other.m_front = nullptr;
        other.m_back = nullptr;
        other.m_size = 0;
    }

    // Move Assignment Operator
    Queue& operator=(Queue&& other) noexcept {
        if (this != &other) {
            clear();
            m_front = other.m_front;
            m_back = other.m_back;
            m_size = other.m_size;
            other.m_front = nullptr;
            other.m_back = nullptr;
            other.m_size = 0;
        }
        return *this;
    }

    // Enqueue at the back (FIFO)
    void enqueue(const T& value) {
        Node* newNode = new Node(value, nullptr);
        if (!m_front) {
            m_front = m_back = newNode;
        } else {
            m_back->next = newNode;
            m_back = newNode;
        }
        ++m_size;
    }

    void enqueue(T&& value) {
        Node* newNode = new Node(std::move(value), nullptr);
        if (!m_front) {
            m_front = m_back = newNode;
        } else {
            m_back->next = newNode;
            m_back = newNode;
        }
        ++m_size;
    }

    // Dequeue from the front (FIFO)
    void dequeue() {
        if (empty()) {
            throw std::underflow_error("Cannot dequeue from an empty Queue.");
        }
        Node* temp = m_front;
        m_front = m_front->next;
        delete temp;
        --m_size;
        if (m_size == 0) {
            m_back = nullptr;
        }
    }

    // Inspect front element
    T& front() {
        if (empty()) {
            throw std::underflow_error("Queue is empty.");
        }
        return m_front->data;
    }

    const T& front() const {
        if (empty()) {
            throw std::underflow_error("Queue is empty.");
        }
        return m_front->data;
    }

    // Inspect back element
    T& back() {
        if (empty()) {
            throw std::underflow_error("Queue is empty.");
        }
        return m_back->data;
    }

    const T& back() const {
        if (empty()) {
            throw std::underflow_error("Queue is empty.");
        }
        return m_back->data;
    }

    size_t size() const { return m_size; }
    bool empty() const { return m_size == 0; }
    bool isEmpty() const { return empty(); }

    void clear() {
        while (m_front) {
            Node* temp = m_front;
            m_front = m_front->next;
            delete temp;
        }
        m_back = nullptr;
        m_size = 0;
    }

    std::vector<T> toVector() const {
        std::vector<T> result;
        result.reserve(m_size);
        Node* curr = m_front;
        while (curr) {
            result.push_back(curr->data);
            curr = curr->next;
        }
        return result;
    }
};
