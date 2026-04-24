#ifndef AEROPORTO_H
#define AEROPORTO_H

typedef struct aeroporto {
    char codigo[10];
    char nome[50];
    char cidade[50];
    char estado[3];
} Aeroporto;

void cadastrarAeroporto();
void inicializarAeroportosPreCadastrados();
void selecionarOrigemDestino(char *origem, char *destino);

#endif