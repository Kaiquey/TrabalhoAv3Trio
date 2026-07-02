//Controle de Estoque

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <locale.h>

#define MAX 30

typedef struct Node{
    int codigo;
    char nome[50];
    int quantidade;
    float preco;
    struct Node *proximo;
}Produto;


void cadastrarProduto(Produto **init);
void listarProdutos(Produto *init);
void editarProdutos(Produto *init);
void excluirProduto(Produto **init);
void buscaProduto(Produto *init);
void salvarEstoque(Produto *init);
void carregarEstoque(Produto **init);
void liberarMemoria(Produto *init);


void menu(){
        system("clear || cls");
        printf("====Controle de Estoque====\n");
        printf("1- Cadastrar Produto \n");
        printf("2- Listar Estoque \n");
        printf("3- Buscar Produto \n");
        printf("4- Editar Produto \n");
        printf("5- Excluir Produto \n");
        printf("0- Sair \n");
}
void main()
{
    setlocale(LC_ALL, "Portuguese");
    Produto *Estoque_Lista = NULL;
    int opc;
    carregarEstoque(&Estoque_Lista);
    do{
        menu();
        printf("Escolha uma opção: ");
        scanf("%d", &opc);
        
        switch(opc){
            case 1:
                cadastrarProduto(&Estoque_Lista);
                printf("\n Pressione Enter para retornar ao menu");
                getchar(); getchar();
                break;
            case 2:
                listarProdutos(Estoque_Lista);
                printf("\n Pressione Enter para retornar ao menu");
                getchar(); getchar();
                break;
            case 3:
                system("clear || cls");
                buscaProduto(Estoque_Lista);
                printf("\n Pressione Enter para retornar ao menu");
                getchar(); getchar();
                break;
            case 4:
                system("clear || cls");
                editarProdutos(Estoque_Lista);
                printf("\n Pressione Enter para retornar ao menu");
                getchar(); getchar();
                break;
            case 5:
                system("clear || cls");
                excluirProduto(&Estoque_Lista);
                printf("\n Pressione Enter para retornar ao menu");
                getchar(); getchar();
                break;
            case 0: 
                printf("Saindo do sistema...\n");
                salvarEstoque(Estoque_Lista);
                liberarMemoria(Estoque_Lista);
                break;
            default:
                printf("Opção inválida! Tente novamente.\n");
        }
    }while(opc!=0);
    
}

void cadastrarProduto(Produto **init){
    int new_Code;
    
    printf("Digite o código do produto: ");
    scanf("%d",&new_Code);
    
    Produto *new = (Produto*) malloc (sizeof(Produto));
    if(new==NULL){
        printf("Erro: Falha na memória!\n");
        return;
    }
    new->codigo=new_Code;
    
    Produto *atual = *init;
    Produto *anterior = NULL;
    
    while(atual != NULL && atual->codigo < new_Code){
        anterior = atual;
        atual = atual->proximo;
    }
    
    if(atual!=NULL && atual->codigo==new_Code){
        printf("Erro: O código %d já existe no estoque!!\n", new_Code)   ;
        free(new);
        return;
    }
    
    printf("Digite o nome do Produto: ");
    scanf(" %[^\n]", new->nome);
    
    printf("Digite a quantidade: ");
    scanf("%d", &new->quantidade);
    
    printf("Digite o preço unitário: ");
    scanf("%f",&new->preco);
    
    if(anterior == NULL){
        new->proximo = *init;
        *init = new;
    }else{
        new->proximo = atual;
        anterior->proximo = new;
    }
    printf("Produto foi cadastrado com sucesso!\n");
    salvarEstoque(*init);
}

void editarProdutos(Produto *init){
    int cod_item;
    Produto *atual = init;
    if (init==NULL){
        printf("\n O estoque está vazio!\n");
        return;
    }
    printf("Digite o codigo do produto que deseja editar: ");
    scanf("%d",&cod_item);
    while(atual!=NULL && atual->codigo != cod_item){
        atual = atual->proximo;
    }
    if(atual != NULL){
        printf("Produto encontrado: %s\n",atual->nome);
        printf("Digite o Novo nome do Produto: ");
        scanf(" %[^\n]", atual->nome);//Utilizado para limpar o buffer de forma sutil antes de ler o nome
        printf("Digite a quantidade: ");
        scanf("%d", &atual->quantidade);
        printf("Digite o novo preço da unidade: ");
        scanf("%f", &atual->preco);
        printf("Produto atualizado com sucesso!\n");

        salvarEstoque(init);
    }else{
        printf("Erro: Produto com o código %d não encontrado.\n", cod_item);
    }
}

