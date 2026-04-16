# register-allocation-graph

## Overview
This project implements an interference graph and solves the register allocation problem using graph coloring techniques.
Register allocation is a fundamental compiler optimization problem where variables must be assigned to a limited number of CPU registers. 
This project models the problem as a graph coloring problem, where variables that interfere cannot share the same register.

## Features
Implementation of an interference graph data structure
Support for adding/removing vertices and edges
Efficient neighbor and degree queries
Register allocation using graph coloring (Welsh-Powell algorithm)
Verification of allocation correctness
Optional visualization using DOT/Graphviz

## Technical Details

### Interference Graph
- Undirected graph where:
  - Nodes = variables
  - Edges = overlapping lifetimes (interference)
- Implemented using adjacency-based data structures for efficient lookup

### Register Allocation
- Based on graph coloring reduction
- Uses Welsh-Powell algorithm:
  - Sort vertices by degree (descending)
  - Assign smallest available color (register)
- Guarantees:
  - No two adjacent vertices share the same register
  - Uses at most **d(G) + 1** registers
