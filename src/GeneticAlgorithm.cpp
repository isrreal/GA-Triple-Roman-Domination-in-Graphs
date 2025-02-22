#include "GeneticAlgorithm.hpp"
  
/**
 * @brief Creates a population of chromosomes with a specific number of genes.
 * 
 * If a heuristic function is provided, the chromosomes are initialized using this heuristic.
 * Otherwise, chromosomes are initialized with random genes.
 * 
 * @param heuristic A pointer to a function that generates chromosomes based on a graph.
 * @param graph Object to the graph used to initialize the chromosomes.
 */

void GeneticAlgorithm::createPopulation(
    std::vector<std::function<Chromosome(const Graph&)>> generateChromosomeHeuristics, 
    const Graph& graph, 
    size_t heuristic) {
    
	if (!generateChromosomeHeuristics.empty()) { 
	
		Chromosome func;
		
        if (heuristic == 4) { 
            size_t portion_size { static_cast<size_t>(population_size / 3) };
			size_t remainder { population_size % 3 }; 
			size_t index {0};
			
			for (size_t i {0}; i < 3; ++i) {		
		        func = generateChromosomeHeuristics[i](graph);
		        
				for (size_t j {0}; j < portion_size; ++j) {
					this->population[index++] = func;
				}
			}
			
			for (size_t i {0}; i < remainder; ++i) {
				this->population[index++] = generateChromosomeHeuristics[getRandomInt(0, 2)](graph);
			}
			
			std::random_device rd;
			std::mt19937 random_engine(rd());
			std::shuffle(population.begin(), population.end(), random_engine);	

        } 
        
        else if (heuristic > 0 && heuristic < 4) {
        
            func = generateChromosomeHeuristics[heuristic - 1](graph);
            
            for (size_t i {0}; i < population_size; ++i) {
                this->population[i] = func;          
            }
        }
        
     	else {
		    throw population;
    	}
    } 
}


// seleciona os `iterations` melhores indivíduos (com menor valor de aptidão (fitness))

void GeneticAlgorithm::elitism(std::vector<Chromosome>& population, float elitism_rate) {
    size_t iterations { static_cast<size_t>(std::ceil(population.size() * elitism_rate)) };
	
	std::sort(population.begin(), population.end(), 
            [](const Chromosome& a, const Chromosome& b){
            	return a.fitness < b.fitness;
	});
	
	population.resize(iterations);
}

// clona o melhor individuo k vezes

void GeneticAlgorithm::elitismClones(std::vector<Chromosome>& population, float elitism_rate) {
    size_t iterations { static_cast<size_t>(std::ceil(population.size() * elitism_rate)) };
	
	Chromosome best_one { findBestSolution(population) };
	
	population.clear();
	population.reserve(iterations);
	
	for (size_t i {0}; i < iterations; ++i) {
		population.push_back(best_one);
	}
}

// mutação podendo ocorrer em alguma posição do vetor de genes

Chromosome& GeneticAlgorithm::mutation1(Chromosome& chromosome) {	
	if (getRandomFloat(0.0, 1.0) < this->mutation_rate) {
		std::vector<size_t> labels {0, 2, 3, 4};
		size_t randomIndex { getRandomInt(0, genes_size - 1) };
		short random_label { static_cast<short>(getRandomInt(0, labels.size() - 1)) };
			
		chromosome.genes[randomIndex] = labels[random_label];
		feasibilityCheck(this->graph, chromosome);
	}

	return chromosome;   
}

// mutação podendo ocorrer em todos as posições do vetor de genes

Chromosome& GeneticAlgorithm::mutation2(Chromosome& chromosome) {	
	for (size_t i {0}; i < chromosome.genes.size(); ++i) {
		if (getRandomFloat(0.0, 1.0) < this->mutation_rate) {
			std::vector<size_t> labels {0, 2, 3, 4};
			short random_label { static_cast<short>(getRandomInt(0, labels.size() - 1)) };
				
			chromosome.genes[i] = labels[random_label];
			feasibilityCheck(this->graph, chromosome);
		}
	}
	
	return chromosome;   
}

Chromosome GeneticAlgorithm::onePointCrossOver(const Chromosome& chromosome1, const Chromosome& chromosome2) {
  
   size_t index { getRandomInt(0, genes_size - 1) };
   
   Chromosome solution1 { chromosome1 };
   Chromosome solution2 { chromosome2 };
    
   for(size_t i {index + 1}; i <= genes_size - 1; ++i) {
		std::swap(solution1.genes[i], solution2.genes[i]);
   }
   
   feasibilityCheck(this->graph, solution1);
   feasibilityCheck(this->graph, solution2);
    
   return chooseBestSolution(solution1, solution2);
}

Chromosome GeneticAlgorithm::twoPointCrossOver(const Chromosome& chromosome1, const Chromosome& chromosome2) {
	size_t range1 { getRandomInt(0, genes_size - 1) };
	size_t range2 { getRandomInt(0, genes_size - 1) };

	Chromosome solution1 { chromosome1 };
	Chromosome solution2 { chromosome2 };

	if (range1 > range2) {
		std::swap(range1, range2);
	}

	for (size_t i {range1}; i <= range2; ++i) {
		std::swap(solution1.genes[i], solution2.genes[i]);
	}
	
	feasibilityCheck(this->graph, solution1);
	feasibilityCheck(this->graph, solution2);

	return chooseBestSolution(solution1, solution2);
}

