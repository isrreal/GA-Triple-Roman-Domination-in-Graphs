# Genetic Algorithm for Triple Roman Domination Problem

This project implements a meta-heuristic approach to solve the Triple Roman Domination Problem (TRDP), an NP-complete combinatorial optimization problem. The meta-heuristic used is a Genetic Algorithm (GA), which leverages evolutionary principles such as selection, crossover, and mutation to find high-quality solutions efficiently. The goal is to identify an optimal labeling function that minimizes the total weight while satisfying the problem's constraints, with applications in resource allocation, communication networks, and strategic planning.

## What is the Triple Roman Domination Problem?
Given a graph $G = (V, E)$, a *Triple Roman Domination Function (TRDF)* is a labeling function $h: V \to \{0,1,2,3,4\}$ that satisfies the following conditions:

1. If $h(v) = 0$, then $v$ must have a neighbor $u$ with $h(u) = 4$, or two neighbors $u_1, u_2$ with $h(u_1) = 2$ and $h(u_2) = 3$, or three neighbors $u_1, u_2, u_3$ all with $h(u_i) = 2$.
2. If $h(v) = 1$, then $v$ must have at least one neighbor $u$ with $h(u) \geq 3$ or two neighbors with $h(u_1) = h(u_2) = 2$.
3. If $h(v) = 2$, then $v$ must have a neighbor $u$ with $h(u) \geq 2$.

The objective is to find a TRDF that minimizes the total weight $h(G) = \sum_{v \in V(G)} h(v)$, ensuring all constraints are met.

## Genetic Algorithm (GA)

### Chromosome Representation
A chromosome is represented as a vector of size $|V|$, where each position corresponds to a vertex and contains its assigned label $h(v)$. The fitness of a chromosome is evaluated by its total weight, with lower values being more optimal.

### Initial Population
Four heuristics are used to generate the initial population:

1. **First Heuristic**: A vertex $v$ is randomly selected and assigned label 2 (or 3 if it has degree 0). Its adjacent vertices receive label 0. If any isolated vertices remain, they are labeled 3. A feasibility check ensures the TRDF constraints are satisfied.
2. **Second Heuristic**: A vertex $v$ is randomly selected and assigned label 4 (or 3 if it has degree 0). Neighbors receive label 0. Isolated vertices are labeled 3. A toggle function attempts to minimize labels while maintaining feasibility.
3. **Third Heuristic**: Vertices are sorted in descending order of degree. The highest-degree vertex is labeled 4 (or 3 if degree 0), and its neighbors receive label 0. Isolated vertices are labeled 3.
4. **Fourth Heuristic**: A mix of all three heuristics, generating 33% of the population using each approach.

### Selection Methods
Two selection methods are implemented:

- **Tournament Selection (flag = 1):** $k$ random individuals are chosen, and the best (lowest fitness) is selected.
- **Random Selection (flag = 0):** Two individuals are chosen randomly from the population.

### Crossover Operators
Two crossover methods are used:

1. **One-Point Crossover (flag = 0):** A random crossover point is selected, and genes after this point are swapped between parents.
2. **Two-Point Crossover (flag = 1):** Two crossover points are selected, and genes between them are swapped.

### Mutation Operators
Mutation introduces random changes to genes to maintain diversity. Two types are implemented:

- **Constant Mutation (flag = 1):** A random gene is altered with a probability determined by the mutation rate. The new value is selected randomly from \{0,2,3,4\}.
- **Linear Mutation (flag = 0):** Similar to constant mutation but with a dynamically adjusted probability.
- **Feasibility Check:** Ensures solutions remain valid after mutation.

### Elitism
Elitism ensures the best solutions are preserved:

- **Elitism by Sorting (flag = 1):** The population is sorted by fitness, and the top $k$ individuals are retained.
- **Best Chromosome Cloning (flag = 0):** The best individual is cloned $k$ times into the next generation.

### Stopping Criteria
The algorithm terminates when:
- The maximum number of generations is reached (used in this study).

### Solution Analysis
Performance is evaluated based on:
- Best fitness
- Average fitness
- Standard deviation of fitness values

## How to Use

1. Clone the repository:
   ```bash
   git clone https://github.com/isrreal/Triple-Roman-Domination-In-Graphs-meta-heuristics.git
   ```

2. Run the code:
   ```bash
   # Syntax:
   # ./app <graph_file> <graph_name> <flag_elitism> <flag_selection> <flag_crossover> <flag_mutation> <graph_order>

   # Example:
   ./app graph.txt graph_name 1 0 1 0 0
   ```
 
