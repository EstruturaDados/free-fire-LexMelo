
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// =======================================
// Definição das structs
// =======================================
typedef struct {
    char nome[30];
    char tipo[20];
    int quantidade;
} Item;

// Nó para lista encadeada
typedef struct No {
    Item dados;
    struct No *proximo;
} No;

// =======================================
// Variáveis globais para vetor
// =======================================
#define MAX_ITENS 10
Item mochilaVetor[MAX_ITENS];
int numItensVetor = 0;

// Contadores de comparações
int comparacoesSequencial = 0;
int comparacoesBinaria = 0;

// =======================================
// Funções auxiliares
// =======================================
void limparTela() {
    printf("\n----------------------------------------\n");
}

void exibirItem(Item item) {
    printf("Nome: %s | Tipo: %s | Quantidade: %d\n", item.nome, item.tipo, item.quantidade);
}

// =======================================
// FUNÇÕES VERSÃO VETOR
// =======================================
void inserirItemVetor() {
    if (numItensVetor >= MAX_ITENS) {
        printf("Mochila cheia!\n");
        return;
    }

    Item novo;
    printf("Digite o nome do item: ");
    scanf("%s", novo.nome);
    printf("Digite o tipo: ");
    scanf("%s", novo.tipo);
    printf("Digite a quantidade: ");
    scanf("%d", &novo.quantidade);

    mochilaVetor[numItensVetor++] = novo;
    printf("Item adicionado com sucesso!\n");
}

void listarItensVetor() {
    if (numItensVetor == 0) {
        printf("Mochila vazia!\n");
        return;
    }
    printf("\n--- Itens no Vetor ---\n");
    for (int i = 0; i < numItensVetor; i++) {
        exibirItem(mochilaVetor[i]);
    }
}

void removerItemVetor() {
    if (numItensVetor == 0) {
        printf("Mochila vazia!\n");
        return;
    }

    char nome[30];
    printf("Digite o nome do item a remover: ");
    scanf("%s", nome);

    for (int i = 0; i < numItensVetor; i++) {
        if (strcmp(mochilaVetor[i].nome, nome) == 0) {
            for (int j = i; j < numItensVetor - 1; j++) {
                mochilaVetor[j] = mochilaVetor[j + 1];
            }
            numItensVetor--;
            printf("Item removido!\n");
            return;
        }
    }
    printf("Item não encontrado!\n");
}

void ordenarVetorPorNome() {
    for (int i = 0; i < numItensVetor - 1; i++) {
        for (int j = 0; j < numItensVetor - i - 1; j++) {
            if (strcmp(mochilaVetor[j].nome, mochilaVetor[j + 1].nome) > 0) {
                Item temp = mochilaVetor[j];
                mochilaVetor[j] = mochilaVetor[j + 1];
                mochilaVetor[j + 1] = temp;
            }
        }
    }
    printf("Itens ordenados por nome!\n");
}

int buscarSequencialVetor(char nome[]) {
    comparacoesSequencial = 0;
    for (int i = 0; i < numItensVetor; i++) {
        comparacoesSequencial++;
        if (strcmp(mochilaVetor[i].nome, nome) == 0)
            return i;
    }
    return -1;
}

int buscarBinariaVetor(char nome[]) {
    comparacoesBinaria = 0;
    int inicio = 0, fim = numItensVetor - 1;

    while (inicio <= fim) {
        int meio = (inicio + fim) / 2;
        comparacoesBinaria++;

        int cmp = strcmp(mochilaVetor[meio].nome, nome);
        if (cmp == 0)
            return meio;
        else if (cmp < 0)
            inicio = meio + 1;
        else
            fim = meio - 1;
    }
    return -1;
}

// =======================================
// FUNÇÕES VERSÃO LISTA ENCADEADA
// =======================================
No* inserirItemLista(No *inicio) {
    No *novo = (No*)malloc(sizeof(No));
    printf("Digite o nome do item: ");
    scanf("%s", novo->dados.nome);
    printf("Digite o tipo: ");
    scanf("%s", novo->dados.tipo);
    printf("Digite a quantidade: ");
    scanf("%d", &novo->dados.quantidade);

    novo->proximo = inicio;
    printf("Item adicionado à lista!\n");
    return novo;
}

