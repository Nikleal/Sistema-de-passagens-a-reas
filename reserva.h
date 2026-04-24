#ifndef RESERVA_H
#define RESERVA_H

typedef struct {
    char codigoRota[10];
    char data[11]; // Formato "DD/MM/AAAA"
    int assentosOcupados[100];
} Reserva;

void salvarReserva(char *codigoRota, char *data, int assentos[]);
void carregarReservas(char *codigoRota, char *data, int assentos[]);

#endif