#include <stdio.h>
#include <stdlib.h>
#include "aeroporto.h"
#include "rota.h"
#include "cliente.h"
#include "funcionario.h"
#include "menu.h"

int main() {
    inicializarAeroportosPreCadastrados();
    inicializarRotasPreCadastradas();
    inicializarFuncionariosPreCadastrados();

    int opcao;
    do {
        menuOpcoes();
        scanf("%d", &opcao);
        switch (opcao) {
            case 1: menuConfiguracoes(); break;
            case 2: menuVendas(); break;
            case 3: printf("Saindo...\n"); break;
            default: printf("Opção inválida!\n");
        }
    } while (opcao != 3);
    
    printf("Processo finalizado. Obrigado por comprar conosco!\n");
    return 0;
}