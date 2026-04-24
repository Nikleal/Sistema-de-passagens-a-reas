#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rota.h"

void cadastrarRota() {
    FILE *file = fopen("voos.dat", "ab+"); // Abre o arquivo para leitura e escrita
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de voos.\n");
        return;
    }

    Rota rota;

    // Conta o número de rotas já cadastradas
    fseek(file, 0, SEEK_END); // Vai para o final do arquivo
    int numRotas = ftell(file) / sizeof(Rota); // Calcula o número de rotas
    fseek(file, 0, SEEK_SET); // Volta para o início do arquivo

    // Gera o código da rota automaticamente
    snprintf(rota.codigo, sizeof(rota.codigo), "R%03d", numRotas + 1); // Formato R001, R002, etc.

    // Exibe o código gerado
    printf("Código da rota gerado: %s\n", rota.codigo);

    // Nome da rota
    printf("Nome da rota (ex: São Paulo X Rio de Janeiro): ");
    fgets(rota.nome, sizeof(rota.nome), stdin);
    rota.nome[strcspn(rota.nome, "\n")] = 0; // Remove o '\n' do final

    // Código do aeroporto de origem
    printf("Código do aeroporto de origem: ");
    fgets(rota.origem, sizeof(rota.origem), stdin);
    rota.origem[strcspn(rota.origem, "\n")] = 0; // Remove o '\n' do final

    // Código do aeroporto de destino
    printf("Código do aeroporto de destino: ");
    fgets(rota.destino, sizeof(rota.destino), stdin);
    rota.destino[strcspn(rota.destino, "\n")] = 0; // Remove o '\n' do final

    // Horário
    printf("Horário (HH MM): ");
    scanf("%d %d", &rota.horarioHoras, &rota.horarioMinutos);
    while (getchar() != '\n'); // Limpa o buffer de entrada

    // Número de poltronas disponíveis
    printf("Número de poltronas disponíveis: ");
    scanf("%d", &rota.poltronasDisponiveis);
    while (getchar() != '\n'); // Limpa o buffer de entrada

    // Distância em milhas
    printf("Distância em milhas: ");
    scanf("%f", &rota.distanciaMilhas);
    while (getchar() != '\n'); // Limpa o buffer de entrada

    // Inicializa todos os assentos como livres (0)
    for (int i = 0; i < 100; i++) {
        rota.assentosOcupados[i] = 0;
    }

    // Salva no arquivo
    fwrite(&rota, sizeof(Rota), 1, file);
    fclose(file);

    printf("Rota cadastrada com sucesso!\n");
}

void inicializarRotasPreCadastradas() {
    FILE *file = fopen("voos.dat", "rb");
    if (file == NULL) {
        printf("Arquivo voos.dat não encontrado. Criando...\n");
        file = fopen("voos.dat", "wb");
        if (file == NULL) {
            printf("Erro ao criar o arquivo de voos.\n");
            return;
        }
        fclose(file);
        file = fopen("voos.dat", "rb");
    }

    fseek(file, 0, SEEK_END);
    if (ftell(file) == 0) {
        fclose(file);
        file = fopen("voos.dat", "ab");

        Rota rotas[] = {
            {"R001", "São Paulo X Rio de Janeiro", "CGH", "SDU", 8, 0, 100, 222.0, {0}},
            {"R002", "São Paulo X Belo Horizonte", "CGH", "CNF", 9, 30, 100, 305.0, {0}},
            {"R003", "São Paulo X Vitória", "CGH", "VIX", 10, 0, 100, 462.0, {0}},
            {"R004", "Rio de Janeiro X Vitória", "SDU", "VIX", 11, 0, 100, 257.0, {0}},
            {"R005", "São Paulo X Fortaleza", "CGH", "FOR", 12, 0, 100, 1474.0, {0}},
            {"R006", "São Paulo X Brasília", "CGH", "BSB", 13, 0, 100, 541.0, {0}},
            {"R007", "Porto Alegre X São Paulo", "POA", "CGH", 14, 0, 100, 531.0, {0}},
            {"R008", "Vitória X Recife", "VIX", "REC", 15, 0, 100, 924.0, {0}}
        };

        for (int i = 0; i < sizeof(rotas) / sizeof(rotas[0]); i++) {
            fwrite(&rotas[i], sizeof(Rota), 1, file);
        }

        printf("Rotas pré-cadastradas adicionadas com sucesso!\n");
    } else {
        printf("O arquivo voos.dat já contém dados. Nenhuma rota pré-cadastrada foi adicionada.\n");
    }

    fclose(file);
}

