#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "menu.h"
#include "aeroporto.h"
#include "rota.h"
#include "cliente.h"
#include "funcionario.h"
#include "vendas.h"

void menuOpcoes() {
 printf("\n");
    printf("╔════════════════════════════════════════════╗\n");
    printf("║                                            ║\n");
    printf("║   BEM-VINDO AO SISTEMA DE PASSAGENS AÉREAS ║\n");
    printf("║                                            ║\n");
    printf("╠════════════════════════════════════════════╣\n");
    printf("║                                            ║\n");
    printf("║   [1] Configurações                        ║\n");
    printf("║   [2] Vendas                               ║\n");
    printf("║   [3] Sair                                 ║\n");
    printf("║                                            ║\n");
    printf("╚════════════════════════════════════════════╝\n");
    printf("\nDigite uma opção: ");
}

void menuConfiguracoes() {
    int opcao;

    do {
        printf("\n");
        printf("╔════════════════════════════════════════════╗\n");
        printf("║                                            ║\n");
        printf("║           MENU DE CONFIGURAÇÕES            ║\n");
        printf("║                                            ║\n");
        printf("╠════════════════════════════════════════════╣\n");
        printf("║                                            ║\n");
        printf("║   [1] Cadastrar Aeroportos                 ║\n");
        printf("║   [2] Cadastrar Rotas                      ║\n");
        printf("║   [3] Cadastrar Passageiros Fidelizados    ║\n");
        printf("║   [4] Alterar Dados de Passageiros         ║\n");
        printf("║   [5] Cadastrar Funcionários               ║\n");
        printf("║   [6] Pesquisar/Alterar Funcionários       ║\n");
        printf("║   [7] Voltar                               ║\n");
        printf("║                                            ║\n");
        printf("╚════════════════════════════════════════════╝\n");
        printf("\nEscolha uma opção: ");
        scanf("%d", &opcao);
        while (getchar() != '\n'); // Limpa o buffer de entrada

        switch (opcao) {
            case 1: cadastrarAeroporto(); break;
            case 2: cadastrarRota(); break;
            case 3: cadastrarPassageiro(); break;
            case 4: {
                int codigo;
                printf("Digite o código do passageiro que deseja alterar: ");
                scanf("%d", &codigo);
                while (getchar() != '\n'); // Limpa o buffer de entrada
                alterarDadosPassageiro(codigo);
                break;
            }
            case 5: cadastrarFuncionario(); break;
            case 6: {
                int matricula;
                printf("Digite a matrícula do funcionário que deseja alterar: ");
                scanf("%d", &matricula);
                while (getchar() != '\n'); // Limpa o buffer de entrada
                alterarFuncionario(matricula);
                break;
            }
            case 7: printf("Voltando...\n"); break;
            default: printf("Opção inválida!\n");
        }
    } while (opcao != 7);
}

