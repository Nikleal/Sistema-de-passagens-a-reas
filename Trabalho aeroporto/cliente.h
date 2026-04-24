#ifndef CLIENTE_H
#define CLIENTE_H

typedef struct cliente {
    int codigo;
    char nome[100];
    char RG[15];
    char CPF[15];
    char dataNascimento[11];
    char telefone[20];
    char email[100];
    char contatoEmergencia[100]; 
    char bagagemExtra;
} Cliente;

void cadastrarPassageiro();
void alterarDadosPassageiro(int codigo);
Cliente buscarClientePorCPF(char *cpf);
int validarCPF(const char *cpf);
int validarDataNascimento(const char *data);

#endif