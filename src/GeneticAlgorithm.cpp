#include "GeneticAlgorithm.hpp"
  
/**
 * @brief Cria uma população de cromossomos com um número específico de genes.
 * 
 * Se funções heurísticas forem fornecidas, os cromossomos são inicializados utilizando essas heurísticas.
 * Caso contrário, uma exceção é lançada.
 * 
 * @param generateChromosomeHeuristics Um vetor de funções heurísticas que geram cromossomos com base em um grafo.
 * @param graph O grafo utilizado para inicializar os cromossomos.
 * @param heuristic Um índice que define qual método heurístico deve ser utilizado:
 *        - `1`: Usa a Heurística 1
 *        - `2`: Usa a Heurística 2
 *        - `3`: Usa a Heurística 3
 *        - `4`: Usa uma combinação das Heurísticas 1, 2 e 3 em proporções iguais.
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


/**
 * **Elitismo:** Seleciona os melhores indivíduos da população com base no menor valor de aptidão (fitness),
 * garantindo que apenas os melhores sejam preservados para a próxima geração.
 * 
 * 1. Ordena a população em ordem crescente de fitness.
 * 2. Mantém apenas os `iterations` melhores indivíduos, onde `iterations` é calculado como uma fração 
 *    do tamanho total da população (`elitism_rate`).
 * 
 * @param population     Vetor contendo os cromossomos da população atual.
 * @param elitism_rate   Taxa de elitismo, ajudando a determinar a proporção de indivíduos que serão preservados.
 */

void GeneticAlgorithm::elitism(std::vector<Chromosome>& population, float elitism_rate) {
    size_t iterations { static_cast<size_t>(std::ceil(population.size() * elitism_rate)) };
	
	std::sort(population.begin(), population.end(), 
            [](const Chromosome& a, const Chromosome& b){
            	return a.fitness < b.fitness;
	});
	
	population.resize(iterations);
}

/**
 * **Elitismo por clonagem:** Preserva o melhor indivíduo da população clonando-o múltiplas vezes.
 * 
 * 1. Encontra o melhor indivíduo da população.
 * 2. Remove todos os outros indivíduos.
 * 3. Clona o melhor indivíduo `iterations` vezes para preencher a nova população.
 * 
 * @param population     Vetor contendo os cromossomos da população atual.
 * @param elitism_rate   Taxa de elitismo, ajuda a determinar quantas cópias do melhor indivíduo serão criadas.
 */

void GeneticAlgorithm::elitismClones(std::vector<Chromosome>& population, float elitism_rate) {
    size_t iterations { static_cast<size_t>(std::ceil(population.size() * elitism_rate)) };
	
	Chromosome best_one { findBestSolution(population) };
	
	population.clear();
	population.reserve(iterations);
	
	for (size_t i {0}; i < iterations; ++i) {
		population.push_back(best_one);
	}
}

/**
 * **Mutação Constante**
 * 
 * - Um único gene do cromossomo é selecionado aleatoriamente e recebe um novo valor.
 * - O novo valor é escolhido aleatoriamente dentro do conjunto de rótulos possíveis `{0, 2, 3, 4}`.
 * - Após a mutação, verifica-se se o cromossomo continua viável utilizando a função `feasibilityCheck`.
 * - A mutação ocorre com uma probabilidade definida por `mutation_rate`.
 * 
 * @param chromosome Cromossomo a ser mutado.
 * @return O cromossomo mutado.
 */

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

/**
 * **Mutação Linear**
 * 
 * - Cada gene do cromossomo tem uma chance de ser mutado, baseada na taxa `mutation_rate`.
 * - Se um gene for selecionado para mutação, ele recebe um novo valor aleatório do conjunto `{0, 2, 3, 4}`.
 * - Após cada mutação, a viabilidade do cromossomo é verificada usando `feasibilityCheck`.
 * - Essa abordagem permite um maior grau de variação na população genética.
 * 
 * @param chromosome Cromossomo a ser mutado.
 * @return O cromossomo mutado.
 */

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


/**
 * **Cruzamento de um ponto (one-point crossover)**: 
 * Uma posição aleatória é selecionada no vetor de genes. Todos os genes a partir essa posição 
 * são trocados entre os cromossomos, criando a prole.
 * 
 * @param chromosome1 Primeiro cromossomo pai.
 * @param chromosome2 Segundo cromossomo pai.
 * @return O melhor cromossomo resultante do cruzamento.
 */

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

/**
 * **Cruzamento de dois pontos (two-point crossover)**: 
 * Duas posições aleatórias são escolhidas dentro do vetor de genes. Todos os genes entre essas 
 * duas posições são trocados entre os cromossomos, permitindo uma recombinação mais complexa.
 * 
 * @param chromosome1 Primeiro cromossomo pai.
 * @param chromosome2 Segundo cromossomo pai.
 * @return O melhor cromossomo resultante do cruzamento.
 */

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

/**
 * Realiza a seleção por torneio, escolhendo aleatoriamente um subconjunto de indivíduos da população
 * e retornando aquele com o menor valor de fitness.
 * 
 * 1. **Seleção Aleatória**: `individuals_size` indivíduos são escolhidos aleatoriamente da população.
 * 2. **Comparação**: O indivíduo com o menor valor de fitness dentro do subconjunto selecionado é escolhido como vencedor.
 * 
 * @param population        Vetor contendo todos os cromossomos da geração atual.
 * @param individuals_size  Quantidade de indivíduos que participarão do torneio.
 * @return O cromossomo com o menor valor de fitness dentro do grupo selecionado.
 */

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
 * Gera uma nova população aplicando operadores genéticos conforme os parâmetros especificados.
 * 
 * 1. **Elitismo**: Se ativado, preserva os melhores indivíduos da população atual com base na taxa de elitismo. 
 *    Caso contrário, uma variação com clones é aplicada.
 * 2. **Seleção**: Dois cromossomos são escolhidos da população inicial. A seleção pode ser feita pelo método de torneio 
 *    ou aleatoriamente.
 * 3. **Crossover**: Aplica o operador de cruzamento para gerar um novo cromossomo. O cruzamento pode ser de dois pontos 
 *    ou de um ponto, dependendo da configuração.
 * 4. **Mutação**: Introduz variações no cromossomo gerado para aumentar a diversidade genética. Pode ser aplicada 
 *    uma mutação de um ponto ou uma mutação linear.
 * 5. **Atualização da população**: A nova geração é formada a partir dos indivíduos resultantes dessas operações, garantindo 
 *    que a população mantenha um tamanho constante.
 * 
 * @param flag_elitism   Indica qual estratégia de elitismo será aplicada.
 * @param flag_selection Define o método de seleção (torneio ou aleatório).
 * @param flag_crossover Determina o tipo de crossover a ser utilizado.
 * @param flag_mutation  Define qual técnica de mutação será aplicada.
 * @return A nova população após a aplicação dos operadores genéticos.
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

// retorna uma tupla contendo: melhor fitness, média e desvio padrão de fitness, respectivamente.

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