void menuVendas() {
    char origem[10], destino[10];
    Rota rotaEscolhida;
    Rota conexoes[10];
    int numConexoes;
    Cliente cliente;
    char portao[10];
    char data[11], cpf[15];
    int dia, mes, ano;
    char resposta;
    int assentoEscolhido = -1; // Variável temporária para armazenar o assento escolhido

    // Etapa 1: Selecionar origem e destino
    selecionarOrigemDestino(origem, destino);

    // Verifica se há voos diretos ou conexões
    numConexoes = verificarConexao(origem, destino, conexoes, 10);

    // Etapa 2: Selecionar horário
    int numRotasDisponiveis = listarHorariosDisponiveis(origem, destino, conexoes, numConexoes);

    if (numRotasDisponiveis == 0) {
        printf("Não há voos disponíveis para a rota selecionada.\n");
        return;
    }

    rotaEscolhida = selecionarRota(conexoes, numConexoes);

    // Etapa 3: Solicitar a data do voo
    do {
        printf("\nInforme a data do voo (DD/MM/AAAA): ");
        scanf("%s", data);
        if (!validarData(data)) {
            printf("Erro: Data inválida! Certifique-se de que esteja no formato correto e após 15/03/2025.\n");
        }
    } while (!validarData(data));

    sscanf(data, "%d/%d/%d", &dia, &mes, &ano);

    // Etapa 4: Selecionar assento (sem marcar como ocupado ainda)
    printf("\nSelecione seu assento:\n");
    escolherAssento(&rotaEscolhida, data, &assentoEscolhido); // Passa o assento escolhido por referência

    // Etapa 5: Calcular preço do voo
    int diasParaViagemValor = diasParaViagem(dia, mes, ano);
    int ehFeriadoValor = ehFeriado(dia, mes);
    int ehFinalSemanaValor = ehFinalSemana(dia, mes, ano);
    int diasRetornoValor = 0;

    // Verifica se é uma viagem de ida e volta
    do {
        printf("\nÉ uma viagem de ida e volta? (s/n): ");
        scanf(" %c", &resposta);
        resposta = tolower(resposta);
        if (resposta != 's' && resposta != 'n') {
            printf("Opção inválida! Digite 's' para sim ou 'n' para não.\n");
        }
    } while (resposta != 's' && resposta != 'n');

    if (resposta == 's') {
        char dataVolta[11];
        do {
            printf("\nInforme a data de retorno (DD/MM/AAAA): ");
            scanf("%s", dataVolta);
            if (!validarData(dataVolta)) {
                printf("Erro: Data inválida! Certifique-se de que esteja no formato correto e após 15/03/2025.\n");
            }
        } while (!validarData(dataVolta));

        int diaVolta, mesVolta, anoVolta;
        sscanf(dataVolta, "%d/%d/%d", &diaVolta, &mesVolta, &anoVolta);

        diasRetornoValor = diasRetorno(dia, mes, ano, diaVolta, mesVolta, anoVolta);
    }

    float preco = calcularPreco(&rotaEscolhida, diasParaViagemValor, ehFeriadoValor, ehFinalSemanaValor, diasRetornoValor, rotaEscolhida.poltronasDisponiveis);
    printf("\nPreço da passagem: R$ %.2f\n", preco);

    // Etapa 6: Verificar status de Cliente Fiel
    // (Código existente para verificar cliente fiel e cadastrar passageiro)

    // Etapa 7: Realizar pagamento
    int pagamentoProcessado = 0;
    do {
        int opcaoPagamento;
        do {
            printf("\nEscolha a forma de pagamento:\n");
            printf("[1] Cartão de Crédito\n");
            printf("[2] Cartão de Débito\n");
            printf("[3] Dinheiro\n");
            printf("Escolha uma opção: ");
            scanf("%d", &opcaoPagamento);
        } while (opcaoPagamento < 1 || opcaoPagamento > 3);

        if (opcaoPagamento == 3) {
            pagamentoProcessado = funcaoDinheiro(); // Tenta processar o pagamento em dinheiro
            if (!pagamentoProcessado) {
                printf("Pagamento em dinheiro não processado. Escolha outra forma de pagamento.\n");
            }
        } else {
            printf("Pagamento com cartão processado com sucesso!\n");
            pagamentoProcessado = 1; // Pagamento processado com sucesso
        }
    } while (!pagamentoProcessado);

    // Etapa 8: Marcar o assento como ocupado após a confirmação do pagamento
    if (pagamentoProcessado && assentoEscolhido != -1) {
        int assentos[100] = {0};
        carregarReservas(rotaEscolhida.codigo, data, assentos); // Carrega os assentos ocupados
        assentos[assentoEscolhido - 1] = 1; // Marca o assento como ocupado
        salvarReserva(rotaEscolhida.codigo, data, assentos); // Salva a reserva no arquivo
        printf("Assento %d marcado como ocupado.\n", assentoEscolhido);
    }

    // Etapa 9: Capturar portão de embarque e gerar e-ticket
    printf("Informe o portão de embarque: ");
    scanf(" %9s", portao);

    gerarETicketTXT(&rotaEscolhida, &cliente, dia, mes, ano, portao, preco);

    // Atualiza a rota no arquivo após a venda
    atualizarRota(&rotaEscolhida);

    printf("Venda concluída com sucesso! E-ticket gerado.\n");
}