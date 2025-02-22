#include "TripleRomanDomination.hpp"
 
/**
 * @brief Executa o algoritmo genético para o problema de Dominação Romana Tripla.
 *
 * Esta função configura e executa o algoritmo genético utilizando uma heurística específica para gerar 
 * a população inicial. O objetivo é encontrar a melhor solução para o problema de Dominação Romana Tripla.
 *
 * @param heuristic Um valor inteiro que indica qual heurística será utilizada para gerar a população inicial.
 *                  As heurísticas disponíveis são:
 *                  - 1: heuristic1
 *                  - 2: heuristic2
 *                  - 3: heuristic3
 *
 * @details A função inicializa o melhor fitness do algoritmo genético como 0 e define um vetor de funções 
 *          heurísticas que serão utilizadas para gerar as soluções iniciais. Em seguida, o algoritmo genético 
 *          é executado com o número de gerações especificado, e a melhor solução encontrada é armazenada. 
 *          O fitness da melhor solução é calculado como a soma dos valores dos genes do cromossomo.
 *
 * @note O fitness da melhor solução é armazenado na variável `genetic_algorithm_best_fitness`, e a solução 
 *       em si é armazenada em `solution_genetic_algorithm`.
 */
 
void TripleRomanDomination::runGeneticAlgorithm(short int heuristic, bool flag_elitism, bool flag_selection, bool flag_crossover, bool flag_mutation) {  
    std::vector<std::function<Chromosome(const Graph&)>> heuristics;
    heuristics.reserve(3);
    
    heuristics.emplace_back(heuristic1);
    heuristics.emplace_back(heuristic2);
    heuristics.emplace_back(heuristic3);

   	genetic_algorithm.run(genetic_algorithm.getGenerations(), heuristics, heuristic, flag_elitism, flag_selection, flag_crossover, flag_mutation);
    
    std::tie(this->genetic_algorithm_best_fitness, 
     	this->genetic_algorithm_fitness_mean, 
     	this->genetic_algorithm_fitness_std) = genetic_algorithm.run(
     		genetic_algorithm.getGenerations(),
     		heuristics,
     		heuristic,
     		flag_elitism,
     		flag_selection,
     		flag_crossover,
     		flag_mutation);

this->solution_genetic_algorithm = genetic_algorithm.getBestSolution();

}

/**
 * @brief Função heurística que gera uma solução inicial de cromossomo para o problema de Dominação Romana Tripla.
 *  
 * Esta heurística seleciona um vértice aleatoriamente e atribui a ele o rótulo 2 (ou 3, caso o vértice seja isolado). 
 * Em seguida, atribui rótulo 0 aos seus vizinhos e os remove do grafo auxiliar. Após a remoção, se houver vértices 
 * isolados, eles são rotulados com 3. Como a solução produzida pode não ser viável (não seguindo as regras da Dominação 
 * Romana Tripla), a rotina `feasibilityCheck` é chamada para corrigir eventuais erros e garantir uma solução válida.
 * 
 * @param graph Grafo utilizado para criar o cromossomo.
 * @return Chromosome Objeto que representa a solução de cromossomo gerada.
 */
 
Chromosome TripleRomanDomination::heuristic1(const Graph& graph) {
    Chromosome solution(Chromosome(graph.getOrder()));
    std::vector<int> valid_vertices;
    size_t chosen_vertex {0};
    valid_vertices.reserve(graph.getOrder());
    Graph temp {graph};

    while (temp.getOrder() > 0) {
        valid_vertices.clear();
        for (const auto& pair : temp.getAdjacencyList()) {
            valid_vertices.push_back(pair.first);
        }
        
        chosen_vertex = valid_vertices[getRandomInt(0, valid_vertices.size() - 1)];

        if (temp.getVertexDegree(chosen_vertex) == 0) {
            solution.genes[chosen_vertex] = 3;
        } 
        
        else {
            solution.genes[chosen_vertex] = 2;
            
            for (const auto& neighbor : temp.getAdjacencyList(chosen_vertex)) {
                if (solution.genes[neighbor] == -1) {
                    solution.genes[neighbor] = 0;
                }
            }
        }

        temp.deleteAdjacencyList(chosen_vertex);
        temp.deleteVertex(chosen_vertex);

        std::vector<size_t> vertices_to_remove;
        
        vertices_to_remove.reserve(temp.getOrder());
        
        for (const auto& [i, _]: temp.getAdjacencyList()) {
            if (temp.getVertexDegree(i) == 0) {
                solution.genes[i] = 3;
                vertices_to_remove.push_back(i);
            }
        }

        for (const auto& vertex : vertices_to_remove) {
            temp.deleteVertex(vertex);
        }
    }

    feasibilityCheck(graph, solution);
    fitness(solution);

    return solution;
}


/**
 * @brief Segunda função heurística para gerar uma solução inicial de cromossomo para o problema de Dominação Romana Tripla.
 * 
 * Esta heurística utiliza um grafo auxiliar G', clone do grafo original, que terá seus vértices removidos durante as iterações. 
 * Seleciona vértices aleatoriamente e atribui a eles o rótulo 4 (ou 3, caso o vértice seja isolado), e rótulo 0 aos seus vizinhos, 
 * removendo-os de G'. Após a remoção, se houver vértices isolados, eles são rotulados com 3.
 * 
 * @param graph Grafo utilizado para criar o cromossomo.
 * @return Chromosome Objeto que representa a solução de cromossomo gerada.
 */
 
