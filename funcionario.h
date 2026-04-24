#ifndef FUNCIONARIO_H
#define FUNCIONARIO_H

typedef struct funcionario {
    int matricula;
    char nome[100];
    char cargo[50];
} Funcionario;

void cadastrarFuncionario();
void alterarFuncionario(int matricula);
void inicializarFuncionariosPreCadastrados();

#endif