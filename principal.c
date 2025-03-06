#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct aeroporto{
    char codigo[10];
    char nome[50];
    char cidade[50];
    char estado[3];
} Aeroporto;

typedef struct rota{
    char codigo[10];
    char nome[50]; 
    char origem[10];
    char destino[10];
    int horarioHoras;
    int horarioMinutos;
    int poltronasDisponiveis;
    float distanciaMilhas;
} Rota;

typedef struct cliente{
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

typedef struct funcionario{
    int matricula;
    char nome[100];
    char cargo[50];
} Funcionario;


void menuConfiguracoes() {
    int opcao;
    do {
        printf("\nConfigurações:\n");
        printf("[1] Cadastrar Aeroportos\n");
        printf("[2] Cadastrar Rotas\n");
        printf("[3] Cadastrar passageiros fidelizados\n");
        printf("[4] Alterados dados de passageiros\n");
        printf("[5] Cadastrar Funcionários\n");
        printf("[6] Pesquisar/Alterar Funcionarios\n");
        printf("[7] Voltar\n");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1: cadastrarAeroporto(); break;
            case 2: cadastrarRota(); break;
            case 3: cadastrarPassageiro(); break;
            case 4: alterarDadosPassageiro(); break;
            case 5: cadastrarFuncionario(); break;
            case 6: alterarFuncionario(); break;
            case 7: break;
            default: printf("Opção inválida!\n");
        }
    } while(opcao != 7);
}

void cadastrarAeroporto() {
    FILE *file = fopen("aeroportos.dat", "ab");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de aeroportos.\n");
        return;
    }

    Aeroporto aeroporto;
    printf("Código do aeroporto: ");
    scanf("%s", aeroporto.codigo);
    printf("Nome do aeroporto: ");
    scanf(" %[^\n]", aeroporto.nome);
    printf("Cidade: ");
    scanf(" %[^\n]", aeroporto.cidade);
    printf("Estado (UF): ");
    scanf("%s", aeroporto.estado);

    fwrite(&aeroporto, sizeof(Aeroporto), 1, file);
    fclose(file);

    printf("Aeroporto cadastrado com sucesso!\n");
}

void cadastrarRota() {
    FILE *file = fopen("voos.dat", "ab");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de voos.\n");
        return;
    }

    Rota rota;
    printf("Código da rota: ");
    scanf("%s", rota.codigo);
    printf("Nome da rota (ex: São Paulo X Rio de Janeiro): ");
    scanf(" %[^\n]", rota.nome);
    printf("Código do aeroporto de origem: ");
    scanf("%s", rota.origem);
    printf("Código do aeroporto de destino: ");
    scanf("%s", rota.destino);
    printf("Horário (HH MM): ");
    scanf("%d %d", &rota.horarioHoras, &rota.horarioMinutos);
    printf("Número de poltronas disponíveis: ");
    scanf("%d", &rota.poltronasDisponiveis);
    printf("Distância em milhas: ");
    scanf("%f", &rota.distanciaMilhas);

    fwrite(&rota, sizeof(Rota), 1, file);
    fclose(file);

    printf("Rota cadastrada com sucesso!\n");
}

void cadastrarPassageiro() {
    FILE *file = fopen("clientes.dat", "ab");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de clientes.\n");
        return;
    }

    Cliente cliente;
    printf("Código do passageiro: ");
    scanf("%d", &cliente.codigo);
    printf("Nome: ");
    scanf(" %[^\n]", cliente.nome);
    printf("RG: ");
    scanf("%s", cliente.RG);
    printf("CPF: ");
    scanf("%s", cliente.CPF);
    printf("Data de nascimento (DD/MM/AAAA): ");
    scanf("%s", cliente.dataNascimento);
    printf("Telefone: ");
    scanf("%s", cliente.telefone);
    printf("E-mail: ");
    scanf("%s", cliente.email);
    printf("Contato de emergência (opcional, pressione Enter para ignorar): ");
    scanf(" %[^\n]", cliente.contatoEmergencia);
    printf("Bagagem extra acima de 10Kg? (S/N): ");
    scanf(" %c", &cliente.bagagemExtra);

    fwrite(&cliente, sizeof(Cliente), 1, file);
    fclose(file);

    printf("Passageiro cadastrado com sucesso!\n");
}

void alterarDadosPassageiro() {
    FILE *file = fopen("clientes.dat", "rb+");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de clientes.\n");
        return;
    }

    int codigo;
    Cliente cliente;
    printf("Informe o código do passageiro que deseja alterar: ");
    scanf("%d", &codigo);

    while (fread(&cliente, sizeof(Cliente), 1, file)) {
        if (cliente.codigo == codigo) {
            printf("Novo telefone: ");
            scanf("%s", cliente.telefone);
            printf("Novo e-mail: ");
            scanf("%s", cliente.email);

            fseek(file, -sizeof(Cliente), SEEK_CUR);
            fwrite(&cliente, sizeof(Cliente), 1, file);
            fclose(file);
            printf("Dados do passageiro alterados com sucesso!\n");
            return;
        }
    }

    fclose(file);
    printf("Passageiro não encontrado!\n");
}

void cadastrarFuncionario() {
    FILE *file = fopen("funcionarios.dat", "ab");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de funcionários.\n");
        return;
    }

    Funcionario funcionario;
    printf("Matrícula: ");
    scanf("%d", &funcionario.matricula);
    printf("Nome: ");
    scanf(" %[^\n]", funcionario.nome);
    printf("Cargo: ");
    scanf(" %[^\n]", funcionario.cargo);

    fwrite(&funcionario, sizeof(Funcionario), 1, file);
    fclose(file);

    printf("Funcionário cadastrado com sucesso!\n");
}

void alterarFuncionario() {
    FILE *file = fopen("funcionarios.dat", "rb+");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de funcionários.\n");
        return;
    }

    int matricula;
    Funcionario funcionario;
    printf("Informe a matrícula do funcionário que deseja alterar: ");
    scanf("%d", &matricula);

    while (fread(&funcionario, sizeof(Funcionario), 1, file)) {
        if (funcionario.matricula == matricula) {
            printf("Novo cargo: ");
            scanf(" %[^\n]", funcionario.cargo);

            fseek(file, -sizeof(Funcionario), SEEK_CUR);
            fwrite(&funcionario, sizeof(Funcionario), 1, file);
            fclose(file);
            printf("Dados do funcionário alterados com sucesso!\n");
            return;
        }
    }

    fclose(file);
    printf("Funcionário não encontrado!\n");
}


int main() {
    int opcao;
    do {
        printf("\nMenu Principal:\n");
        printf("[1] Configurações\n");
        printf("[2] Vendas\n");
        printf("[3] Sair\n");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1: menuConfiguracoes(); break;
            case 2: menuVendas(); break;
            case 3: printf("Saindo...\n"); break;
            default: printf("Opção inválida!\n");
        }
    } while(opcao != 3);
    return 0;
}