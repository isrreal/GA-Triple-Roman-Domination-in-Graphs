#!/bin/bash

# Diretório base para salvar os resultados
BASE_DIR="single_flag_tests"
mkdir -p $BASE_DIR

# Função para rodar os testes de uma flag específica
run_flag_test() {
    FLAG_NAME=$1
    FLAG_INDEX=$2  # Índice da flag nos argumentos do programa (1 = elitismo, 2 = seleção, etc.)
    FLAG_VALUES=("0" "1")  # Valores possíveis para as flags (0 ou 1)

    echo "Rodando testes para a flag: $FLAG_NAME"

    # Cria a pasta para a flag
    FLAG_DIR="$BASE_DIR/$FLAG_NAME"
    mkdir -p $FLAG_DIR

    # Loop sobre os valores da flag
    for flag_value in "${FLAG_VALUES[@]}"; do
        # Define as flags: todas são 1, exceto a flag testada
        FLAGS=("1" "1" "1" "1")  # Padrão: todas as flags ativas
        FLAGS[$FLAG_INDEX]=$flag_value  # Altera apenas a flag testada

        # Nome do arquivo de saída
        OUTPUT_FILE="$FLAG_DIR/${FLAG_NAME}_${flag_value}.csv"
        echo "best_fitness,fitness_mean,fitness_std,elapsed_time(seconds)" > $OUTPUT_FILE

        # Executa o programa e salva a saída no arquivo CSV
        echo "Rodando com $FLAG_NAME = $flag_value"
        ./app "./input_file/random_graph11-order100-edge_probability-0.5.txt" "g100-.5" ${FLAGS[0]} ${FLAGS[1]} ${FLAGS[2]} ${FLAGS[3]} 0 >> $OUTPUT_FILE
    done
}

# Executa os testes para cada flag
run_flag_test "elitismo" 0  # Flag de elitismo (índice 0 nos argumentos)
run_flag_test "selecao" 1   # Flag de seleção (índice 1 nos argumentos)
run_flag_test "crossover" 2 # Flag de crossover (índice 2 nos argumentos)
run_flag_test "mutacao" 3   # Flag de mutação (índice 3 nos argumentos)

echo "Todos os testes foram concluídos!"
