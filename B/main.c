#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Macros
#define MAX_LINE_CHAR 256
#define CSV_HEADER "url;titulo;dataHora"
#define CSV_HEADER_LEN 21
#define WIPE_STDIN_GARBAGE(c)                 \
  while ((c = getchar()) != '\n' && c != EOF) \
    ;

typedef struct
{
  char url[100];
  char title[50];
  char dateHour[20];
} Page;

typedef struct Stack
{
  size_t size;
  size_t capacity;
  Page *historico;
} Stack;

/** utils */
void Print_Page(Page *page);
Page Make_Page();                                                 // Função que encapsula a recepção de inputs para a criação de uma page
void Input_Loop(const char *format, void *arg, const char *text); // Tratação de input
int Menu(const char *opts[], int quant, const char *title);       // Formata as opções e retorna a escolha do usuário
void Exit(int code);                                              // Função para terminar o programa
void verify_header();                                             // Garante que o arquivo CSV possue o cabeçário esperado.
void Get_Hour(char buffer[20]);

/** Pilha dinâmica */
_Bool Peek(Stack *self, Page *buffer); // Retorna o topo
_Bool Pop(Stack *self, Page *buffer);  // Retorna o topo e remove ele
void Push(Stack *self, Page page);     // Bota no topo, se a capacidade for atingida, realocará a pilha
void Listar(Stack *self);              // Lista a pilha

/** Alocação da pilha */
void NewStack(size_t size, Stack *pilha);
void DestructStack(Stack *stack);

/** CRUD */
_Bool Save_CSV(Stack *pilha); // vai salvar todas as páginas presente na stack
_Bool Load_CSV(Stack *pilha); // Vai carregar para a pilha as últimas 10 páginas presentes no csv

/** Dispatchers */
void main_menu();
void web_menu();
void stack_menu();
void crud_menu(Stack *pilha);

/** Funções do "Site" */
void Avançar();
void Voltar();

/** Variáveis globais. */
// Telas de opções
const char *_main_menu[3] = {"Site", "Crud", "Sair"};                   // tela principal que direciona para o site ou para as opções obrigatórias da tarefa
const char *_web_menu[3] = {"Avançar", "Voltar", "Tela Principal"};     // tela do site
const char *_stack_menu[3] = {"Historico", "Futuro", "Tela Principal"}; // opções para qual pilha utilizar as funções de crud
const char *_crud_menu[7] = {"Push", "Pop", "Peek", "Listar", "Salvar CSV", "Carregar CSV", "Tela Principal"};

Stack historico;
Stack futuro;
FILE *csv;

struct tm tm_info;

void main()
{
  time_t t = time(NULL);
  localtime_s(&tm_info, &t);

  NewStack(10, &historico);
  NewStack(10, &futuro);
  csv = fopen("dados.csv", "a+");
  if (csv == NULL)
  {
    printf("Erro ao abrir o arquivo.");
    Exit(1);
  }
  verify_header();
  main_menu();
}

