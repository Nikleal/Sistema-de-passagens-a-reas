#ifndef UTILS_H
#define UTILS_H

int ehNumero(char *str);
int validarData(char *data);
int validarCPF(char *cpf);
int validarDataNascimento(const char *data);
int ehFeriado(int dia, int mes);
int ehFinalSemana(int dia, int mes, int ano);
int diasParaViagem(int diaViagem, int mesViagem, int anoViagem);
int diasRetorno(int diaIda, int mesIda, int anoIda, int diaVolta, int mesVolta, int anoVolta);
int funcaoDinheiro(void);

#endif