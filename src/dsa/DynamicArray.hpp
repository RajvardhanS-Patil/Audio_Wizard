#pragma once

#include <stdexcept>
#include <initializer_list>
#include <utility>
#include <vector>

template <typename T>
class DynamicArray {
public:
    DynamicArray() : m_capacity(4), m_size(0) {
        m_data = new T[m_capacity];
    }

    explicit DynamicArray(size_t initialCapacity) : m_capacity(initialCapacity > 0 ? initialCapacity : 4), m_size(0) {
        m_data = new T[m_capacity];
    }

    DynamicArray(std::initializer_list<T> list) : m_capacity(list.size() > 0 ? list.size() * 2 : 4), m_size(0) {
        m_data = new T[m_capacity];
        for (const auto& item : list) {
            push_back(item);
        }
    }

    DynamicArray(const DynamicArray& other) : m_capacity(other.m_capacity), m_size(other.m_size) {
        m_data = new T[m_capacity];
        for (size_t i = 0; i < m_size; ++i) {
            m_data[i] = other.m_data[i];
        }
    }

    DynamicArray& operator=(const DynamicArray& other) {
        if (this != &other) {
            delete[] m_data;
            m_capacity = other.m_capacity;
            m_size = other.m_size;
            m_data = new T[m_capacity];
            for (size_t i = 0; i < m_size; ++i) {
                m_data[i] = other.m_data[i];
            }
        }
        return *this;
    }

    DynamicArray(DynamicArray&& other) noexcept : m_data(other.m_data), m_capacity(other.m_capacity), m_size(other.m_size) {
        other.m_data = nullptr;
        other.m_capacity = 0;
        other.m_size = 0;
    }

    DynamicArray& operator=(DynamicArray&& other) noexcept {
        if (this != &other) {
            delete[] m_data;
            m_data = other.m_data;
            m_capacity = other.m_capacity;
            m_size = other.m_size;
            other.m_data = nullptr;
            other.m_capacity = 0;
            other.m_size = 0;
        }
        return *this;
    }

    ~DynamicArray() {
        delete[] m_data;
    }

    T& access(size_t index) {
        if (index >= m_size) {
            throw std::out_of_range("DynamicArray index out of bounds");
        }
        return m_data[index];
    }

    const T& access(size_t index) const {
        if (index >= m_size) {
            throw std::out_of_range("DynamicArray index out of bounds");
        }
        return m_data[index];
    }

    T& operator[](size_t index) { return access(index); }
    const T& operator[](size_t index) const { return access(index); }

    void update(size_t index, const T& value) {
        if (index >= m_size) {
            throw std::out_of_range("DynamicArray update index out of bounds");
        }
        m_data[index] = value;
    }

    void push_back(const T& value) {
        if (m_size >= m_capacity) {
            resize(m_capacity * 2);
        }
        m_data[m_size++] = value;
    }

    void insert(size_t index, const T& value) {
        if (index > m_size) {
            throw std::out_of_range("DynamicArray insert index out of bounds");
        }
        if (m_size >= m_capacity) {
            resize(m_capacity * 2);
        }
        for (size_t i = m_size; i > index; --i) {
            m_data[i] = m_data[i - 1];
        }
        m_data[index] = value;
        m_size++;
    }

    void remove(size_t index) {
        if (index >= m_size) {
            throw std::out_of_range("DynamicArray remove index out of bounds");
        }
        for (size_t i = index; i < m_size - 1; ++i) {
            m_data[i] = m_data[i + 1];
        }
        m_size--;
    }

    void pop_back() {
        if (m_size == 0) {
            throw std::out_of_range("DynamicArray pop_back on empty array");
        }
        m_size--;
    }

    void swap(size_t i, size_t j) {
        if (i >= m_size || j >= m_size) {
            throw std::out_of_range("DynamicArray swap indices out of bounds");
        }
        std::swap(m_data[i], m_data[j]);
    }

    size_t size() const { return m_size; }
    size_t capacity() const { return m_capacity; }
    bool empty() const { return m_size == 0; }

    void clear() {
        m_size = 0;
    }

    std::vector<T> toVector() const {
        std::vector<T> vec;
        vec.reserve(m_size);
        for (size_t i = 0; i < m_size; ++i) {
            vec.push_back(m_data[i]);
        }
        return vec;
    }

private:
    void resize(size_t newCapacity) {
        T* newData = new T[newCapacity];
        for (size_t i = 0; i < m_size; ++i) {
            newData[i] = std::move(m_data[i]);
        }
        delete[] m_data;
        m_data = newData;
        m_capacity = newCapacity;
    }

    T* m_data{nullptr};
    size_t m_capacity{0};
    size_t m_size{0};
};