Rota selecionarRota(Rota *conexoes, int numConexoes) {
    FILE *file = fopen("voos.dat", "rb");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de voos.\n");
        Rota rotaVazia = {0};
        return rotaVazia;
    }

    Rota rota;
    int contador = 1;
    int escolha;

    printf("\nRotas disponíveis:\n");

    // Exibe as conexões encontradas
    for (int i = 0; i < numConexoes; i += 2) {
        printf("[%d] Conexão: %s -> %s -> %s | Horário: %02d:%02d e %02d:%02d | Poltronas disponíveis: %d\n",
               contador, conexoes[i].origem, conexoes[i].destino, conexoes[i + 1].destino,
               conexoes[i].horarioHoras, conexoes[i].horarioMinutos,
               conexoes[i + 1].horarioHoras, conexoes[i + 1].horarioMinutos,
               conexoes[i].poltronasDisponiveis);
        contador++;
    }

    // Exibe as rotas diretas
    while (fread(&rota, sizeof(Rota), 1, file)) {
        printf("[%d] Código: %s | Nome: %s | Origem: %s | Destino: %s | Horário: %02d:%02d | Poltronas disponíveis: %d\n",
               contador, rota.codigo, rota.nome, rota.origem, rota.destino, rota.horarioHoras, rota.horarioMinutos, rota.poltronasDisponiveis);
        contador++;
    }
    fclose(file);

    printf("Escolha uma rota (1 a %d): ", contador - 1);
    scanf("%d", &escolha);

    // Verifica se a escolha é uma conexão
    if (escolha <= numConexoes / 2) {
        // Retorna a primeira parte da conexão
        return conexoes[(escolha - 1) * 2];
    }

    // Caso contrário, retorna uma rota direta
    file = fopen("voos.dat", "rb");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de voos.\n");
        Rota rotaVazia = {0};
        return rotaVazia;
    }

    Rota rotaEscolhida;
    fseek(file, (escolha - 1 - numConexoes / 2) * sizeof(Rota), SEEK_SET);
    fread(&rotaEscolhida, sizeof(Rota), 1, file);
    fclose(file);

    return rotaEscolhida;
}

Rota buscarRotaPorCodigo(char *codigo) {
    FILE *file = fopen("voos.dat", "rb");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de voos.\n");
        Rota rotaVazia = {0};
        return rotaVazia;
    }

    Rota rota;
    while (fread(&rota, sizeof(Rota), 1, file)) {
        if (strcmp(rota.codigo, codigo) == 0) {
            fclose(file);
            return rota;
        }
    }

    fclose(file);
    printf("Rota com código %s não encontrada.\n", codigo);
    Rota rotaVazia = {0};
    return rotaVazia;
}

void atualizarRota(Rota *rotaAtualizada) {
    FILE *file = fopen("voos.dat", "rb+");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de voos.\n");
        return;
    }

    Rota rota;
    while (fread(&rota, sizeof(Rota), 1, file)) {
        if (strcmp(rota.codigo, rotaAtualizada->codigo) == 0) {
            fseek(file, -sizeof(Rota), SEEK_CUR); // Volta para o início da rota
            fwrite(rotaAtualizada, sizeof(Rota), 1, file); // Atualiza a rota
            fclose(file);
            printf("Rota atualizada com sucesso!\n");
            return;
        }
    }

    fclose(file);
    printf("Rota com código %s não encontrada.\n", rotaAtualizada->codigo);
}

