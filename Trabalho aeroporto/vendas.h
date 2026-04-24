#ifndef VENDAS_H
#define VENDAS_H

#include "rota.h"
#include "cliente.h"
#include "funcionario.h"

int gerarNumeroETicket(Cliente *cliente, Rota *rota, int dia, int mes, int ano);
void gerarETicketTXT(Rota *rota, Cliente *cliente, int dia, int mes, int ano, char *portao, float preco);
float calcularPreco(Rota *rota, int diasParaViagem, int ehFeriado, int ehFinalSemana, int diasRetorno, int poltronasVagas);

#endif