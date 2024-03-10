#include <CSVReader.hpp>
#include <IGWriter.hpp>
#include <InterferenceGraph.hpp>
#include <catch2/catch_amalgamated.hpp>

namespace {

namespace proj6 = shindler::ics46::project6;

const std::string &GRAPH_DIR = std::string("tst/graphs/");

TEST_CASE("SimpleAddEdgesAndVertices", "[Required][IG]") {
    const auto &GRAPH = GRAPH_DIR + "simple.csv";
    /// autograder/git-repo/student-execution-files/tests/
    const proj6::InterferenceGraph<proj6::Variable> &igraph =
        proj6::CSVReader::load(GRAPH);

    REQUIRE(igraph.numEdges() == 3);
    REQUIRE(igraph.numVertices() == 3);
}

TEST_CASE("Neighbors", "[Required][IG]") {
    const auto &GRAPH = GRAPH_DIR + "simple.csv";

    const proj6::InterferenceGraph<proj6::Variable> &igraph =
        proj6::CSVReader::load(GRAPH);
    const std::unordered_set<proj6::Variable> &expected_neighbors = {"y", "z"};

    REQUIRE(igraph.neighbors("x") == expected_neighbors);
}

TEST_CASE("RuntimeTest", "[Additional][IG]") {
const auto &GRAPH = GRAPH_DIR + "complete_1000.csv";
proj6::InterferenceGraph<proj6::Variable> igraph = proj6::CSVReader::load(GRAPH);
}


TEST_CASE("Complete6AddEdgesAndVertices", "[Additional][IG]") {
    const auto &GRAPH = GRAPH_DIR + "complete_6.csv";
    /// autograder/git-repo/student-execution-files/tests/
    proj6::InterferenceGraph<proj6::Variable> igraph = proj6::CSVReader::load(GRAPH);

    REQUIRE(igraph.numEdges() == 15);
    REQUIRE(igraph.numVertices() == 6);

    const std::unordered_set<proj6::Variable> &expected_neighbors = {"1", "3", "4", "5", "6"};
    REQUIRE(igraph.neighbors("2") == expected_neighbors);
    const std::unordered_set<proj6::Variable> &expected_vertices = {"1", "2", "3", "4", "5", "6"};
    REQUIRE(igraph.vertices() == expected_vertices);

    igraph.removeEdge("1", "5");
    REQUIRE(igraph.numEdges() == 14);
    REQUIRE(igraph.numVertices() == 6);
    REQUIRE_FALSE(igraph.interferes("1", "5"));
    REQUIRE_FALSE(igraph.interferes("5", "1"));
    REQUIRE(igraph.interferes("2", "5"));
    REQUIRE(igraph.degree("1") == 4);
    REQUIRE(igraph.degree("5") == 4);
    REQUIRE_THROWS(igraph.degree("10"));
    igraph.removeVertex("1");
    REQUIRE(igraph.numEdges() == 10);
    REQUIRE(igraph.numVertices() == 5);
}

TEST_CASE("bigBipartiteTest", "[Additional][IG]") {
    const auto &GRAPH = GRAPH_DIR + "big_bipartite.csv";
    /// autograder/git-repo/student-execution-files/tests/
    const proj6::InterferenceGraph<proj6::Variable> &igraph =
        proj6::CSVReader::load(GRAPH);

    REQUIRE(igraph.numEdges() == 8);
    REQUIRE(igraph.numVertices() == 6);

    const std::unordered_set<proj6::Variable> &expected_neighbors = {"x", "y"};
    REQUIRE(igraph.neighbors("a") == expected_neighbors);
}

TEST_CASE("ExceptionTesting", "[Additional][IG]") {
    const auto &GRAPH = GRAPH_DIR + "big_bipartite.csv";
    proj6::InterferenceGraph<proj6::Variable> igraph = proj6::CSVReader::load(GRAPH);

    REQUIRE_THROWS(igraph.neighbors("w"));
    // vertex not exist
    REQUIRE_THROWS(igraph.addEdge("w", "x"));
    REQUIRE_THROWS(igraph.addEdge("y", "w"));
    // Edge not exist
    REQUIRE_THROWS(igraph.removeEdge("a", "b"));
    // vertex not exist
    REQUIRE_THROWS(igraph.removeEdge("w", "x"));
    REQUIRE_THROWS(igraph.removeEdge("y", "w"));

    REQUIRE_THROWS(igraph.removeVertex("w"));

    REQUIRE_THROWS(igraph.interferes("w", "x"));
    REQUIRE_THROWS(igraph.interferes("y", "w"));
    REQUIRE_THROWS(igraph.degree("w"));
}

TEST_CASE("AddDuplicate", "[Additional][IG]") {
    const auto &GRAPH = GRAPH_DIR + "simple.csv";

    proj6::InterferenceGraph<proj6::Variable> igraph = proj6::CSVReader::load(GRAPH);
    igraph.addEdge("x", "y");
    REQUIRE(igraph.numEdges() == 3);
    igraph.addVertex("x");
    REQUIRE(igraph.numVertices() == 3);
    igraph.addVertex("w");
    igraph.addEdge("w", "x");
    igraph.addEdge("w", "y");
    REQUIRE(igraph.numEdges() == 5);
    REQUIRE(igraph.numVertices() == 4);
}

}  // namespace
