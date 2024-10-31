#ifndef TRIPLE_ROMAN_DOMINATION_HPP
#define TRIPLE_ROMAN_DOMINATION_HPP

#include "Graph.hpp"
#include "GeneticAlgorithm.hpp"
#include "Chromosome.hpp"
#include "AntColonyOptimization.hpp"
#include <vector>
#include <random>

class TripleRomanDomination {
	private:
    	Graph graph;
    	GeneticAlgorithm* geneticAlgorithm;
    	AntColonyOptimization* ACO;
		std::vector<int> solutionACO;
        std::vector<int> solutionGeneticAlgorithm;
        size_t gamma3rGeneticAlgorithm;		
        size_t gamma3rACO;        
	public:
		TripleRomanDomination(Graph& graph, size_t populationSize, size_t genesSize, uint8_t heuristic, size_t generations,
                size_t numberOfAnts, size_t iterations) 
    			: graph(graph), gamma3rGeneticAlgorithm(0), gamma3rACO(0),   
                geneticAlgorithm(new GeneticAlgorithm(graph, populationSize, genesSize, generations)),
    		    ACO(new AntColonyOptimization(graph, iterations, numberOfAnts)) {
                    this->runGeneticAlgorithm(heuristic);                    
                    this->runACO();
                }

        ~TripleRomanDomination();
        Graph& getGraph();
        std::vector<int> getSolutionACO();
        std::vector<int> getSolutionGeneticAlgorithm();
        size_t getGamma3rGeneticAlgorithm();
        size_t getGamma3rACO();

        void runGeneticAlgorithm(uint8_t heuristic);
        void runACO();

        static Chromosome heuristic1(Graph graph);
        static Chromosome heuristic2(Graph graph);
        static Chromosome heuristic3(Graph graph);   
};
#endif
