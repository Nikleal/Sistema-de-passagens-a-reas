#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "vendas.h"

int gerarNumeroETicket(Cliente *cliente, Rota *rota, int dia, int mes, int ano) {
    srand(cliente->codigo + rota->codigo[0] * dia * mes * ano);
    return (rand() % 900000) + 100000; // Número entre 100000 e 999999
}

void gerarETicketTXT(Rota *rota, Cliente *cliente, int dia, int mes, int ano, char *portao, float preco) {
    char nomeArquivo[50];
    snprintf(nomeArquivo, sizeof(nomeArquivo), "eticket_%.20s_%d.txt", cliente->nome, rand() % 1000000);

    FILE *file = fopen(nomeArquivo, "w");
    if (file == NULL) {
        printf("Erro ao gerar o e-ticket.\n");
        return;
    }

    int numeroETicket = gerarNumeroETicket(cliente, rota, dia, mes, ano);

    // Salva no arquivo
    fprintf(file, "----------------------------------------\n");
    fprintf(file, "E-TICKET: %d\n", numeroETicket);
    fprintf(file, "Passageiro: %s\n", cliente->nome);
    fprintf(file, "CPF: %s\n", cliente->CPF);
    fprintf(file, "Data do voo: %02d/%02d/%04d\n", dia, mes, ano);
    fprintf(file, "Origem: %s  |  Destino: %s\n", rota->origem, rota->destino);
    fprintf(file, "Horário: %02d:%02d\n", rota->horarioHoras, rota->horarioMinutos);
    fprintf(file, "Código do voo: %s\n", rota->codigo);
    fprintf(file, "Portão de embarque: %s\n", portao);
    fprintf(file, "Preço da passagem: R$ %.2f\n", preco);
    fprintf(file, "----------------------------------------\n\n");
    fclose(file);

    // Exibe o e-ticket no terminal
    printf("\n======= E-TICKET GERADO =======\n");
    printf("E-TICKET: %d\n", numeroETicket);
    printf("Passageiro: %s\n", cliente->nome);
    printf("CPF: %s\n", cliente->CPF);
    printf("Data do voo: %02d/%02d/%04d\n", dia, mes, ano);
    printf("Origem: %s  |  Destino: %s\n", rota->origem, rota->destino);
    printf("Horário: %02d:%02d\n", rota->horarioHoras, rota->horarioMinutos);
    printf("Código do voo: %s\n", rota->codigo);
    printf("Portão de embarque: %s\n", portao);
    printf("Preço da passagem: R$ %.2f\n", preco);
    printf("================================\n");
}

float calcularPreco(Rota *rota, int diasParaViagem, int ehFeriado, int ehFinalSemana, int diasRetorno, int poltronasVagas) {
    float precoMilha;

    // Define o preço por milha com base na distância
    if (rota->distanciaMilhas <= 500) precoMilha = 0.36;
    else if (rota->distanciaMilhas <= 800) precoMilha = 0.29;
    else precoMilha = 0.21;

    // Define o fator de período com base nos dias para a viagem
    float fatorPeriodo;
    if (diasParaViagem <= 3) fatorPeriodo = 4.52;
    else if (diasParaViagem <= 6) fatorPeriodo = 3.21;
    else if (diasParaViagem <= 10) fatorPeriodo = 2.25;
    else if (diasParaViagem <= 15) fatorPeriodo = 1.98;
    else if (diasParaViagem <= 20) fatorPeriodo = 1.78;
    else if (diasParaViagem <= 30) fatorPeriodo = 1.65;
    else fatorPeriodo = 1.45;

    // Define o fator Duffs (feriado ou final de semana)
    float fatorDuffs = ehFeriado ? 3.56 : (ehFinalSemana ? 1.21 : 1.00);

    // Define o fator de retorno com base nos dias de retorno
    float fatorRetorno;
    if (diasRetorno == 0) {
        fatorRetorno = 1.00; // Viagem só de ida
    } else if (diasRetorno <= 2) {
        fatorRetorno = 1.09;
    } else if (diasRetorno <= 5) {
        fatorRetorno = 1.05;
    } else if (diasRetorno <= 8) {
        fatorRetorno = 1.02;
    } else {
        fatorRetorno = 1.00;
    }

    // Define o fator de procura com base na ocupação do voo
    float fatorProcura;
    float ocupacao = 100 - ((float)poltronasVagas / rota->poltronasDisponiveis) * 100;
    if (ocupacao > 90) fatorProcura = 0.75;
    else if (ocupacao > 70) fatorProcura = 0.85;
    else if (ocupacao > 60) fatorProcura = 0.95;
    else if (ocupacao > 40) fatorProcura = 1.00;
    else if (ocupacao > 20) fatorProcura = 1.15;
    else if (ocupacao > 10) fatorProcura = 1.20;
    else fatorProcura = 1.35;

    // Calcula o preço final
    return rota->distanciaMilhas * precoMilha * fatorPeriodo * fatorDuffs * fatorRetorno * fatorProcura;
}

