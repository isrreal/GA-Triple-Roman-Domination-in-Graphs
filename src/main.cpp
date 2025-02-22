#include "GeneticAlgorithm.hpp"  
#include "TripleRomanDomination.hpp"
#include "Graph.hpp"             
#include "util_functions.hpp"
#include <chrono>

void printGeneticAlgorithmLog(short heuristic) {
	std::cout << "order,best_fitness,fitness_mean,fitness_std,elapsed_time(seconds)\n";
}

void computeGeneticAlgorithm(TripleRomanDomination& trd, short heuristic,
	bool flag_elitism, bool flag_selection, bool flag_crossover, bool flag_mutation) {
	
    std::chrono::duration<double> elapsed_time;
  
	auto start = std::chrono::high_resolution_clock::now();
	
	trd.runGeneticAlgorithm(heuristic, flag_elitism, flag_selection, flag_crossover, flag_mutation);
	
	auto end = std::chrono::high_resolution_clock::now();
	
	elapsed_time = end - start;
  	   
	std::cout << trd.getGeneticAlgorithmBestFitness() << ',';
	std::cout << trd.getGeneticAlgorithmFitnessMean() << ',';
	std::cout << trd.getGeneticAlgorithmFitnessSTD() << ',';

	std::cout << elapsed_time.count() << '\n';
}

/*
    O Problema de Dominação Romana Tripla (PDRT) é uma variante do problema clássico de dominação romana em grafos.  
    Dado um grafo G = (V, E), define-se uma função de rotulagem h : V → {0, 1, 2, 3, 4}, chamada de  
    Função de Dominação Romana Tripla (FDRT), que deve satisfazer a seguinte condição:  
    - Todo vértice v ∈ V deve possuir pelo menos um vizinho rotulado com 3, ou a soma dos rótulos de seus vizinhos  
      deve ser pelo menos 3.  

    O objetivo é encontrar uma FDRT ótima, ou seja, uma atribuição de rótulos que minimize o peso total da função:  
    h(G) = ∑ h(v), para v ∈ V.  

    Este problema é NP-completo e possui aplicações em alocação de recursos, redes de comunicação e estratégias  
    militares, onde é necessário proteger ou cobrir um conjunto de elementos de forma eficiente.  

    ### Abordagem Computacional  

    A implementação utiliza a meta-heurística de Algoritmos Genéticos (AG) para encontrar soluções viáveis  
    de menor custo para o PDRT. A abordagem baseia-se na Teoria da Evolução de Darwin, onde soluções mais adaptadas  
    (com menor peso da função h) têm maior probabilidade de serem selecionadas para reprodução. O algoritmo faz uso  
    de operadores evolutivos como mutação, elitismo e cruzamento, além de introduzir variabilidade estocástica  
    por meio de funções que geram valores pseudoaleatórios.  

    ### Representação do Cromossomo  
    Um cromossomo é representado por um vetor de inteiros de tamanho |V| (ordem do grafo), onde cada posição  
    corresponde ao rótulo de um vértice. A aptidão (fitness) de um cromossomo é dada pela soma de seus rótulos.  

    O estudo visa analisar o impacto da variação dos parâmetros do Algoritmo Genético no desempenho da busca  
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
         
    1.1 A heurística usada:
    	A heurística utilizada é a 4.

    2. **Avaliação de Aptidão**  
       Cada cromossomo é avaliado com base no peso total da FDRT. Soluções com menor custo são consideradas mais aptas.  

    3. **Seleção**  
       (flag = 1): torneio são selecionados k individuos aleatorios da população para "brigarem" entre si e quem "vencer" será escolhido. O critério da vitória é quem tem o menor fitness
       (flag = 0): Dois indivíduos da população são selecionados aleatoriamente.

    4. **Operadores Genéticos**  
       - **Cruzamento:** Combina genes de dois indivíduos para gerar novas soluções. Pode ser realizado de duas formas:  
         1: **Cruzamento de dois pontos (flag = 1): ** Duas posições aleatórias são escolhidas, e todos os genes entre essas posições são trocados entre os cromossomos.    
         0:  **Cruzamento de um ponto (flag = 0):** Uma posição aleatória é selecionada no vetor de genes, e todos os genes a partir dessa posição são permutados entre os indivíduos.

       - **Mutação:** Pequenas alterações são aplicadas a um indivíduo para manter a diversidade da população. O processo ocorre da seguinte forma:  
         - **Mutação Constante (flag = 1):** Uma posição do cromossomo (vetor de genes) é selecionada aleatoriamente, tendo seu valor alterado caso um número aleatório sorteado entre 0 e 1 supere a taxa de mutação definida nas entradas do algoritmo. O rótulo nessa posição é alterado para um valor aleatório dentre {0, 2, 3, 4}.  
         - **Mutação Linear (flag = 0):** O rótulo nessa posição é alterado para um valor aleatório dentre {0, 2, 3, 4} caso o valor de um número gerado entre 0 e 1 ultrapassar a taxa de mutação.  
         - Caso a nova solução não satisfaça as restrições da FDRT, a rotina `feasibilityCheck` é acionada para corrigir eventuais rótulos inválidos e garantir a viabilidade da solução.

       - **Elitismo:** Garante que os melhores indivíduos sejam preservados entre gerações.  
         - Possibilidade 1 (flag = 1): A população é ordenada com base no valor de fitness de cada solução. Com base na taxa de elitismo e no tamanho da população, um número `k` de indivíduos (representam os k primeiros, visto que está ordenado em ordem crescente com base em fitness).  
         - Possibilidade 2 (flag = 0): `k` clones do melhor cromossomo da população são mantidos são mantidos para a próxima geração.
         
    5. **Critério de Parada**  
       O algoritmo evolui iterativamente até que um critério de parada seja atingido. O critério utilizado é:  
       - Número máximo de gerações atingido (utilizado neste trabalho).  

    6. **Critérios de Análise das Soluções**  
       As soluções são analisadas com base no melhor fitness, na média de fitness e no desvio padrão de fitness a cada execução do Algoritmo Genético.
*/

