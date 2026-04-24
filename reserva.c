#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "reserva.h"

void salvarReserva(char *codigoRota, char *data, int assentos[]) {
    FILE *file = fopen("reservas.dat", "rb+");
    if (file == NULL) {
        file = fopen("reservas.dat", "wb"); // Cria o arquivo se não existir
    }

    Reserva reserva;
    int encontrado = 0;
    
    // Atualiza reserva existente
    while (fread(&reserva, sizeof(Reserva), 1, file)) {
        if (strcmp(reserva.codigoRota, codigoRota) == 0 && strcmp(reserva.data, data) == 0) {
            encontrado = 1;
            fseek(file, -sizeof(Reserva), SEEK_CUR);
            memcpy(reserva.assentosOcupados, assentos, sizeof(reserva.assentosOcupados));
            fwrite(&reserva, sizeof(Reserva), 1, file);
            break;
        }
    }

    fclose(file);

    if (!encontrado) {
        file = fopen("reservas.dat", "ab");
        strcpy(reserva.codigoRota, codigoRota);
        strcpy(reserva.data, data);
        memcpy(reserva.assentosOcupados, assentos, sizeof(reserva.assentosOcupados));
        fwrite(&reserva, sizeof(Reserva), 1, file);
        fclose(file);
    }
}


void carregarReservas(char *codigoRota, char *data, int assentos[]) {
    FILE *file = fopen("reservas.dat", "rb");
    if (file == NULL) {
        return; // Se não existir, nenhuma reserva foi feita ainda.
    }

    Reserva reserva;
    while (fread(&reserva, sizeof(Reserva), 1, file)) {
        if (strcmp(reserva.codigoRota, codigoRota) == 0 && strcmp(reserva.data, data) == 0) {
            memcpy(assentos, reserva.assentosOcupados, sizeof(reserva.assentosOcupados));
            fclose(file);
            return;
        }
    }

    fclose(file);
}