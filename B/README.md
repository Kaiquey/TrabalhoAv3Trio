# Módulo B - Histórico de Navegação Web (B1)

Este diretório contém a simulação de um histórico de navegador de internet utilizando **Pilhas Alocadas Dinamicamente**.

## 🛠️ O que foi feito (`main.c`)
O código utiliza duas pilhas principais (`historico` e `futuro`) para armazenar as páginas visitadas (estrutura contendo URL, título e data/hora).
As principais operações são:
- **Navegação (Push/Pop)**: A função "Avançar" remove (`Pop`) da pilha `futuro` e insere (`Push`) no `historico`. A função "Voltar" faz o caminho inverso.
- **Alocação Dinâmica**: As pilhas são inicializadas com um tamanho padrão. Se a capacidade (`capacity`) atingir o limite (`size`), o código utiliza `realloc` para dobrar o tamanho do vetor na memória.
- **Persistência de Dados**: Salva o histórico atual em `dados.csv` e carrega os registros na inicialização.

## ⚙️ Como Compilar e Executar
No terminal, acesse a pasta `B/` e utilize o GCC:

```bash
# Para compilar:
gcc main.c -o main

# Para executar no Linux:
./main

# Para executar no Windows:
main.exe