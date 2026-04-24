#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcionario.h"

void cadastrarFuncionario() {
    FILE *file = fopen("funcionarios.dat", "ab");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de funcionários.\n");
        return;
    }

    Funcionario funcionario;
    char input[20]; // Buffer para leitura da matrícula

    // Validação da matrícula (5 dígitos)
    int matriculaValida = 0;
    do {
        printf("Matrícula (5 dígitos): ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0; // Remove o '\n' do final

        // Verifica se a matrícula tem exatamente 5 dígitos e se é um número
        if (strlen(input) == 5 && ehNumero(input)) {
            funcionario.matricula = atoi(input); // Converte a string para inteiro
            matriculaValida = 1; // Marca a matrícula como válida
        } else {
            printf("Erro: A matrícula deve ter exatamente 5 dígitos.\n");
        }
    } while (!matriculaValida); // Repete até que a matrícula seja válida

    // Nome
    printf("Nome: ");
    fgets(funcionario.nome, sizeof(funcionario.nome), stdin);
    funcionario.nome[strcspn(funcionario.nome, "\n")] = 0; // Remove o '\n' do final

    // Cargo
    printf("Cargo: ");
    fgets(funcionario.cargo, sizeof(funcionario.cargo), stdin);
    funcionario.cargo[strcspn(funcionario.cargo, "\n")] = 0; // Remove o '\n' do final

    // Salva no arquivo
    fwrite(&funcionario, sizeof(Funcionario), 1, file);
    fclose(file);

    printf("Funcionário cadastrado com sucesso!\n");
}

void alterarFuncionario(int matricula) {
    FILE *file = fopen("funcionarios.dat", "rb+");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de funcionários.\n");
        return;
    }

    Funcionario funcionario;
    int encontrado = 0;
    while (fread(&funcionario, sizeof(Funcionario), 1, file)) {
        if (funcionario.matricula == matricula) {
            encontrado = 1;
            printf("Novo nome: ");
            scanf(" %[^\n]", funcionario.nome);
            printf("Novo cargo: ");
            scanf(" %[^\n]", funcionario.cargo);

            fseek(file, -sizeof(Funcionario), SEEK_CUR);
            fwrite(&funcionario, sizeof(Funcionario), 1, file);
            break;
        }
    }
    fclose(file);

    if (encontrado)
        printf("Dados do funcionário alterados com sucesso!\n");
    else
        printf("Funcionário não encontrado!\n");
}

void inicializarFuncionariosPreCadastrados() {
    FILE *file = fopen("funcionarios.dat", "rb");
    if (file == NULL) {
        printf("Arquivo funcionarios.dat não encontrado. Criando...\n");
        file = fopen("funcionarios.dat", "wb");
        if (file == NULL) {
            printf("Erro ao criar o arquivo de funcionários.\n");
            return;
        }
        fclose(file);
        printf("Arquivo funcionarios.dat criado com sucesso!\n");
    } else {
        printf("O arquivo funcionarios.dat já existe.\n");
        fclose(file);
    }
}