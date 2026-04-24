#ifndef ROTA_H
#define ROTA_H

typedef struct rota {
    char codigo[10];
    char nome[50]; 
    char origem[10];
    char destino[10];
    int horarioHoras;
    int horarioMinutos;
    int poltronasDisponiveis;
    float distanciaMilhas;
    int assentosOcupados[100]; // 100 assentos no avião
} Rota;

void cadastrarRota();
void inicializarRotasPreCadastradas();
Rota selecionarRota(Rota *conexoes, int numConexoes);
Rota buscarRotaPorCodigo(char *codigo);
void atualizarRota(Rota *rotaAtualizada);
int verificarConexao(char *origem, char *destino, Rota *conexoes, int maxConexoes);
int listarHorariosDisponiveis(char *origem, char *destino, Rota *conexoes, int numConexoes);
void escolherAssento(Rota *rota, char *data);

#endif