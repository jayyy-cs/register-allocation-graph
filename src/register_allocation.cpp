#include "register_allocation.hpp"

#include "CSVReader.hpp"
#include "InterferenceGraph.hpp"

namespace shindler::ics46::project6 {

// assignRegisters
//
// This is where you implement the register allocation algorithm
// as mentioned in the README. Remember, you must allocate at MOST
// d(G) + 1 registers where d(G) is the maximum degree of the graph G.
// If num_registers is not enough registers to accomodate the passed in
// graph you should return an empty map. You MUST use registers in the
// range [1, num_registers] inclusive.
RegisterAssignment assignRegisters(const std::string &pathToGraph,
                                   int numRegisters) noexcept {
    InterferenceGraph<Variable> igraph = CSVReader::load(pathToGraph);

    // Determine the maximum degree in the graph
    unsigned maxDegree = 0;
    for (const auto& vertex : igraph.vertices())
    {
        maxDegree = std::max(maxDegree, igraph.degree(vertex));
    }

    // Check if the number of registers is sufficient
    if (maxDegree + 1 > static_cast<unsigned>(numRegisters))
    {
        return {}; //return empty map
    }

    std::vector<Variable> vertices(igraph.vertices().begin(), igraph.vertices().end());
    // Selection sort
    for (size_t i = 0; i < vertices.size(); ++i)
    {
        size_t maxIndex = i;
        for (size_t j = i + 1; j < vertices.size(); ++j)
        {
            if (igraph.degree(vertices[j]) > igraph.degree(vertices[maxIndex]))
            {
                maxIndex = j;
            }
        }
        std::swap(vertices[i], vertices[maxIndex]);
    }

    // Allocate registers
    std::unordered_map<Variable, int> registerAssignment;
    for (const auto& vertex : vertices)
    {
        std::unordered_set<int> unavailableRegisters;
        for (const auto& neighbor : igraph.neighbors(vertex))
        {
            auto found = registerAssignment.find(neighbor);
            if (found != registerAssignment.end())
            {
                unavailableRegisters.insert(found->second);
            }
        }

        // Assign the first available register
        int assignedRegister = 1;
        while (unavailableRegisters.find(assignedRegister) != unavailableRegisters.end())
        {
            assignedRegister++;
        }

        registerAssignment[vertex] = assignedRegister;
    }

    return registerAssignment;
}

}  // namespace shindler::ics46::project6
