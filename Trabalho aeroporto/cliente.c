#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cliente.h"

void cadastrarPassageiro() {
    FILE *file = fopen("clientes.dat", "ab+"); // Abre o arquivo para leitura e escrita
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de clientes.\n");
        return;
    }

    Cliente cliente;

    // Validação do código do passageiro (3 dígitos e único)
    int codigoValido = 0;
    do {
        printf("Código do passageiro (3 dígitos): ");
        scanf("%d", &cliente.codigo);
        getchar(); // Limpa o buffer

        // Verifica se o código tem exatamente 3 dígitos
        if (cliente.codigo < 100 || cliente.codigo > 999) {
            printf("Erro: O código deve ter exatamente 3 dígitos. Tente novamente.\n");
            continue;
        }

        // Verifica se o código já existe no arquivo
        int codigoExistente = 0;
        Cliente temp;
        rewind(file); // Volta ao início do arquivo para verificar todos os registros
        while (fread(&temp, sizeof(Cliente), 1, file)) {
            if (temp.codigo == cliente.codigo) {
                codigoExistente = 1;
                break;
            }
        }

        if (codigoExistente) {
            printf("Erro: Já existe um passageiro cadastrado com esse código. Tente novamente.\n");
        } else {
            codigoValido = 1; // Código válido e único
        }
    } while (!codigoValido);

    // Nome
    printf("Nome: ");
    fgets(cliente.nome, sizeof(cliente.nome), stdin);
    cliente.nome[strcspn(cliente.nome, "\n")] = 0; // Remove o '\n' do final

    // Validação do RG
    int rgValido = 0;
    do {
        printf("RG (formato: 00.000.000-0): ");
        scanf("%s", cliente.RG);
        getchar();

        // Verifica se o RG tem o formato correto
        if (strlen(cliente.RG) != 12 || cliente.RG[2] != '.' || cliente.RG[6] != '.' || cliente.RG[10] != '-') {
            printf("Erro: Formato de RG inválido! Use o formato 00.000.000-0. Tente novamente.\n");
            continue;
        }

        // Verifica se o RG já existe no arquivo
        int rgExistente = 0;
        Cliente temp;
        rewind(file); // Volta ao início do arquivo para verificar todos os registros
        while (fread(&temp, sizeof(Cliente), 1, file)) {
            if (strcmp(temp.RG, cliente.RG) == 0) {
                rgExistente = 1;
                break;
            }
        }

        if (rgExistente) {
            printf("Erro: Já existe um passageiro cadastrado com esse RG. Tente novamente.\n");
        } else {
            rgValido = 1; // RG válido e único
        }
    } while (!rgValido);

    // Validação do CPF
    int cpfValido = 0;
    do {
        printf("CPF (formato: 000.000.000-00): ");
        scanf("%s", cliente.CPF);
        getchar();

        // Verifica se o CPF é válido
        if (!validarCPF(cliente.CPF)) {
            printf("Erro: CPF inválido! Tente novamente.\n");
            continue; // Volta ao início do loop para pedir o CPF novamente
        }

        // Verifica se o CPF já existe no arquivo
        int cpfExistente = 0;
        Cliente temp;
        rewind(file); // Volta ao início do arquivo para verificar todos os registros
        while (fread(&temp, sizeof(Cliente), 1, file)) {
            if (strcmp(temp.CPF, cliente.CPF) == 0) {
                cpfExistente = 1;
                break;
            }
        }

        if (cpfExistente) {
            printf("Erro: Já existe um passageiro cadastrado com esse CPF. Tente novamente.\n");
        } else {
            cpfValido = 1; // CPF válido e único
        }
    } while (!cpfValido);

    // Validação da data de nascimento
    do {
        printf("Data de nascimento (DD/MM/AAAA): ");
        fgets(cliente.dataNascimento, sizeof(cliente.dataNascimento), stdin);
        cliente.dataNascimento[strcspn(cliente.dataNascimento, "\n")] = 0;

        if (!validarDataNascimento(cliente.dataNascimento)) {
            printf("Erro: Data de nascimento inválida! Tente novamente.\n");
        }
    } while (!validarDataNascimento(cliente.dataNascimento));

    // Telefone
    printf("Telefone: ");
    scanf("%s", cliente.telefone);
    getchar();

    // E-mail
    printf("E-mail: ");
    scanf("%s", cliente.email);
    getchar();

    // Contato de emergência
    printf("Contato de emergência (opcional, pressione Enter para ignorar): ");
    fgets(cliente.contatoEmergencia, sizeof(cliente.contatoEmergencia), stdin);
    cliente.contatoEmergencia[strcspn(cliente.contatoEmergencia, "\n")] = 0;

    // Bagagem extra
    printf("Bagagem extra acima de 10Kg? (S/N): ");
    scanf(" %c", &cliente.bagagemExtra);
    getchar();

    // Salva o passageiro no arquivo
    fwrite(&cliente, sizeof(Cliente), 1, file);
    fclose(file);

    printf("Passageiro cadastrado com sucesso!\n");
}

void alterarDadosPassageiro(int codigo) {
    FILE *file = fopen("clientes.dat", "rb+");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de clientes.\n");
        return;
    }

    Cliente cliente;
    int encontrado = 0;
    while (fread(&cliente, sizeof(Cliente), 1, file)) {
        if (cliente.codigo == codigo) {
            encontrado = 1;
            printf("Novo nome: ");
            scanf(" %[^\n]", cliente.nome);
            printf("Novo RG: ");
            scanf("%s", cliente.RG);
            printf("Novo CPF: ");
            scanf("%s", cliente.CPF);
            printf("Nova data de nascimento (DD/MM/AAAA): ");
            scanf("%s", cliente.dataNascimento);
            printf("Novo telefone: ");
            scanf("%s", cliente.telefone);
            printf("Novo e-mail: ");
            scanf("%s", cliente.email);
            printf("Novo contato de emergência: ");
            getchar();
            fgets(cliente.contatoEmergencia, sizeof(cliente.contatoEmergencia), stdin);
            printf("Bagagem extra acima de 10Kg? (S/N): ");
            scanf(" %c", &cliente.bagagemExtra);

            fseek(file, -sizeof(Cliente), SEEK_CUR);
            fwrite(&cliente, sizeof(Cliente), 1, file);
            break;
        }
    }
    fclose(file);

    if (encontrado)
        printf("Dados do passageiro alterados com sucesso!\n");
    else
        printf("Passageiro não encontrado!\n");
}

Cliente buscarClientePorCPF(char *cpf) {
    FILE *file = fopen("clientes.dat", "rb");
    Cliente cliente;
    
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de clientes.\n");
        cliente.codigo = -1; // Indica erro
        return cliente;
    }

    while (fread(&cliente, sizeof(Cliente), 1, file)) {
        if (strcmp(cliente.CPF, cpf) == 0) {
            fclose(file);
            return cliente;
        }
    }

    fclose(file);
    cliente.codigo = -1; // Cliente não encontrado
    return cliente;
}