void excluirProduto(Produto **init){
    int cod_busca;
    
    if(*init == NULL){
        printf("\nO estoque está vazio.\n");
        return;
    }
    
    printf("Digite o código do produto que deseja excluir: ");
    scanf("%d", &cod_busca);
    
    Produto *atual = *init;
    Produto *anterior = NULL;
    
    while(atual!=NULL && atual->codigo != cod_busca){
        anterior = atual;
        atual = atual->proximo;
    }
    
    if (atual == NULL){
        printf("Erro: O protudo com código %d, não foi encontrado...\n", cod_busca);
        return;
    }
    
    if(anterior == NULL){
        *init = atual->proximo;
    }else{
        anterior->proximo = atual->proximo;
    }
    
    printf("Produto \"%s\" removido do estoque com sucesso.\n", atual->nome);
    free(atual);

    salvarEstoque(*init);
}

void buscaProduto(Produto *init){
    int cod_busca;
    if(init == NULL){
        printf("Estoque se encontra vazio.");
        return;
    }
    printf("Digite o código do produto para busca: ");
    scanf("%d",&cod_busca);
    
    Produto *atual = init;
    
    while(atual != NULL && atual->codigo != cod_busca){
        atual = atual->proximo;
    }
    
    if(atual != NULL){
        printf("Produto Encontrado\n");
        printf("Cod.: %d | Nome: %s | QTD: %d | Preco: R$ %.2f\n",atual->codigo,atual->nome,atual->quantidade,atual->preco);
    }else{
        printf("Erro: Produto com o código %d não foi encontrado.\n", cod_busca);
    }
}

void listarProdutos(Produto *init){
    if(init == NULL){
        printf("\nO estoque está vazio.\n");
        return;
    }
    
    Produto *atual = init;
    printf("--- Relatório do Estoque (Ordenado) ---\n");
    
    while(atual!=NULL){
        printf("Cód.: %d | Nome: %s | Qtd: %d | Preço: R$ %.2f\n", atual->codigo,atual->nome,atual->quantidade,atual->preco);
        atual = atual->proximo;
    }
}

void salvarEstoque(Produto *init){
    FILE *arquivo = fopen("estoque.csv", "w");
    if(arquivo == NULL){
        printf("Erro: Não foi possível salvar o arquivo de dados!\n");
        return;
    }
    Produto *atual = init;
    while(atual != NULL){
        fprintf(arquivo, "%d;%s;%d;%.2f\n",atual->codigo,atual->nome,atual->quantidade,atual->preco);
        atual = atual->proximo;
    }
    fflush(arquivo);
    fclose(arquivo);
    printf("Dados em \"estoque.csv\"!");
}

void carregarEstoque(Produto **init){
    FILE *arquivo = fopen("estoque.csv", "r");
    if(arquivo == NULL){
        return;
    }
    
    int cod, qtd;
    float prc;
    char nm[50];
    
    while(fscanf(arquivo,"%d;%[^;];%d;%f\n", &cod,nm,&qtd,&prc) == 4){
        Produto *new = (Produto*) malloc(sizeof(Produto));
        if(new==NULL){
            break;
        }
        new->codigo = cod;
        strcpy(new->nome,nm);
        new->quantidade = qtd;
        new->preco = prc;
        
        Produto *atual = *init;
        Produto *anterior = NULL;
        
        while(atual != NULL && atual->codigo <cod){
            anterior = atual;
            atual = atual->proximo;
        }
        
        if(anterior == NULL){
            new->proximo = *init;
            *init = new;
        }else{
            new->proximo = atual;
            anterior->proximo=new;
        }
    }
    fclose(arquivo);
}

void liberarMemoria(Produto *init){
    Produto *atual = init;
    Produto *proximo_no;
    
    while(atual != NULL){
        proximo_no = atual->proximo;
        free(atual);
        atual = proximo_no;
    }
}