Chromosome TripleRomanDomination::heuristic2(const Graph& graph) {
    Chromosome solution(Chromosome(graph.getOrder()));
    std::vector<size_t> valid_vertices;

    size_t chosen_vertex {0};
    
    valid_vertices.reserve(graph.getOrder());
    
    Graph temp {graph};
	
    while (temp.getOrder() > 0) {
     	valid_vertices.clear();
        for (const auto& [i, _] : temp.getAdjacencyList()) {
            valid_vertices.push_back(i);
        }
       
        chosen_vertex = valid_vertices[getRandomInt(0, valid_vertices.size() - 1)];
		
        if (temp.getVertexDegree(chosen_vertex) == 0) {
        	solution.genes[chosen_vertex] = 3;
        }
        	
        else {
        	solution.genes[chosen_vertex] = 4;

		    for (const auto& it: temp.getAdjacencyList(chosen_vertex)) {
		        if (solution.genes[it] == -1) {
		            solution.genes[it] = 0;
		        }
		    }
		}
		
        temp.deleteAdjacencyList(chosen_vertex);
        temp.deleteAdjacencyList(chosen_vertex);
        temp.deleteVertex(chosen_vertex);

       	std::vector<size_t> vertices_to_remove;
        
        vertices_to_remove.reserve(temp.getOrder());
        
        for (const auto& [i, _]: temp.getAdjacencyList()) {
            if (temp.getVertexDegree(i) == 0) {
                solution.genes[i] = 3;
                vertices_to_remove.push_back(i);
            }
        }

        for (const auto& vertex : vertices_to_remove) {
            temp.deleteVertex(vertex);
        }
    }
    
    decreaseLabels(graph, solution.genes);
    
    fitness(solution);
	
    return solution;
}


/**
 * @brief Gera uma solução viável para o problema de Dominação Romana Tripla ordenando os vértices por grau decrescente.
 * 
 * Esta heurística utiliza um grafo auxiliar, clone do grafo original, ordena seus vértices em ordem decrescente de grau e atribui 
 * rótulo 4 ao i-ésimo vértice ordenado, e rótulo 0 aos seus vizinhos, removendo-os do grafo auxiliar. Após isso, a rotina 
 * `decreaseLabels` tenta reduzir os rótulos da solução atual, visando diminuir seu peso.
 * 
 * @param graph Grafo utilizado para criar o cromossomo.
 * @return Chromosome Objeto que representa a solução de cromossomo gerada.
 */
 
Chromosome TripleRomanDomination::heuristic3(const Graph& graph) {
    Chromosome solution(Chromosome(graph.getOrder()));
    
    std::vector<size_t> sorted_vertices;
    
    sorted_vertices.reserve(graph.getOrder());	
    
    Graph temp {graph};
    
    size_t chosen_vertex {0};
    
   	for (const auto& [i, _] : graph.getAdjacencyList()) {
        sorted_vertices.emplace_back(i);
    }

    std::sort(sorted_vertices.begin(), sorted_vertices.end(),
        [&](size_t a, size_t b) {
            return graph.getVertexDegree(a) > graph.getVertexDegree(b);
    });

    while ((temp.getOrder() > 0) && (chosen_vertex < sorted_vertices.size())) {

        while (chosen_vertex < sorted_vertices.size() && 
            	(!temp.vertexExists(sorted_vertices[chosen_vertex]))) {
            ++chosen_vertex;
        }

        if (chosen_vertex >= sorted_vertices.size()) { break; };

	    if (temp.getVertexDegree(sorted_vertices[chosen_vertex]) == 0) {
	        solution.genes[sorted_vertices[chosen_vertex]] = 3;
	    }
	    
	    else {
	        solution.genes[sorted_vertices[chosen_vertex]] = 4;

	        for (const auto& it : temp.getAdjacencyList(sorted_vertices[chosen_vertex])) {
	            if (solution.genes[it] == -1) {
                	solution.genes[it] = 0;
                }
	        }
	    }

	    temp.deleteAdjacencyList(sorted_vertices[chosen_vertex]);
        temp.deleteVertex(sorted_vertices[chosen_vertex++]); 

	    std::vector<size_t> vertices_to_remove;
        
        vertices_to_remove.reserve(temp.getOrder());
        
        for (const auto& [i, _]: temp.getAdjacencyList()) {
            if (temp.getVertexDegree(i) == 0) {
                solution.genes[i] = 3;
                vertices_to_remove.push_back(i);
            }
        }

        for (const auto& vertex : vertices_to_remove) {
            temp.deleteVertex(vertex);
        }
	}
    
    decreaseLabels(graph, solution.genes);
	
    fitness(solution);
	
    return solution;
}

Graph& TripleRomanDomination::getGraph() {
    return this->graph;
}

std::vector<int> TripleRomanDomination::getSolutionGeneticAlgorithm() {
	return this->solution_genetic_algorithm;
}

size_t TripleRomanDomination::getGeneticAlgorithmBestFitness() {
    return this->genetic_algorithm_best_fitness;
}

float TripleRomanDomination::getGeneticAlgorithmFitnessMean() {
    return this->genetic_algorithm_fitness_mean;
}

float TripleRomanDomination::getGeneticAlgorithmFitnessSTD() {
    return this->genetic_algorithm_fitness_std;
}
