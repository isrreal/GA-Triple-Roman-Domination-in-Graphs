#include "GeneticAlgorithm.hpp"  
#include "TripleRomanDomination.hpp"
#include "Graph.hpp"             
#include "util_functions.hpp"
#include <chrono>

void printGeneticAlgorithmLog(short heuristic) {
	std::cout << "graph_name,graph_order,graph_size,graph_min_degree,graph_max_degree,fitness_heuristic_" << heuristic;
    std::cout << ",lower_bound,upper_bound,graph_density,elapsed_time(seconds),is_feasible\n";
}

void computeGeneticAlgorithm(TripleRomanDomination& trd, short heuristic, int upper_bound, int lower_bound, double graph_density) {
    std::chrono::duration<double> elapsed_time;
  
	auto start = std::chrono::high_resolution_clock::now();
	
	trd.runGeneticAlgorithm(heuristic);
	
	auto end = std::chrono::high_resolution_clock::now();
	
	elapsed_time = end - start;
  	   
	std::cout << trd.getGeneticAlgorithmBestFitness() << ',';
	std::cout << lower_bound << ',';
    std::cout << upper_bound << ',';  	
    std::cout << graph_density << ',';
	std::cout << elapsed_time.count() << ',';
	std::cout << feasible(trd.getGraph(), trd.getSolutionGeneticAlgorithm()) << '\n';
}

/*
    O Problema de Dominação Romana Tripla (PDRT) é uma variante do problema clássico de dominação em grafos.  
    Dado um grafo G = (V, E), define-se uma função de rotulagem h : V → {0, 1, 2, 3, 4}, chamada de  
    Função de Dominação Romana Tripla (FDRT), que deve satisfazer a seguinte condição:  
    - Todo vértice v ∈ V deve possuir pelo menos um vizinho rotulado com 3, ou a soma dos rótulos de seus vizinhos  
      deve ser pelo menos 3.  

    O objetivo é encontrar uma FDRT ótima, ou seja, uma atribuição de rótulos que minimize o peso total da função:  
    h(G) = ∑ h(v), para v ∈ V.  

    Esse problema é NP-completo e possui aplicações em alocação de recursos, redes de comunicação e estratégias  
    militares, onde é necessário proteger ou cobrir um conjunto de elementos de forma eficiente.  

    ### Abordagem Computacional  

    Esta implementação utiliza a meta-heurística de Algoritmos Genéticos (AG) para encontrar soluções viáveis  
    de menor custo para o PDRT. A abordagem se baseia na Teoria da Evolução de Darwin, onde soluções mais adaptadas  
    (com menor peso da função h) têm maior probabilidade de serem selecionadas para reprodução. O algoritmo faz uso  
    de operadores evolutivos como mutação, elitismo e cruzamento, além de introduzir variabilidade estocástica  
    através de funções que geram valores pseudoaleatórios.  

    ### Representação do Cromossomo  
    Um cromossomo é representado por um vetor de inteiros de tamanho |V| (ordem do grafo), onde cada posição  
    corresponde ao rótulo de um vértice. A aptidão (fitness) de um cromossomo é dada pela soma de seus rótulos.  

    O estudo busca analisar o impacto da variação dos parâmetros do Algoritmo Genético no desempenho da busca  
    por soluções ótimas, comparando diferentes configurações.  

    ### Etapas do Algoritmo  

    1. **Geração da População Inicial**  
       A população inicial é gerada utilizando três heurísticas distintas:  

       - **Heurística 1:**  
         Escolhe um vértice aleatório e atribui o rótulo 2, enquanto seus vizinhos recebem o rótulo 3.  
         Esses vértices são removidos de um grafo temporário (cópia do original), garantindo que a solução  
         respeite as restrições do problema.  

       - **Heurística 2:**  
         O grafo é clonado e reduzido iterativamente até sua ordem ser zero. Um vértice aleatório recebe  
         o rótulo 4, seus vizinhos são rotulados com 0 e removidos do grafo. Caso haja vértices isolados,  
         eles são rotulados com 3 e removidos. Por fim, uma rotina de otimização tenta reduzir os rótulos  
         sem violar as restrições.  

       - **Heurística 3:**  
         Ordena os vértices do grafo em ordem decrescente de grau. O vértice de maior grau recebe o rótulo 4  
         e seus vizinhos são rotulados com 0. Caso ainda restem vértices isolados, eles recebem o rótulo 3.  
         Assim como na Heurística 2, aplica-se uma etapa de refinamento para minimizar os rótulos da solução.  

       - **Heurística 4:**  
         Combina as três heurísticas anteriores, gerando a população inicial com 33% de cada abordagem.  

    2. **Avaliação de Aptidão**  
       Cada cromossomo é avaliado com base no peso total da FDRT. Soluções com menor custo são consideradas mais aptas.  

    3. **Seleção**  
       Métodos como roleta viciada ou torneio são utilizados para escolher indivíduos para a próxima geração.  

    4. **Operadores Genéticos**  
       - **Cruzamento:** Combina genes de dois indivíduos para gerar novas soluções. Pode ser realizado de duas formas:  
         - **Cruzamento de um ponto:** Uma posição aleatória é selecionada no vetor de genes, e todos os genes a partir dessa posição são permutados entre os indivíduos.  
         - **Cruzamento de dois pontos:** Duas posições aleatórias são escolhidas, e todos os genes entre essas posições são trocados entre os cromossomos.  

       - **Mutação:** Pequenas alterações são aplicadas a um indivíduo para manter a diversidade da população. O processo ocorre da seguinte forma:  
         - Um cromossomo é selecionado aleatoriamente.  
         - Uma posição aleatória dentro desse cromossomo é escolhida.  
         - O rótulo nessa posição é alterado para um valor aleatório dentre {0, 2, 3, 4}.  
         - Caso a nova solução não satisfaça as restrições da FDRT, a rotina `feasibilityCheck` é acionada para corrigir eventuais rótulos inválidos e garantir a viabilidade da solução.  

       - **Elitismo:** Garante que os melhores indivíduos sejam preservados entre gerações.  
         - A população é ordenada com base no valor de fitness de cada solução.  
         - Com base na taxa de elitismo e no tamanho da população, um número `k` de indivíduos é determinado.  
         - Somente os `k` melhores indivíduos (os primeiros na população ordenada) são mantidos para a próxima geração, garantindo que as soluções mais promissoras não sejam perdidas.  

    5. **Critério de Parada e Convergência**  
       O algoritmo evolui iterativamente até que um critério de parada seja atingido. Os principais critérios utilizados são:  
       - Número máximo de gerações atingido.  (utilizado neste trabalho).
       - Melhor solução não melhora após um número determinado de gerações consecutivas.  
       - A diversidade da população se torna muito baixa, resultando em estagnação da evolução.  

    ### Conclusão  

    O principal objetivo deste estudo é experimentar diferentes configurações de parâmetros do Algoritmo Genético  
    e analisar seu impacto na convergência para soluções de menor custo. Essa abordagem pode ser estendida para  
    outras variantes do problema de dominação e aplicações em otimização combinatória.  

    A implementação foi projetada para ser modular e flexível, permitindo ajustes nos operadores genéticos,  
    métodos de seleção e critérios de parada. Dessa forma, o algoritmo pode ser adaptado para diferentes  
    instâncias do problema e comparado com outras técnicas de otimização heurística e meta-heurística.  
*/


