#ifndef TRIPLE_ROMAN_DOMINATION_HPP
#define TRIPLE_ROMAN_DOMINATION_HPP

#include "Graph.hpp"
#include "GeneticAlgorithm.hpp"
#include "Chromosome.hpp"
#include "util_functions.hpp"
#include <vector>
#include <random>

class TripleRomanDomination {
private:
    Graph graph;
    GeneticAlgorithm genetic_algorithm;
    std::vector<int> solution_genetic_algorithm;
    size_t genetic_algorithm_best_fitness;

public:
    TripleRomanDomination(Graph& graph, size_t population_size, size_t genes_size, size_t generations,
                float mutation_rate, float elitism_rate, float cross_over_rate,
                float tournament_population_size)
        : graph(std::move(graph)),
          genetic_algorithm(graph, population_size, genes_size, generations, mutation_rate, elitism_rate, cross_over_rate, tournament_population_size) {}
          
  	TripleRomanDomination() = default;

    ~TripleRomanDomination() = default;

    Graph& getGraph();
    std::vector<int> getSolutionGeneticAlgorithm();

    size_t getGeneticAlgorithmBestFitness();

    void runGeneticAlgorithm(short int heuristic);

    static Chromosome heuristic1(const Graph& graph);
    static Chromosome heuristic2(const Graph& graph);
    static Chromosome heuristic3(const Graph& graph);
};

#endif

