# Módulo C - Fila de Impressão (C1)

Este diretório contém o gerenciamento de uma fila de impressão de documentos utilizando o conceito de **Filas Estáticas Circulares** (implementadas em vetores).

## 🛠️ O que foi feito (`main.c`)
O sistema organiza os trabalhos de impressão em duas filas globais (`normal` e `prioritario`), ambas com tamanho máximo fixo (`MAX = 100`).
Os destaques da implementação incluem:
- **Lógica Circular**: Utiliza o operador de módulo `%` (`fim = (fim + 1) % MAX`) para reaproveitar os espaços vazios no início do vetor após os itens serem processados.
- **Processamento Condicional**: A função de processamento verifica e esvazia primeiro a fila `prioritario`; apenas quando ela está vazia, os trabalhos da fila `normal` são removidos e processados.
- **Manipulação Interna**: Funções de busca e cancelamento iteram pelas filas respeitando os índices `ini` e `fim`, deslocando os itens quando ocorre um cancelamento para evitar buracos na estrutura.
- **Persistência**: Grava e lê a estrutura atual das filas no arquivo `fila.csv`.

## ⚙️ Como Compilar e Executar
No terminal, acesse a pasta `C/` e utilize o GCC:

```bash
# Para compilar:
gcc main.c -o main

# Para executar no Linux:
./main

# Para executar no Windows:
main.exe