auto main(int argc, char** argv) -> int {

    // argumentos: caminho_do_grafo nome_do_grafo flag_elitism flag_selection flag_crossover flag_mutation graph_order

    if (argc > 6) {
        Graph graph;

        // se for 0, então é a comparação normal
        bool part5 { std::stoi(argv[7]) > 0 ? true : false };

        // pega o grafo do arquivo, senão, gera um com `graph_order`
        if (part5) {
        	graph = Graph(std::stoi(argv[7]), 0.5);
        } 
        
        else {
            // grafo gerado aleatoriamente com (ordem, probabilidade de conexão entre arestas).
            graph = Graph(argv[1]);
        }

        if (graph.getOrder() == 0) {
            return -1;
        }

        constexpr size_t trial {20};

        // Genetic Algorithm parameters
        size_t population_size {100};
        constexpr size_t generations {1000};
        short heuristic {4};
        constexpr float elitism_rate {0.4043};
        constexpr float selection_rate {0.5};
        constexpr float crossover_rate {0.4095};
        constexpr float mutation_rate {0.5362};
        size_t tournament_population_size {9};

        // flags para teste
        bool flag_elitism { std::stoi(argv[3]) };
        bool flag_selection { std::stoi(argv[4]) }; 
        bool flag_crossover { std::stoi(argv[5]) };
        bool flag_mutation { std::stoi(argv[6]) };

        TripleRomanDomination trd(graph, population_size, graph.getOrder(), generations,
            elitism_rate, selection_rate, crossover_rate, mutation_rate, tournament_population_size);
		 
        printGeneticAlgorithmLog(heuristic);

        for (size_t i {0}; i < trial; ++i) {   
            computeGeneticAlgorithm(trd, heuristic, flag_elitism, flag_selection, flag_crossover, flag_mutation);       
        }

        return EXIT_SUCCESS;
    }

    return -1;
}