int funcaoDinheiro() {
    FILE *file = fopen("funcionarios.dat", "rb+");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de funcionários. Criando novo arquivo...\n");
        file = fopen("funcionarios.dat", "wb");
        if (file == NULL) {
            printf("Erro ao criar o arquivo de funcionários.\n");
            return 0; // Pagamento não processado
        }
        fclose(file);
        file = fopen("funcionarios.dat", "rb+");
    }

    int matricula;
    int encontrado = 0;
    char confirmacao;

    printf("Informe o número de matrícula do funcionário: ");
    scanf("%d", &matricula);

    Funcionario funcionario;
    while (fread(&funcionario, sizeof(Funcionario), 1, file)) {
        if (funcionario.matricula == matricula) {
            encontrado = 1;
            printf("Confirmar pagamento? (s/n): ");
            scanf(" %c", &confirmacao);

            if (confirmacao == 's' || confirmacao == 'S') {
                printf("Pagamento confirmado!\n");
                fclose(file);
                return 1; // Pagamento processado com sucesso
            } else {
                printf("Pagamento não confirmado.\n");
                fclose(file);
                return 0; // Pagamento não processado
            }
        }
    }

    if (!encontrado) {
        printf("Funcionário não encontrado. Deseja cadastrar um novo funcionário? (s/n): ");
        scanf(" %c", &confirmacao);

        if (confirmacao == 's' || confirmacao == 'S') {
            // Cadastra um novo funcionário
            printf("\nCadastro de novo funcionário:\n");
            printf("Matrícula: %d\n", matricula);
            funcionario.matricula = matricula;

            printf("Nome: ");
            scanf(" %[^\n]", funcionario.nome);

            printf("Cargo: ");
            scanf(" %[^\n]", funcionario.cargo);

            // Adiciona o novo funcionário ao arquivo
            fseek(file, 0, SEEK_END); // Posiciona no final do arquivo
            fwrite(&funcionario, sizeof(Funcionario), 1, file);

            printf("Funcionário cadastrado com sucesso!\n");

            // Confirma o pagamento
            printf("Confirmar pagamento? (s/n): ");
            scanf(" %c", &confirmacao);

            if (confirmacao == 's' || confirmacao == 'S') {
                printf("Pagamento confirmado!\n");
                fclose(file);
                return 1; // Pagamento processado com sucesso
            } else {
                printf("Pagamento não confirmado.\n");
                fclose(file);
                return 0; // Pagamento não processado
            }
        } else {
            printf("Cadastro de funcionário cancelado. Pagamento não pode ser processado.\n");
            fclose(file);
            return 0; // Pagamento não processado
        }
    }

    fclose(file);
    return 0; // Pagamento não processado
}

void precoPassagem(double *preco, double Milhas, double Per, double Duffs, double ret, double proc, double dist) {
    double aux;
    aux = Milhas * Per * Duffs * ret * proc * dist;
    *preco = aux;
}