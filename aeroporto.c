#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "aeroporto.h"

void cadastrarAeroporto() {
    FILE *file = fopen("aeroportos.dat", "ab+"); // Abre o arquivo para leitura e escrita
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de aeroportos.\n");
        return;
    }

    Aeroporto aeroporto;
    char input[100]; // Buffer para leitura da entrada
    int codigoValido = 0;

    // Verifica se o código já foi cadastrado
    do {
        printf("Código do aeroporto (3 letras): ");
        fgets(input, sizeof(input), stdin); // Lê a entrada do usuário
        input[strcspn(input, "\n")] = 0; // Remove o '\n' do final

        // Verifica se a entrada tem exatamente 3 caracteres
        if (strlen(input) == 3 && 
            isalpha(input[0]) && 
            isalpha(input[1]) && 
            isalpha(input[2])) {

            // Copia os 3 caracteres para o código do aeroporto
            strncpy(aeroporto.codigo, input, 3);
            aeroporto.codigo[3] = '\0'; // Garante que o código termine com '\0'

            // Verifica se o código já existe no arquivo
            int codigoDuplicado = 0;
            Aeroporto temp;

            rewind(file); // Volta ao início do arquivo para verificar todos os registros
            while (fread(&temp, sizeof(Aeroporto), 1, file)) {
                if (strcmp(temp.codigo, aeroporto.codigo) == 0) {
                    codigoDuplicado = 1;
                    break;
                }
            }

            if (codigoDuplicado) {
                printf("Erro: O código do aeroporto já foi cadastrado.\n");
            } else {
                codigoValido = 1; 
            }
        } else {
            printf("Erro: O código do aeroporto deve ter exatamente 3 letras.\n");
        }
    } while (!codigoValido);

    printf("Nome do aeroporto: ");
    fgets(aeroporto.nome, sizeof(aeroporto.nome), stdin);
    aeroporto.nome[strcspn(aeroporto.nome, "\n")] = 0; // Remove o '\n' do final

    printf("Cidade: ");
    fgets(aeroporto.cidade, sizeof(aeroporto.cidade), stdin);
    aeroporto.cidade[strcspn(aeroporto.cidade, "\n")] = 0; // Remove o '\n' do final

    printf("Estado (UF): ");
    fgets(aeroporto.estado, sizeof(aeroporto.estado), stdin);
    aeroporto.estado[strcspn(aeroporto.estado, "\n")] = 0; // Remove o '\n' do final

    fwrite(&aeroporto, sizeof(Aeroporto), 1, file);
    fclose(file);

    printf("Aeroporto cadastrado com sucesso!\n");
}

void inicializarAeroportosPreCadastrados() {
    FILE *file = fopen("aeroportos.dat", "rb");
    if (file == NULL) {
        printf("Arquivo aeroportos.dat não encontrado. Criando...\n");
        file = fopen("aeroportos.dat", "wb");
        if (file == NULL) {
            printf("Erro ao criar o arquivo de aeroportos.\n");
            return;
        }
        fclose(file);
        file = fopen("aeroportos.dat", "rb");
    }

    fseek(file, 0, SEEK_END);
    if (ftell(file) == 0) {
        fclose(file);
        file = fopen("aeroportos.dat", "ab");

        Aeroporto aeroportos[] = {
            {"CGH", "Aeroporto de Congonhas", "São Paulo", "SP"},
            {"SDU", "Aeroporto Santos Dumont", "Rio de Janeiro", "RJ"},
            {"CNF", "Aeroporto Internacional de Belo Horizonte", "Belo Horizonte", "MG"},
            {"VIX", "Aeroporto de Vitória", "Vitória", "ES"},
            {"FOR", "Aeroporto Internacional Pinto Martins", "Fortaleza", "CE"},
            {"BSB", "Aeroporto Internacional de Brasília", "Brasília", "DF"},
            {"POA", "Aeroporto Internacional Salgado Filho", "Porto Alegre", "RS"},
            {"REC", "Aeroporto Internacional do Recife", "Recife", "PE"}
        };

        for (int i = 0; i < sizeof(aeroportos) / sizeof(aeroportos[0]); i++) {
            fwrite(&aeroportos[i], sizeof(Aeroporto), 1, file);
        }

        printf("Aeroportos pré-cadastrados adicionados com sucesso!\n");
    } else {
        printf("O arquivo aeroportos.dat já contém dados. Nenhum aeroporto pré-cadastrado foi adicionado.\n");
    }

    fclose(file);
}

void selecionarOrigemDestino(char *origem, char *destino) {
    FILE *file = fopen("aeroportos.dat", "rb");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de aeroportos.\n");
        return;
    }

    Aeroporto aeroporto;
    int contador = 1;
    int escolhaOrigem, escolhaDestino;

    printf("\nAeroportos disponíveis:\n");
    while (fread(&aeroporto, sizeof(Aeroporto), 1, file)) {
        printf("[%d] %s (%s) - %s/%s\n", contador, aeroporto.nome, aeroporto.codigo, aeroporto.cidade, aeroporto.estado);
        contador++;
    }
    fclose(file);

    printf("\nEscolha o aeroporto de origem (1 a %d): ", contador - 1);
    scanf("%d", &escolhaOrigem);
    printf("Escolha o aeroporto de destino (1 a %d): ", contador - 1);
    scanf("%d", &escolhaDestino);

    if (escolhaOrigem < 1 || escolhaOrigem >= contador || escolhaDestino < 1 || escolhaDestino >= contador) {
        printf("Escolha inválida. Tente novamente.\n");
        return;
    }

    file = fopen("aeroportos.dat", "rb");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de aeroportos.\n");
        return;
    }

    fseek(file, (escolhaOrigem - 1) * sizeof(Aeroporto), SEEK_SET);
    fread(&aeroporto, sizeof(Aeroporto), 1, file);
    strcpy(origem, aeroporto.codigo);

    fseek(file, (escolhaDestino - 1) * sizeof(Aeroporto), SEEK_SET);
    fread(&aeroporto, sizeof(Aeroporto), 1, file);
    strcpy(destino, aeroporto.codigo);

    fclose(file);
}