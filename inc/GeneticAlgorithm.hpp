#ifndef GENETIC_ALGORITHM_HPP
#define GENETIC_ALGORITHM_HPP

#include <iostream>
#include <random>
#include <vector>
#include <functional>
#include <tuple>
#include "Chromosome.hpp"
#include "util_functions.hpp"

class GeneticAlgorithm {
	private:
		size_t population_size;
		size_t genes_size;
		std::vector<Chromosome> population;
	    size_t generations;
        Graph graph;
        std::vector<int> best_solution;
        float best_fitness;
        float fitness_mean;
        float fitness_std;
        
        float elitism_rate;
        float crossover_rate;
        float mutation_rate;
        size_t tournament_population_size;


		inline void createPopulation(std::vector<std::function<Chromosome(const Graph&)>> generateChromosomeHeuristics,
		 	const Graph& graph, size_t heuristic);
	
        inline std::vector<Chromosome>& createNewPopulation(bool flag_elitism, bool flag_selection, bool flag_crossover, bool flag_mutation);    
        
     	inline void elitism(std::vector<Chromosome>& population, float elitism_rate);	
     	
     	inline void elitismClones(std::vector<Chromosome>& population, float elitism_rate);	
        
		inline Chromosome onePointCrossOver(const Chromosome& chromosome1, const Chromosome& cromossomo2); 
                	
    	inline Chromosome twoPointCrossOver(const Chromosome& chromosome1, const Chromosome& cromossomo2);
                	
        inline Chromosome& mutation1(Chromosome& chromosome);
        
        inline Chromosome& mutation2(Chromosome& chromosome);
        
		inline const Chromosome& tournamentSelection(const std::vector<Chromosome>& population, size_t individuals_size);
		
		inline static Chromosome& chooseBestSolution(Chromosome& chromosome1, Chromosome& chromosome2);
		
		inline Chromosome findBestSolution(const std::vector<Chromosome>& population); 

	public:
		GeneticAlgorithm(Graph& graph, size_t population_size, size_t genes_size, size_t generations,
			float elitism_rate, float crossover_rate, float mutation_rate, size_t tournament_population_size):
			  population_size(population_size), genes_size(genes_size), 
			  population(population_size), generations(generations), 
			  graph(graph), best_solution(), fitness_mean(0), fitness_std(0.0),
	  		  elitism_rate(elitism_rate), crossover_rate(crossover_rate),
	  		  mutation_rate(mutation_rate), tournament_population_size(tournament_population_size) {}               

		~GeneticAlgorithm() {}
		
		size_t getGenerations();   
		
		size_t getBestFitness();

		size_t getFitnessMean();

		size_t getFitnessSTD();
		
        std::vector<int> getBestSolution();		      
        
        std::tuple<size_t, float, float> run(size_t generations, std::vector<std::function<Chromosome(const Graph&)>>, size_t chosen_heuristic,
        		bool flag_elitism, bool flag_selection , bool flag_crossover , bool flag_mutation);
};	

#endif
