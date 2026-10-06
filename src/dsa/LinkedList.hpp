#pragma once

#include <cstddef>
#include <stdexcept>
#include <utility>
#include <vector>

/**
 * @brief Custom generic Singly-Linked List data structure.
 * Fully encapsulated node management with pointer reversal, positional insertions/deletions, and RAII.
 */
template <typename T>
class LinkedList {
private:
    struct Node {
        T data;
        Node* next{nullptr};

        Node(const T& value, Node* nextNode = nullptr)
            : data(value), next(nextNode) {}

        Node(T&& value, Node* nextNode = nullptr)
            : data(std::move(value)), next(nextNode) {}
    };

    Node* m_head{nullptr};
    Node* m_tail{nullptr};
    size_t m_size{0};

public:
    LinkedList() : m_head(nullptr), m_tail(nullptr), m_size(0) {}

    ~LinkedList() {
        clear();
    }

    // Copy Constructor (Deep Copy)
    LinkedList(const LinkedList& other) : m_head(nullptr), m_tail(nullptr), m_size(0) {
        Node* curr = other.m_head;
        while (curr) {
            insertBack(curr->data);
            curr = curr->next;
        }
    }

    // Copy Assignment Operator
    LinkedList& operator=(const LinkedList& other) {
        if (this != &other) {
            clear();
            Node* curr = other.m_head;
            while (curr) {
                insertBack(curr->data);
                curr = curr->next;
            }
        }
        return *this;
    }

    // Move Constructor
    LinkedList(LinkedList&& other) noexcept
        : m_head(other.m_head), m_tail(other.m_tail), m_size(other.m_size) {
        other.m_head = nullptr;
        other.m_tail = nullptr;
        other.m_size = 0;
    }

    // Move Assignment Operator
    LinkedList& operator=(LinkedList&& other) noexcept {
        if (this != &other) {
            clear();
            m_head = other.m_head;
            m_tail = other.m_tail;
            m_size = other.m_size;
            other.m_head = nullptr;
            other.m_tail = nullptr;
            other.m_size = 0;
        }
        return *this;
    }

    void insertFront(const T& value) {
        Node* newNode = new Node(value, m_head);
        m_head = newNode;
        if (!m_tail) {
            m_tail = newNode;
        }
        ++m_size;
    }

    void insertFront(T&& value) {
        Node* newNode = new Node(std::move(value), m_head);
        m_head = newNode;
        if (!m_tail) {
            m_tail = newNode;
        }
        ++m_size;
    }

    void insertBack(const T& value) {
        Node* newNode = new Node(value, nullptr);
        if (!m_head) {
            m_head = m_tail = newNode;
        } else {
            m_tail->next = newNode;
            m_tail = newNode;
        }
        ++m_size;
    }

    void insertBack(T&& value) {
        Node* newNode = new Node(std::move(value), nullptr);
        if (!m_head) {
            m_head = m_tail = newNode;
        } else {
            m_tail->next = newNode;
            m_tail = newNode;
        }
        ++m_size;
    }

    void insertAt(size_t index, const T& value) {
        if (index > m_size) {
            throw std::out_of_range("Index out of bounds in LinkedList::insertAt");
        }
        if (index == 0) {
            insertFront(value);
            return;
        }
        if (index == m_size) {
            insertBack(value);
            return;
        }

        Node* prev = m_head;
        for (size_t i = 0; i < index - 1; ++i) {
            prev = prev->next;
        }
        Node* newNode = new Node(value, prev->next);
        prev->next = newNode;
        ++m_size;
    }

    void removeFront() {
        if (empty()) {
            throw std::underflow_error("Cannot remove from empty LinkedList");
        }
        Node* temp = m_head;
        m_head = m_head->next;
        delete temp;
        --m_size;
        if (m_size == 0) {
            m_tail = nullptr;
        }
    }

    void removeBack() {
        if (empty()) {
            throw std::underflow_error("Cannot remove from empty LinkedList");
        }
        if (m_size == 1) {
            delete m_head;
            m_head = m_tail = nullptr;
            m_size = 0;
            return;
        }

        Node* curr = m_head;
        while (curr->next != m_tail) {
            curr = curr->next;
        }
        delete m_tail;
        m_tail = curr;
        m_tail->next = nullptr;
        --m_size;
    }

    void removeAt(size_t index) {
        if (index >= m_size) {
            throw std::out_of_range("Index out of bounds in LinkedList::removeAt");
        }
        if (index == 0) {
            removeFront();
            return;
        }
        if (index == m_size - 1) {
            removeBack();
            return;
        }

        Node* prev = m_head;
        for (size_t i = 0; i < index - 1; ++i) {
            prev = prev->next;
        }
        Node* toDelete = prev->next;
        prev->next = toDelete->next;
        delete toDelete;
        --m_size;
    }

    T& getAt(size_t index) {
        if (index >= m_size) {
            throw std::out_of_range("Index out of bounds in LinkedList::getAt");
        }
        Node* curr = m_head;
        for (size_t i = 0; i < index; ++i) {
            curr = curr->next;
        }
        return curr->data;
    }

    const T& getAt(size_t index) const {
        if (index >= m_size) {
            throw std::out_of_range("Index out of bounds in LinkedList::getAt");
        }
        Node* curr = m_head;
        for (size_t i = 0; i < index; ++i) {
            curr = curr->next;
        }
        return curr->data;
    }

    int find(const T& value) const {
        Node* curr = m_head;
        int idx = 0;
        while (curr) {
            if (curr->data == value) {
                return idx;
            }
            curr = curr->next;
            ++idx;
        }
        return -1;
    }

    bool contains(const T& value) const {
        return find(value) != -1;
    }

    // In-place pointer reversal
    void reverse() {
        if (m_size <= 1) return;

        Node* prev = nullptr;
        Node* curr = m_head;
        Node* nextNode = nullptr;

        m_tail = m_head; // Original head becomes new tail

        while (curr) {
            nextNode = curr->next;
            curr->next = prev;
            prev = curr;
            curr = nextNode;
        }

        m_head = prev; // Prev is now the new head
    }

    size_t size() const { return m_size; }
    bool empty() const { return m_size == 0; }
    bool isEmpty() const { return empty(); }

    void clear() {
        while (m_head) {
            Node* temp = m_head;
            m_head = m_head->next;
            delete temp;
        }
        m_tail = nullptr;
        m_size = 0;
    }

    std::vector<T> toVector() const {
        std::vector<T> result;
        result.reserve(m_size);
        Node* curr = m_head;
        while (curr) {
            result.push_back(curr->data);
            curr = curr->next;
        }
        return result;
    }
};