int verificarConexao(char *origem, char *destino, Rota *conexoes, int maxConexoes) {
    FILE *file = fopen("voos.dat", "rb");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de voos.\n");
        return 0;
    }

    Rota rotas[100]; // Armazena todas as rotas do arquivo
    int numRotas = 0;

    // Lê todas as rotas do arquivo
    while (fread(&rotas[numRotas], sizeof(Rota), 1, file)) {
        numRotas++;
        if (numRotas >= 100) { // Limite máximo de rotas
            break;
        }
    }
    fclose(file);

    int numConexoes = 0;

    // Verifica se há um voo direto
    for (int i = 0; i < numRotas; i++) {
        if (strcmp(rotas[i].origem, origem) == 0 && strcmp(rotas[i].destino, destino) == 0) {
            printf("Voo direto encontrado de %s para %s!\n", origem, destino);
            return 0; // Retorna 0 para indicar que há um voo direto
        }
    }

    // Verifica se há uma conexão com um voo intermediário
    for (int i = 0; i < numRotas; i++) {
        if (strcmp(rotas[i].origem, origem) == 0) {
            for (int j = 0; j < numRotas; j++) {
                if (strcmp(rotas[i].destino, rotas[j].origem) == 0 && strcmp(rotas[j].destino, destino) == 0) {
                    if (numConexoes < maxConexoes) {
                        // Adiciona a conexão à lista de conexões
                        conexoes[numConexoes] = rotas[i]; // Primeiro trecho
                        numConexoes++;
                        conexoes[numConexoes] = rotas[j]; // Segundo trecho
                        numConexoes++;
                    }
                    printf("Conexão encontrada: %s -> %s -> %s!\n", origem, rotas[i].destino, destino);
                }
            }
        }
    }

    return numConexoes; // Retorna o número de conexões encontradas
}

int listarHorariosDisponiveis(char *origem, char *destino, Rota *conexoes, int numConexoes) {
    FILE *file = fopen("voos.dat", "rb");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de voos.\n");
        return 0;
    }

    Rota rota;
    int contador = 0;

    printf("\nHorários disponíveis para %s -> %s:\n", origem, destino);

    // Exibe as conexões encontradas
    for (int i = 0; i < numConexoes; i += 2) {
        printf("[%d] Conexão: %s -> %s -> %s | Horário: %02d:%02d e %02d:%02d | Poltronas disponíveis: %d\n",
               contador + 1, conexoes[i].origem, conexoes[i].destino, conexoes[i + 1].destino,
               conexoes[i].horarioHoras, conexoes[i].horarioMinutos,
               conexoes[i + 1].horarioHoras, conexoes[i + 1].horarioMinutos,
               conexoes[i].poltronasDisponiveis);
        contador++;
    }

    // Exibe as rotas diretas
    while (fread(&rota, sizeof(Rota), 1, file)) {
        if (strcmp(rota.origem, origem) == 0 && strcmp(rota.destino, destino) == 0 && rota.poltronasDisponiveis > 0) {
            printf("[%d] Código: %s | Nome: %s | Horário: %02d:%02d | Poltronas disponíveis: %d\n",
                   contador + 1, rota.codigo, rota.nome, rota.horarioHoras, rota.horarioMinutos, rota.poltronasDisponiveis);
            contador++;
        }
    }

    fclose(file);

    return contador; // Retorna o número de rotas disponíveis
}

void escolherAssento(Rota *rota, char *data, int *assentoEscolhido) {
    int assentos[100] = {0};
    carregarReservas(rota->codigo, data, assentos); // Carrega os assentos ocupados

    while (1) {
        printf("\nAssentos disponíveis para o voo %s no dia %s:\n", rota->codigo, data);

        // Imprime os assentos disponíveis
        for (int i = 0; i < rota->poltronasDisponiveis; i++) {
            if (assentos[i] == 0) {
                printf("\033[34m[%02d] Livre\t\033[0m", i + 1); // Assento livre (azul)
            } else {
                printf("\033[31m[%02d] Ocupado\t\033[0m", i + 1); // Assento ocupado (vermelho)
            }
            if ((i + 1) % 5 == 0) printf("\n"); // Quebra a linha a cada 5 assentos
        }

        printf("\nEscolha um assento (1 a %d): ", rota->poltronasDisponiveis);
        scanf("%d", assentoEscolhido);

        // Verifica se o assento escolhido é válido
        if (*assentoEscolhido < 1 || *assentoEscolhido > rota->poltronasDisponiveis) {
            printf("Assento inválido. Tente novamente.\n");
        } else if (assentos[*assentoEscolhido - 1] == 1) {
            printf("Assento já ocupado. Tente novamente.\n");
        } else {
            printf("Assento %d selecionado com sucesso!\n", *assentoEscolhido);
            break;
        }
    }
}