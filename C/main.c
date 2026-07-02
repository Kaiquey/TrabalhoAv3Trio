#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

typedef struct
{
  int id;
  char arquivo[50];
  int paginas;
  char tipo;
} Trabalho;

Trabalho normal[MAX], prioritario[MAX];
int iniN = 0, fimN = 0, qtdN = 0;
int iniP = 0, fimP = 0, qtdP = 0;

void enfileirarNormal(Trabalho t)
{
  if (qtdN == MAX)
  {
    printf("Fila normal cheia!\n");
    return;
  }
  normal[fimN] = t;
  fimN = (fimN + 1) % MAX;
  qtdN++;
}

void enfileirarPrioritario(Trabalho t)
{
  if (qtdP == MAX)
  {
    printf("Fila prioritaria cheia!\n");
    return;
  }
  prioritario[fimP] = t;
  fimP = (fimP + 1) % MAX;
  qtdP++;
}

void adicionar()
{
  Trabalho t;
  printf("ID: ");
  scanf("%d", &t.id);
  printf("Arquivo: ");
  scanf(" %49[^\n]", t.arquivo);
  printf("Paginas: ");
  scanf("%d", &t.paginas);
  printf("Tipo (N/P): ");
  scanf(" %c", &t.tipo);

  if (t.tipo == 'P' || t.tipo == 'p')
    enfileirarPrioritario(t);
  else
    enfileirarNormal(t);
}

void processar()
{
  Trabalho t;
  if (qtdP > 0)
  {
    t = prioritario[iniP];
    iniP = (iniP + 1) % MAX;
    qtdP--;
  }
  else if (qtdN > 0)
  {
    t = normal[iniN];
    iniN = (iniN + 1) % MAX;
    qtdN--;
  }
  else
  {
    printf("Filas vazias!\n");
    return;
  }
  printf("Processado: %d - %s - %d paginas\n", t.id, t.arquivo, t.paginas);
}

void consultar()
{
  Trabalho t;
  if (qtdP > 0)
    t = prioritario[iniP];
  else if (qtdN > 0)
    t = normal[iniN];
  else
  {
    printf("Filas vazias!\n");
    return;
  }

  printf("Proximo: %d - %s - %d paginas\n", t.id, t.arquivo, t.paginas);
}

void listar()
{
  int i, pos;
  printf("\nPrioritarios:\n");
  for (i = 0; i < qtdP; i++)
  {
    pos = (iniP + i) % MAX;
    printf("%d %s %d\n", prioritario[pos].id, prioritario[pos].arquivo, prioritario[pos].paginas);
  }
  printf("\nNormais:\n");
  for (i = 0; i < qtdN; i++)
  {
    pos = (iniN + i) % MAX;
    printf("%d %s %d\n", normal[pos].id, normal[pos].arquivo, normal[pos].paginas);
  }
}

void buscar()
{
  int id, i, pos;
  printf("ID: ");
  scanf("%d", &id);

  for (i = 0; i < qtdP; i++)
  {
    pos = (iniP + i) % MAX;
    if (prioritario[pos].id == id)
    {
      printf("Encontrado na fila prioritaria\n");
      return;
    }
  }
  for (i = 0; i < qtdN; i++)
  {
    pos = (iniN + i) % MAX;
    if (normal[pos].id == id)
    {
      printf("Encontrado na fila normal\n");
      return;
    }
  }
  printf("Nao encontrado\n");
}

void cancelar()
{
  int id, i, pos, n = 0;
  Trabalho aux[MAX];
  printf("ID: ");
  scanf("%d", &id);

  for (i = 0; i < qtdP; i++)
  {
    pos = (iniP + i) % MAX;
    if (prioritario[pos].id != id)
      aux[n++] = prioritario[pos];
  }
  if (n != qtdP)
  {
    iniP = fimP = qtdP = 0;
    for (i = 0; i < n; i++)
      enfileirarPrioritario(aux[i]);
    printf("Cancelado.\n");
    return;
  }

  n = 0;
  for (i = 0; i < qtdN; i++)
  {
    pos = (iniN + i) % MAX;
    if (normal[pos].id != id)
      aux[n++] = normal[pos];
  }
  if (n != qtdN)
  {
    iniN = fimN = qtdN = 0;
    for (i = 0; i < n; i++)
      enfileirarNormal(aux[i]);
    printf("Cancelado.\n");
    return;
  }
  printf("Nao encontrado\n");
}

void salvar()
{
  FILE *f = fopen("fila.csv", "w");
  int i, pos;
  if (!f)
    return;
  fprintf(f, "id;arquivo;paginas;tipo\n");
  for (i = 0; i < qtdP; i++)
  {
    pos = (iniP + i) % MAX;
    fprintf(f, "%d;%s;%d;P\n", prioritario[pos].id, prioritario[pos].arquivo, prioritario[pos].paginas);
  }
  for (i = 0; i < qtdN; i++)
  {
    pos = (iniN + i) % MAX;
    fprintf(f, "%d;%s;%d;N\n", normal[pos].id, normal[pos].arquivo, normal[pos].paginas);
  }
  fclose(f);
}

void carregar()
{
  FILE *f = fopen("fila.csv", "r");
  Trabalho t;
  char lixo[100];
  if (!f)
  {
    printf("Arquivo nao encontrado\n");
    return;
  }
  iniN = fimN = qtdN = 0;
  iniP = fimP = qtdP = 0;
  fgets(lixo, 100, f);
  while (fscanf(f, "%d;%49[^;];%d;%c\n", &t.id, t.arquivo, &t.paginas, &t.tipo) == 4)
  {
    if (t.tipo == 'P')
      enfileirarPrioritario(t);
    else
      enfileirarNormal(t);
  }
  fclose(f);
}

int main()
{
  int op;
  do
  {
    printf("\n1-Adicionar\n2-Processar\n3-Consultar\n4-Listar\n5-Buscar\n6-Cancelar\n7-Salvar CSV\n8-Carregar CSV\n0-Sair\nOpcao: ");
    scanf("%d", &op);

    switch (op)
    {
    case 1:
      adicionar();
      break;
    case 2:
      processar();
      break;
    case 3:
      consultar();
      break;
    case 4:
      listar();
      break;
    case 5:
      buscar();
      break;
    case 6:
      cancelar();
      break;
    case 7:
      salvar();
      break;
    case 8:
      carregar();
      break;
    }
  } while (op != 0);
  return 0;
}