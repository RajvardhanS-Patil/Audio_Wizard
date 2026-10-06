#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include "../Queue.hpp"
#include "../Stack.hpp"

// Generic Graph implementation using Adjacency List
template <typename T>
class Graph {
private:
    std::unordered_map<T, std::vector<std::pair<T, int>>> m_adjList;
    bool m_directed;

public:
    explicit Graph(bool directed = false) : m_directed(directed) {}

    void addVertex(const T& vertex) {
        if (m_adjList.find(vertex) == m_adjList.end()) {
            m_adjList[vertex] = std::vector<std::pair<T, int>>();
        }
    }

    void removeVertex(const T& vertex) {
        m_adjList.erase(vertex);
        for (auto& pair : m_adjList) {
            auto& edges = pair.second;
            edges.erase(
                std::remove_if(edges.begin(), edges.end(), [&](const std::pair<T, int>& e) {
                    return e.first == vertex;
                }),
                edges.end()
            );
        }
    }

    void addEdge(const T& src, const T& dest, int weight = 1) {
        addVertex(src);
        addVertex(dest);
        
        // Add edge if it doesn't already exist
        auto& srcEdges = m_adjList[src];
        auto it = std::find_if(srcEdges.begin(), srcEdges.end(), [&](const std::pair<T, int>& e) {
            return e.first == dest;
        });
        if (it == srcEdges.end()) {
            srcEdges.push_back({dest, weight});
        }

        if (!m_directed) {
            auto& destEdges = m_adjList[dest];
            auto it2 = std::find_if(destEdges.begin(), destEdges.end(), [&](const std::pair<T, int>& e) {
                return e.first == src;
            });
            if (it2 == destEdges.end()) {
                destEdges.push_back({src, weight});
            }
        }
    }

    void removeEdge(const T& src, const T& dest) {
        if (m_adjList.find(src) != m_adjList.end()) {
            auto& edges = m_adjList[src];
            edges.erase(
                std::remove_if(edges.begin(), edges.end(), [&](const std::pair<T, int>& e) {
                    return e.first == dest;
                }),
                edges.end()
            );
        }

        if (!m_directed && m_adjList.find(dest) != m_adjList.end()) {
            auto& edges = m_adjList[dest];
            edges.erase(
                std::remove_if(edges.begin(), edges.end(), [&](const std::pair<T, int>& e) {
                    return e.first == src;
                }),
                edges.end()
            );
        }
    }

    bool hasVertex(const T& vertex) const {
        return m_adjList.find(vertex) != m_adjList.end();
    }

    bool hasEdge(const T& src, const T& dest) const {
        auto it = m_adjList.find(src);
        if (it != m_adjList.end()) {
            for (const auto& e : it->second) {
                if (e.first == dest) return true;
            }
        }
        return false;
    }

    std::vector<T> getNeighbors(const T& vertex) const {
        std::vector<T> neighbors;
        auto it = m_adjList.find(vertex);
        if (it != m_adjList.end()) {
            for (const auto& e : it->second) {
                neighbors.push_back(e.first);
            }
        }
        return neighbors;
    }

    std::vector<T> getVertices() const {
        std::vector<T> vertices;
        for (const auto& pair : m_adjList) {
            vertices.push_back(pair.first);
        }
        return vertices;
    }

    size_t vertexCount() const {
        return m_adjList.size();
    }

    size_t edgeCount() const {
        size_t count = 0;
        for (const auto& pair : m_adjList) {
            count += pair.second.size();
        }
        return m_directed ? count : count / 2;
    }

    void clear() {
        m_adjList.clear();
    }

    bool empty() const {
        return m_adjList.empty();
    }

    // BFS Traversal
    std::vector<T> bfs(const T& startVertex) const {
        std::vector<T> result;
        if (!hasVertex(startVertex)) return result;

        std::unordered_set<T> visited;
        Queue<T> q;

        visited.insert(startVertex);
        q.enqueue(startVertex);

        while (!q.empty()) {
            T current = q.front();
            q.dequeue();
            result.push_back(current);

            for (const auto& neighbor : getNeighbors(current)) {
                if (visited.find(neighbor) == visited.end()) {
                    visited.insert(neighbor);
                    q.enqueue(neighbor);
                }
            }
        }

        return result;
    }

    // DFS Traversal
    std::vector<T> dfs(const T& startVertex) const {
        std::vector<T> result;
        if (!hasVertex(startVertex)) return result;

        std::unordered_set<T> visited;
        Stack<T> s;

        s.push(startVertex);

        while (!s.isEmpty()) {
            T current = s.top();
            s.pop();

            if (visited.find(current) == visited.end()) {
                visited.insert(current);
                result.push_back(current);

                // Push neighbors (reverse order for standard DFS visit order, optional)
                auto neighbors = getNeighbors(current);
                for (auto it = neighbors.rbegin(); it != neighbors.rend(); ++it) {
                    if (visited.find(*it) == visited.end()) {
                        s.push(*it);
                    }
                }
            }
        }

        return result;
    }

    const std::unordered_map<T, std::vector<std::pair<T, int>>>& getAdjacencyList() const {
        return m_adjList;
    }
};