void Avançar()
{
  Page buffer = {0};
  if (!Pop(&futuro, &buffer))
  {
    printf("Não há página para avançar.\n");
    return;
  }
  Get_Hour(buffer.dateHour);
  Push(&historico, buffer);
}
void Voltar()
{
  Page buffer = {0};
  if (!Pop(&historico, &buffer))
  {
    printf("Não há página para retornar.\n");
    return;
  }
  Get_Hour(buffer.dateHour);
  Push(&futuro, buffer);
}
void Get_Hour(char buffer[20])
{
  strftime(buffer, 20, "%Y-%m-%d %H:%M:%S", &tm_info);
}
void Exit(int code)
{
  printf("Saindo...");
  if (csv != NULL)
    fclose(csv);
  exit(code);
}
void Print_Page(Page *page)
{
  printf("|Url      : %50s|\n", page->url);
  printf("|Title    : %50s|\n", page->title);
  printf("|DateHour : %50s|\n", page->dateHour);
}
Page Make_Page()
{
  Page buffer = {0};
  Input_Loop(
      "%49[^\n]",
      &(buffer.title),
      "Digite o titulo da página: ");
  Input_Loop(
      "%99[^\n]",
      &(buffer.url),
      "Digite a url da página: ");
  printf("Digite o último horário de acesso ou -1 para um site nunca antes acessado");
  Input_Loop(
      "%19[^\n]",
      &(buffer.dateHour),
      "> ");
  return buffer;
}
void Input_Loop(const char *format, void *arg, const char *text)
{
Start:
  printf(text);
  if (scanf(format, arg) != 1)
  {
    char c;
    WIPE_STDIN_GARBAGE(c);
    goto Start;
  }
}
int Menu(const char *opts[], int quant, const char *title)
{
  printf("%10s%s%-10s\n", " ", title, " ");

  for (int i = 0; i < quant; i++)
  {
    printf("%d - %-10s\n", i + 1, opts[i]);
  }
  int opt;
Start:
  printf("> ");
  int ok = scanf("%d", &opt);

  char c;
  WIPE_STDIN_GARBAGE(c);

  if (opt > (quant) || opt <= 0 || ok != 1)
  {
    printf("Opção inválida!\n");
    goto Start;
  }
  return opt;
}
_Bool Peek(Stack *self, Page *buffer)
{
  if (self->size == 0)
    return false;
  // printf("Size: %d, Capacity: %d", self->size, self->capacity);
  // Print_Page(self->historico);
  *buffer = self->historico[self->size - 1];
  return true;
}
void Push(Stack *self, Page page)
{
  if (self->capacity == self->size)
  {
    self->capacity *= 2;
    Stack *new = realloc(self->historico, sizeof(Page) * self->capacity);
  }
  self->historico[self->size] = page;
  self->size += 1;
}
_Bool Pop(Stack *self, Page *buffer)
{
  if (self->size == 0)
    return false;
  self->size -= 1;
  *buffer = *(self->historico + self->size);
  Print_Page(buffer);
  return true;
}
void Listar(Stack *self)
{
  if (self->size == 0)
  {
    printf("Pilha está vazia.\n");
  }
  for (size_t i = self->size; i > 0; i--)
  {
    Print_Page(self->historico + i);
  }
}
void NewStack(size_t size, Stack *pilha)
{

  size_t b = sizeof(Page) * size;
  pilha->historico = malloc(b);
  pilha->capacity = size;
  pilha->size = 0;
};
void DestructStack(Stack *stack)
{
  free(stack);
}
void main_menu()
{
  switch (Menu(_main_menu, 3, "Main"))
  {
  case 1:
    web_menu();
    return;
  case 2:
    stack_menu();
    return;
  case 3:
    Exit(0);
  }
}
void web_menu()
{
Start:
  switch (Menu(_web_menu, 3, "Site"))
  {
  case 1:
    Avançar();
    goto Start;
  case 2:
    Voltar();
    goto Start;
  case 3:
    main_menu();
  }
}
void stack_menu()
{
Start:
  switch (Menu(_stack_menu, 3, "Site"))
  {
  case 1:
    crud_menu(&historico);
  case 2:
    crud_menu(&futuro);
  case 3:
    main_menu();
  }
}
void crud_menu(Stack *pilha)
{
  Page buffer = {0};
Start:
  switch (Menu(_crud_menu, 7, "CRUD"))
  {
  case 1:
    Page pag = Make_Page();
    Push(pilha, pag);

    goto Start;
  case 2:
    if (!Pop(pilha, &buffer))
    {
      printf("Erro ao executar Pop");
      goto Start;
    }
    Print_Page(&buffer);
    goto Start;
  case 3:
    if (!Peek(pilha, &buffer))
    {
      printf("Erro ao executar Peek");
    }
    Print_Page(&buffer);
    goto Start;
  case 4:
    Listar(pilha);
    goto Start;

  case 5:
    if (!Save_CSV(pilha))
    {
      printf("Erro ao Salvar no CSV");
      Exit(1);
    }
    goto Start;
  case 6:
    if (!Load_CSV(pilha))
    {
      printf("Erro ao Carregar do CSV");
      Exit(1);
    }
    goto Start;
  case 7:
  }
}
void verify_header()
{
  char buffer[20];
  fscanf(csv, "%19s", buffer);
  if (strcmp(buffer, CSV_HEADER) != 0)
  {
    fprintf(csv, CSV_HEADER);
    fflush(csv);
    return;
  }
}
_Bool Save_CSV(Stack *pilha)
{
  if (pilha->size == 0)
  {
    printf("Nada foi salvo, a pilha estava vazia\n");
    return true;
  }
  Page *page = pilha->historico;
  for (size_t i = 0; i < pilha->size; i++)
  {
    int ok = fprintf_s(csv, "%s;%s;%s\n", page->url, page->title, page->dateHour);
    if (ok < 0)
    {
      printf("Erro ao salvar os dados da página %s\n", page->title);
      return false;
    }
    page += i;
  }
  fflush(csv);

  printf("Dados salvos para o arquivo dados.csv!\n");

  return true;
}
_Bool Load_CSV(Stack *pilha)
{
  fseek(csv, 0, SEEK_SET);

  char buffer[256];
  if (fgets(buffer, (int)sizeof(buffer), csv) == NULL)
  {
    printf("Não há nenhum dado para ser carregado.\n");
    return true;
  }

  int count = 0;
  while (fgets(buffer, (int)sizeof(buffer), csv) != NULL && count < 10)
  {
    Page page = {0};
    int ok = sscanf_s(buffer, "%99[^;];%49[^;];%19[^\r\n]",
                      page.url, (unsigned)sizeof(page.url),
                      page.title, (unsigned)sizeof(page.title),
                      page.dateHour, (unsigned)sizeof(page.dateHour));

    if (ok == 3)
    {
      Push(pilha, page);
      count++;
    }
    else
    {
      return false;
    }
  }
  printf("Dados carregados para a pilha!\n");
  return true;
}