const Chromosome& GeneticAlgorithm::tournamentSelection(const std::vector<Chromosome>& population, size_t individuals_size) {  
    std::vector<size_t> tournament_individuals_indices; 
    tournament_individuals_indices.reserve(individuals_size);
    size_t random_index {0};
    
    for (size_t i {0}; i < individuals_size; ++i) {
        random_index = getRandomInt(0, population.size() - 1);
        tournament_individuals_indices.push_back(random_index);
    }
	
    size_t best_index = tournament_individuals_indices[0];
	
    for (auto index : tournament_individuals_indices) {
        if (population[index].fitness < population[best_index].fitness) {
            best_index = index;
        }
    }
		
    return population[best_index];
}

Chromosome& GeneticAlgorithm::chooseBestSolution(Chromosome& chromosome1, Chromosome& chromosome2) {
    return (chromosome1.fitness < chromosome2.fitness ? chromosome1 : chromosome2);
}

Chromosome GeneticAlgorithm::findBestSolution(const std::vector<Chromosome>& population) { 
	Chromosome best_solution { population[0] };

	for (const auto& chromosome: population) {
		if (best_solution.fitness > chromosome.fitness) {
			best_solution = chromosome;
		}
	}

	return best_solution; 
}

/**
 * @brief Generates a new population by crossing over Chromosomes from the current population.
 *     
 * 
 * @return std::vector<Chromosome>& A new population of Chromosomes.
 */

std::vector<Chromosome>& GeneticAlgorithm::createNewPopulation(bool flag_elitism, bool flag_selection, bool flag_crossover, bool flag_mutation) {
	std::vector<Chromosome> old_population = population;
	
   	if (flag_elitism == true) {  		
   		this->elitism(population, elitism_rate);
   	}
   	
   	else {
   		this->elitismClones(population, elitism_rate);
   	}
    
    Chromosome selected1;
    Chromosome selected2;
    Chromosome offspring;
    
    while (population.size() < population_size) {
       	if (flag_selection == true) {  		
        	selected1 = tournamentSelection(old_population, tournament_population_size);
        	selected2 = tournamentSelection(old_population, tournament_population_size);
        }
        
        else {
    		selected1 = old_population[getRandomInt(0, old_population.size() - 1)];
    		selected2 = old_population[getRandomInt(0, old_population.size() - 1)];
        }

       	if (flag_crossover == true) {  		
        	offspring = this->twoPointCrossOver(selected1, selected2);
		} 
        
		else {      
        	offspring = this->onePointCrossOver(selected1, selected2);     
    	}
    	
    	if (flag_mutation == true) {  		
	    	mutation1(offspring);	    
		} 
        
		else {      
	    	mutation2(offspring);	    
    	}
    	
        population.emplace_back(offspring);       
    }
    
    return population;
}
		
// public methods 

size_t GeneticAlgorithm::getGenerations() { return generations; }

size_t GeneticAlgorithm::getBestFitness() { return best_fitness; }

size_t GeneticAlgorithm::getFitnessMean() { return fitness_mean; }

size_t GeneticAlgorithm::getFitnessSTD() { return fitness_std; }

std::tuple<size_t, float, float> GeneticAlgorithm::run(size_t generations, 
    std::vector<std::function<Chromosome(const Graph&)>> heuristics, 
    size_t chosen_heuristic, bool flag_elitism, bool flag_selection, bool flag_crossover, bool flag_mutation) { 
	
    this->createPopulation(heuristics, graph, chosen_heuristic);
   
    Chromosome current_best_solution { findBestSolution(population) };  
    Chromosome best_solution { current_best_solution };
    size_t generation {0};

    float best_fitness { best_solution.fitness };
    float fitness_mean {0.0f};
    float fitness_std {0.0f};

    while (generation < generations) {
        this->population.swap(createNewPopulation(flag_elitism, flag_selection, flag_crossover, flag_mutation));
        
        current_best_solution = findBestSolution(population);  

        if (best_solution.fitness > current_best_solution.fitness) {
            best_solution = current_best_solution;       
            best_fitness = current_best_solution.fitness;
        }

        // Cálculo da média da aptidão
        float sum_fitness = 0.0f;
        for (const auto& individual : population) {
            sum_fitness += individual.fitness;
        }
        fitness_mean = sum_fitness / population.size();

        // Cálculo da variância
        float variance = 0.0f;
        for (const auto& individual : population) {
            variance += std::pow(individual.fitness - fitness_mean, 2);
        }
        variance /= population.size();

        // Cálculo do desvio padrão
        fitness_std = std::sqrt(variance);

        ++generation;
    }  
	
    this->best_solution.swap(best_solution.genes);

    return {best_fitness, fitness_mean, fitness_std};
}

std::vector<int> GeneticAlgorithm::getBestSolution() {
	return best_solution;
}		      
