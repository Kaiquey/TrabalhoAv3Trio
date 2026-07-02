# Módulo A - Controle de Estoque (A2)

Este diretório contém a implementação de um sistema de Controle de Estoque utilizando **Listas Encadeadas Simples**.

## 🛠️ O que foi feito (`main.c`)
O programa gerencia produtos (compostos por código, nome, quantidade e preço), mantendo-os sempre ordenados pelo código de cadastro no momento da inserção. 
As principais funcionalidades implementadas são:
- **Inserção Ordenada**: Ao adicionar um produto, a lista é percorrida para encontrar a posição correta baseada no código.
- **Edição e Exclusão**: Manipulação de ponteiros (`atual` e `anterior`) para atualizar dados ou remover e liberar a memória (`free`) de um nó específico.
- **Persistência de Dados**: Leitura e gravação contínua dos produtos no arquivo `estoque.csv`.

## ⚙️ Como Compilar e Executar
No terminal, acesse a pasta `A/` e utilize o GCC:

```bash
# Para compilar:
gcc main.c -o main

# Para executar no Linux:
./main

# Para executar no Windows:
main.exe