No* removerItemLista(No *inicio) {
    if (inicio == NULL) {
        printf("Lista vazia!\n");
        return NULL;
    }

    char nome[30];
    printf("Digite o nome do item a remover: ");
    scanf("%s", nome);

    No *atual = inicio, *anterior = NULL;

    while (atual != NULL) {
        if (strcmp(atual->dados.nome, nome) == 0) {
            if (anterior == NULL)
                inicio = atual->proximo;
            else
                anterior->proximo = atual->proximo;

            free(atual);
            printf("Item removido da lista!\n");
            return inicio;
        }
        anterior = atual;
        atual = atual->proximo;
    }

    printf("Item não encontrado!\n");
    return inicio;
}

void listarItensLista(No *inicio) {
    if (inicio == NULL) {
        printf("Lista vazia!\n");
        return;
    }

    printf("\n--- Itens na Lista Encadeada ---\n");
    No *p = inicio;
    while (p != NULL) {
        exibirItem(p->dados);
        p = p->proximo;
    }
}

void buscarSequencialLista(No *inicio) {
    comparacoesSequencial = 0;
    char nome[30];
    printf("Digite o nome do item a buscar: ");
    scanf("%s", nome);

    No *p = inicio;
    while (p != NULL) {
        comparacoesSequencial++;
        if (strcmp(p->dados.nome, nome) == 0) {
            printf("Item encontrado na lista!\n");
            exibirItem(p->dados);
            printf("Comparações: %d\n", comparacoesSequencial);
            return;
        }
        p = p->proximo;
    }

    printf("Item não encontrado!\n");
    printf("Comparações: %d\n", comparacoesSequencial);
}

// =======================================
// MENU PRINCIPAL
// =======================================
int main() {
    No *inicioLista = NULL;
    int opcao, estrutura, indice;
    char nomeBusca[30];

    do {
        limparTela();
        printf("=== Sistema de Mochila ===\n");
        printf("1. Usar Vetor\n");
        printf("2. Usar Lista Encadeada\n");
        printf("0. Sair\n");
        printf("Escolha a estrutura: ");
        scanf("%d", &estrutura);

        if (estrutura == 1) {
            do {
                printf("\n--- Mochila (Vetor) ---\n");
                printf("1. Inserir Item\n");
                printf("2. Remover Item\n");
                printf("3. Listar Itens\n");
                printf("4. Ordenar Itens por Nome\n");
                printf("5. Buscar Sequencial\n");
                printf("6. Buscar Binária\n");
                printf("0. Voltar\n");
                printf("Opção: ");
                scanf("%d", &opcao);

                switch (opcao) {
                    case 1: inserirItemVetor(); break;
                    case 2: removerItemVetor(); break;
                    case 3: listarItensVetor(); break;
                    case 4: ordenarVetorPorNome(); break;
                    case 5:
                        printf("Digite o nome do item: ");
                        scanf("%s", nomeBusca);
                        indice = buscarSequencialVetor(nomeBusca);
                        if (indice != -1) exibirItem(mochilaVetor[indice]);
                        else printf("Item não encontrado!\n");
                        printf("Comparações: %d\n", comparacoesSequencial);
                        break;
                    case 6:
                        printf("Digite o nome do item: ");
                        scanf("%s", nomeBusca);
                        indice = buscarBinariaVetor(nomeBusca);
                        if (indice != -1) exibirItem(mochilaVetor[indice]);
                        else printf("Item não encontrado!\n");
                        printf("Comparações: %d\n", comparacoesBinaria);
                        break;
                }
            } while (opcao != 0);
        }

        else if (estrutura == 2) {
            do {
                printf("\n--- Mochila (Lista Encadeada) ---\n");
                printf("1. Inserir Item\n");
                printf("2. Remover Item\n");
                printf("3. Listar Itens\n");
                printf("4. Buscar Sequencial\n");
                printf("0. Voltar\n");
                printf("Opção: ");
                scanf("%d", &opcao);

                switch (opcao) {
                    case 1: inicioLista = inserirItemLista(inicioLista); break;
                    case 2: inicioLista = removerItemLista(inicioLista); break;
                    case 3: listarItensLista(inicioLista); break;
                    case 4: buscarSequencialLista(inicioLista); break;
                }
            } while (opcao != 0);
        }

    } while (estrutura != 0);

    printf("\nSaindo do sistema... até a próxima batalha!\n");
    return 0;
}