auto main(int argc, char** argv) -> int {

	// argumentos: caminho_do_grafo nome_do_grafo heurística(1-4)
    if (argc > 3) {
        Graph graph(argv[1]);
        
        if (graph.getOrder() == 0) {
            return -1;
        }
                     
    	constexpr size_t trial {15};
    	
    	// Genetic Algorithm parameters
    	
        size_t population_size { static_cast<size_t>(graph.getOrder() / 1) };
        constexpr size_t generations {601};
        short heuristic = std::stoi(argv[3]);
        constexpr float elitism_rate {0.4043};
        constexpr float mutation_rate {0.5362};
        constexpr float cross_over_rate {0.4095};
        size_t tournament_population_size {9};
        constexpr size_t max_no_improvement_iterations {79};      
        
        int upper_bound {0};
    	int lower_bound {0};
    
		upper_bound = computeRightUpperBound(graph, upper_bound);

		lower_bound = computeRightLowerBound(graph, lower_bound);

        size_t Delta { graph.getMaxDegree() };
       	size_t delta { graph.getMinDegree() };
       	
   	   	double graph_density { static_cast<double>(2 * graph.getSize()) / (graph.getOrder() * (graph.getOrder() - 1)) };
   	   	
    	TripleRomanDomination trd(graph, population_size, graph.getOrder(), generations,
            	mutation_rate, elitism_rate, cross_over_rate, tournament_population_size);
		 
		printGeneticAlgorithmLog(heuristic);
        
       	for (size_t i {0}; i < trial; ++i) {         	
        	std::cout << argv[2] << ',';
	        std::cout << graph.getOrder() << ',';
	        std::cout << graph.getSize() << ',';
	        std::cout << delta << ',';
	        std::cout << Delta << ',';
	        
            computeGeneticAlgorithm(trd, heuristic, upper_bound, lower_bound, graph_density);       
 		}
 		
    	return EXIT_SUCCESS;
	}
	
	return -1;
	
}
