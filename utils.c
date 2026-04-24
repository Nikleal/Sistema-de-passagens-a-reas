#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "utils.h"

int ehNumero(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit(str[i])) {
            return 0;  // Não é número
        }
    }
    return 1;  // É número
}

int validarData(char *data) {
    int dia, mes, ano;
    
    // Verifica se a entrada está no formato correto (10 caracteres, incluindo as barras)
    if (strlen(data) != 10 || data[2] != '/' || data[5] != '/') {
        return 0; // Formato inválido
    }

    // Extrai os valores numéricos da data
    if (sscanf(data, "%d/%d/%d", &dia, &mes, &ano) != 3) {
        return 0; // Falha na conversão dos valores
    }

    // Verifica se o ano, mês e dia estão dentro de faixas válidas
    if (ano < 2025 || mes < 1 || mes > 12 || dia < 1 || dia > 31) {
        return 0;
    }

    // Verifica meses com menos de 31 dias
    if ((mes == 4 || mes == 6 || mes == 9 || mes == 11) && dia > 30) {
        return 0;
    }

    // Verifica Fevereiro e anos bissextos
    if (mes == 2) {
        int bissexto = (ano % 4 == 0 && (ano % 100 != 0 || ano % 400 == 0));
        if (dia > (bissexto ? 29 : 28)) {
            return 0;
        }
    }

    // Verifica se a data é pelo menos 15/03/2025
    if (ano < 2025 || (ano == 2025 && (mes < 3 || (mes == 3 && dia < 15)))) {
        return 0; // Data inválida para voo
    }

    return 1; // Data válida
}

int validarCPF(char *cpf) {
    // Verifica se o CPF tem 14 caracteres e está no formato correto
    if (strlen(cpf) != 14 || cpf[3] != '.' || cpf[7] != '.' || cpf[11] != '-') {
        return 0; // Formato inválido
    }

    // Verifica se todos os demais caracteres são dígitos
    for (int i = 0; i < 14; i++) {
        if ((i != 3 && i != 7 && i != 11) && !isdigit(cpf[i])) {
            return 0; // Caracter inválido
        }
    }

    return 1; // CPF válido
}

int validarDataNascimento(const char *data) {
    int dia, mes, ano;

    // Verifica se o formato está correto (DD/MM/AAAA)
    if (sscanf(data, "%d/%d/%d", &dia, &mes, &ano) != 3) {
        return 0; // Formato inválido
    }

    // Verifica se o mês está correto
    if (mes < 1 || mes > 12) {
        return 0;
    }

    // Número de dias em cada mês
    int diasPorMes[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    // Ajusta para ano bissexto
    if ((ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0)) {
        diasPorMes[1] = 29;
    }

    // Verifica se o dia é válido para o mês
    if (dia < 1 || dia > diasPorMes[mes - 1]) {
        return 0;
    }

    return 1; // Data válida
}

int ehFeriado(int dia, int mes) {
    int feriados[][2] = {
        {1, 1},   // Ano Novo (01/01)
        {21, 4},  // Tiradentes (21/04)
        {1, 5},   // Dia do Trabalho (01/05)
        {7, 9},   // Independência do Brasil (07/09)
        {12, 10}, // Nossa Senhora Aparecida (12/10)
        {2, 11},  // Finados (02/11)
        {15, 11}, // Proclamação da República (15/11)
        {25, 12}  // Natal (25/12)
    };

    for (int i = 0; i < sizeof(feriados) / sizeof(feriados[0]); i++) {
        if (feriados[i][0] == dia && feriados[i][1] == mes) {
            return 1;
        }
    }
    return 0;
}

int ehFinalSemana(int dia, int mes, int ano) {
    struct tm tm = {0};
    tm.tm_mday = dia;
    tm.tm_mon = mes - 1;
    tm.tm_year = ano - 1900;

    mktime(&tm);
    return (tm.tm_wday == 0 || tm.tm_wday == 6);
}

int diasParaViagem(int diaViagem, int mesViagem, int anoViagem) {
    time_t agora;
    time(&agora);

    struct tm tmViagem = {0};
    tmViagem.tm_mday = diaViagem;
    tmViagem.tm_mon = mesViagem - 1;
    tmViagem.tm_year = anoViagem - 1900;

    time_t tempoViagem = mktime(&tmViagem);
    double diferenca = difftime(tempoViagem, agora);
    return (int)(diferenca / (60 * 60 * 24));
}


int diasRetorno(int diaIda, int mesIda, int anoIda, int diaVolta, int mesVolta, int anoVolta) {
    struct tm tmIda = {0};
    tmIda.tm_mday = diaIda;
    tmIda.tm_mon = mesIda - 1;
    tmIda.tm_year = anoIda - 1900;

    struct tm tmVolta = {0};
    tmVolta.tm_mday = diaVolta;
    tmVolta.tm_mon = mesVolta - 1;
    tmVolta.tm_year = anoVolta - 1900;

    time_t tempoIda = mktime(&tmIda);
    time_t tempoVolta = mktime(&tmVolta);

    double diferenca = difftime(tempoVolta, tempoIda);
    int dias = (int)(diferenca / (60 * 60 * 24));

    return dias > 0 ? dias : 0;
}