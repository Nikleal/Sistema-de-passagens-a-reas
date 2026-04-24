# Sistema de Passagens Aéreas ✈️

Um sistema completo de gerenciamento de companhia aérea desenvolvido em **C**, rodando via terminal. O projeto gerencia desde o cadastro de aeroportos e aeronaves até a venda de passagens, com persistência de dados utilizando arquivos binários.

## 🚀 Funcionalidades

- **Configurações e Cadastros:**
  - Gerenciamento de Aeroportos (códigos IATA, cidades, estados).
  - Controle de Rotas e Voos (origem, destino, horários, milhas, assentos).
  - Cadastro de Passageiros e Funcionários.
- **Módulo de Vendas:**
  - Seleção de origem e destino com verificação de voos diretos e conexões.
  - Interface no terminal para seleção de assentos livres e ocupados.
  - Algoritmo dinâmico de precificação de passagens baseado em:
    - Distância em milhas.
    - Antecedência da compra.
    - Feriados e finais de semana.
    - Taxa de ocupação do voo.
  - Geração automática de E-Ticket em formato `.txt`.
- **Persistência de Dados:** Todos os cadastros e reservas são salvos e atualizados em arquivos `.dat`, garantindo que nada se perca ao fechar o programa.

## 🛠️ Tecnologias Utilizadas

- **Linguagem:** C
- **Conceitos:** Manipulação de Arquivos (Leitura/Escrita Binária e Texto), Structs, Ponteiros, Modularização, Lógica de Precificação Dinâmica.

## ⚙️ Como executar o projeto

Para compilar e executar o sistema no seu ambiente local (recomendado uso de ambiente Linux/Ubuntu), siga os passos abaixo:

1. Clone este repositório:
   ```bash
   git clone https://github.com/Nikleal/Sistema-de-passagens-aereas.git