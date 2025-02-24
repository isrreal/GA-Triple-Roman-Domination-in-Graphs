#!/bin/bash

# Diretório base para salvar os resultados
BASE_DIR="incremental_order_tests"
mkdir -p $BASE_DIR

# Nome do arquivo de saída
OUTPUT_FILE="$BASE_DIR/results.csv"

# Cabeçalho do arquivo CSV
echo "order,best_fitness,fitness_mean,fitness_std,elapsed_time(seconds)" > $OUTPUT_FILE

# Valores fixos para as flags (ajuste conforme necessário)
FLAG_ELITISM=0
FLAG_SELECTION=1
FLAG_CROSSOVER=1
FLAG_MUTATION=0

# Loop para incrementar a ordem
for order in {10..100..10}; do  # Ajuste o intervalo e incremento conforme necessário
    echo "Rodando com ordem = $order"

    # Executa o algoritmo e redireciona a saída para um arquivo temporário
    ./app "" "" $FLAG_ELITISM $FLAG_SELECTION $FLAG_CROSSOVER $FLAG_MUTATION $order > temp_output.txt

    # Adiciona a ordem como a primeira coluna e salva no arquivo CSV
    awk -v order="$order" '{print order "," $0}' temp_output.txt >> $OUTPUT_FILE

    # Remove o arquivo temporário
    rm -f temp_output.txt
done

echo "Testes concluídos! Resultados salvos em $OUTPUT_FILE"
