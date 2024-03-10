#ifndef __PROJ6_INTERFERENCE_GRAPH__HPP
#define __PROJ6_INTERFERENCE_GRAPH__HPP

#include <stdexcept>
#include <string>
#include <unordered_set>
#include <unordered_map>

namespace shindler::ics46::project6 {

class UnknownVertexException : public std::runtime_error {
   public:
    explicit UnknownVertexException(const std::string &vertex)
        : std::runtime_error("Unkown vertex " + vertex) {}
};

class UnknownEdgeException : public std::runtime_error {
   public:
    UnknownEdgeException(const std::string &source,
                         const std::string &destination)
        : std::runtime_error("Unknown edge " + source + " - " + destination) {}
};

// InterferenceGraph
//
// This is a class representing an interference graph
// as described in "Part 1: Interference Graph" of the README.md
// file. Though this class is templated, because of the usage of exceptions
// UnknownVertexException and UnknownEdgeException it will
// ONLY be tested with strings.
template <typename T>
class InterferenceGraph {
   public:
    // Custom type used to represent edges. This is mainly
    // used in the utility function for reading and writing
    // the graph structure to/from files. You don't need to use it.
    using EdgeTy = std::pair<T, T>;

    InterferenceGraph();

    // You DO NOT need to implement these
    InterferenceGraph(const InterferenceGraph &) = delete;
    InterferenceGraph(InterferenceGraph &&) = default;
    InterferenceGraph &operator=(const InterferenceGraph &) = delete;
    InterferenceGraph &operator=(InterferenceGraph &&) = default;

    ~InterferenceGraph();

    void addEdge(const T &source, const T &destination);

    void addVertex(const T &vertex);

    void removeEdge(const T &source, const T &destination);

    void removeVertex(const T &vertex);

    [[nodiscard]] std::unordered_set<T> vertices() const noexcept;

    [[nodiscard]] std::unordered_set<T> neighbors(const T &vertex) const;

    [[nodiscard]] unsigned numVertices() const noexcept;

    [[nodiscard]] unsigned numEdges() const noexcept;

    [[nodiscard]] bool interferes(const T &source, const T &destination) const;

    [[nodiscard]] unsigned degree(const T &vertex) const;

   private:
    std::unordered_map<T, std::unordered_set<T>> adjList;
    unsigned edgeCounter;
};

template <typename T>
InterferenceGraph<T>::InterferenceGraph()
:edgeCounter(0)
{
    adjList.clear();
}

template <typename T>
InterferenceGraph<T>::~InterferenceGraph()
{
}

template <typename T>
std::unordered_set<T> InterferenceGraph<T>::neighbors(const T &vertex) const {
    auto vertexIterator = adjList.find(vertex);
    if(vertexIterator == adjList.end())
    {
        throw UnknownVertexException(vertex);
    }

    return vertexIterator->second;
}

template <typename T>
std::unordered_set<T> InterferenceGraph<T>::vertices() const noexcept {
    std::unordered_set<T> verticesSet;
    for(const auto& pair: adjList)
    {
        verticesSet.insert(pair.first);
    }

    return verticesSet;

}

template <typename T>
unsigned InterferenceGraph<T>::numVertices() const noexcept {
    return adjList.size();
}

template <typename T>
unsigned InterferenceGraph<T>::numEdges() const noexcept {
    return edgeCounter;
}

template <typename T>
void InterferenceGraph<T>::addEdge(const T &source, const T &destination) {
    auto sourceIterator = adjList.find(source);
    auto destinationIterator = adjList.find(destination);

    // If either vertex does not exist, throw UnknownVertexException
    if(sourceIterator == adjList.end() || destinationIterator == adjList.end())
    {
        throw UnknownVertexException(sourceIterator == adjList.end()? source : destination);
    }
    
    if (adjList[source].find(destination) == adjList[source].end())
    {
        adjList[source].insert(destination);
        adjList[destination].insert(source);
        edgeCounter++;  // Increment edge counter
    }
}

template <typename T>
void InterferenceGraph<T>::removeEdge(const T &source, const T &destination) {
    auto sourceIterator = adjList.find(source);
    auto destinationIterator = adjList.find(destination);

    // If either vertex does not exist, throw UnknownVertexException
    if(sourceIterator == adjList.end() || destinationIterator == adjList.end())
    {
        throw UnknownVertexException(sourceIterator == adjList.end()? source : destination);
    }

    // Both vertices exist, now check if the edge exists
    if(sourceIterator->second.find(destination) == sourceIterator->second.end() ||
        destinationIterator->second.find(source) == destinationIterator->second.end())
    {
        // If the edge does not exist, throw UnknownEdgeException
        throw UnknownEdgeException(source, destination);
    }

    if(adjList[source].find(destination) != adjList[source].end())
    {
        adjList[source].erase(destination);
        adjList[destination].erase(source);
        edgeCounter--;
    }
    
}

template <typename T>
void InterferenceGraph<T>::addVertex(const T &vertex) {
    adjList.insert({vertex, std::unordered_set<T>()});
}

template <typename T>
void InterferenceGraph<T>::removeVertex(const T &vertex) {
    auto vertexIterator = adjList.find(vertex);
    if(vertexIterator == adjList.end())
    {
        throw UnknownVertexException(vertex);
    }

    for(const T& neighbor : vertexIterator->second)
    {
        adjList[neighbor].erase(vertex);
        edgeCounter--;
    }

    adjList.erase(vertex);

}

template <typename T>
bool InterferenceGraph<T>::interferes(const T &source,
                                      const T &destination) const {
    auto sourceIterator = adjList.find(source);
    auto destinationIterator = adjList.find(destination);

    // If either vertex does not exist, throw UnknownVertexException
    if(sourceIterator == adjList.end() || destinationIterator == adjList.end())
    {
        throw UnknownVertexException(sourceIterator == adjList.end() ? source : destination);
    }

    return sourceIterator->second.find(destination) != sourceIterator->second.end();
}

template <typename T>
unsigned InterferenceGraph<T>::degree(const T &vertex) const {
    auto vertexIterator = adjList.find(vertex);
    if(vertexIterator == adjList.end())
    {
        throw UnknownVertexException(vertex);
    }

    return vertexIterator->second.size();
}

}  // namespace shindler::ics46::project6

#endif
