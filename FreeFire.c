
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_COMPONENTES 20
#define NOME_TAM 30
#define TIPO_TAM 20

typedef struct {
    char nome[NOME_TAM];
    char tipo[TIPO_TAM];
    int prioridade; // 1 a 10
} Componente;

/* ------ Funções utilitárias ------ */

// remove \n do final de uma string obtida por fgets
void trim_newline(char *s) {
    size_t len = strlen(s);
    if (len == 0) return;
    if (s[len - 1] == '\n') s[len - 1] = '\0';
}

// mostra todos os componentes em formato de tabela
void mostrarComponentes(Componente vet[], int n) {
    if (n == 0) {
        printf("Nenhum componente cadastrado.\n");
        return;
    }
    printf("\n%-3s %-30s %-15s %-9s\n", "N", "NOME", "TIPO", "PRIOR.");
    printf("-----------------------------------------------------------------\n");
    for (int i = 0; i < n; ++i) {
        printf("%-3d %-30s %-15s %-9d\n", i + 1, vet[i].nome, vet[i].tipo, vet[i].prioridade);
    }
    printf("\n");
}

/* ------ Algoritmos de ordenação (cada um retorna comparações via ponteiro) ------ */

/*
  Bubble Sort por nome (ordena ascending A-Z)
  comparacoes: incrementado a cada comparação de strings
*/
void bubbleSortNome(Componente vet[], int n, long *comparacoes) {
    *comparacoes = 0;
    if (n <= 1) return;
    for (int i = 0; i < n - 1; ++i) {
        int trocou = 0;
        for (int j = 0; j < n - i - 1; ++j) {
            (*comparacoes)++;
            if (strcmp(vet[j].nome, vet[j + 1].nome) > 0) {
                Componente tmp = vet[j];
                vet[j] = vet[j + 1];
                vet[j + 1] = tmp;
                trocou = 1;
            }
        }
        if (!trocou) break; // otimização
    }
}

/*
  Insertion Sort por tipo (ordena ascending A-Z pelo campo tipo)
  comparacoes: incrementado a cada comparação de strings
*/
void insertionSortTipo(Componente vet[], int n, long *comparacoes) {
    *comparacoes = 0;
    for (int i = 1; i < n; ++i) {
        Componente chave = vet[i];
        int j = i - 1;
        // comparar tipos
        while (j >= 0) {
            (*comparacoes)++;
            if (strcmp(vet[j].tipo, chave.tipo) > 0) {
                vet[j + 1] = vet[j];
                j--;
            } else {
                break;
            }
        }
        vet[j + 1] = chave;
    }
}

/*
  Selection Sort por prioridade (ordena da maior prioridade para a menor)
  comparacoes: incrementado a cada comparação de inteiros
*/
void selectionSortPrioridade(Componente vet[], int n, long *comparacoes) {
    *comparacoes = 0;
    for (int i = 0; i < n - 1; ++i) {
        int idxMaior = i;
        for (int j = i + 1; j < n; ++j) {
            (*comparacoes)++;
            if (vet[j].prioridade > vet[idxMaior].prioridade) {
                idxMaior = j;
            }
        }
        if (idxMaior != i) {
            Componente tmp = vet[i];
            vet[i] = vet[idxMaior];
            vet[idxMaior] = tmp;
        }
    }
}

/* ------ Busca binária por nome (aplicável apenas se estiver ordenado por nome) ------ */
/* Retorna índice do componente ou -1 se não encontrado. Conta comparações em comparacoesBin. */
int buscaBinariaPorNome(Componente vet[], int n, const char chave[], long *comparacoesBin) {
    int inicio = 0, fim = n - 1;
    *comparacoesBin = 0;
    while (inicio <= fim) {
        int meio = (inicio + fim) / 2;
        (*comparacoesBin)++;
        int cmp = strcmp(chave, vet[meio].nome); // comparar chave com meio
        if (cmp == 0) return meio;
        else if (cmp < 0) fim = meio - 1;
        else inicio = meio + 1;
    }
    return -1;
}

/* ------ Função para medir tempo de execução de um algoritmo de ordenação ------ */
/*
  medidor recebe ponteiro para função de ordenação com assinatura:
    void algoritmo(Componente[], int, long*)
  Retorna tempo em segundos via ponteiro tempoSegs e comparações via comparacoes.
*/
typedef void (*AlgOrdFunc)(Componente[], int, long*);

double medirTempoOrdenacao(AlgOrdFunc algoritmo, Componente vet[], int n, long *comparacoes) {
    clock_t inicio = clock();
    algoritmo(vet, n, comparacoes);
    clock_t fim = clock();
    return (double)(fim - inicio) / (double)CLOCKS_PER_SEC;
}

/* ------ Função para clonar vetor (útil para comparar tempos sem alterar original) ------ */
void clonarVetor(Componente src[], Componente dst[], int n) {
    for (int i = 0; i < n; ++i) dst[i] = src[i];
}

/* ------ Entrada de dados (fgets) ------ */
void cadastrarComponentes(Componente vet[], int *n) {
    char buffer[128];
    *n = 0;
    printf("Quantos componentes deseja cadastrar? (1 a %d): ", MAX_COMPONENTES);
    if (!fgets(buffer, sizeof(buffer), stdin)) return;
    int q = atoi(buffer);
    if (q < 1) q = 1;
    if (q > MAX_COMPONENTES) q = MAX_COMPONENTES;

    for (int i = 0; i < q; ++i) {
        printf("\n--- Componente %d ---\n", i + 1);

        printf("Nome (ex: chip central): ");
        if (!fgets(vet[i].nome, NOME_TAM, stdin)) vet[i].nome[0] = '\0';
        trim_newline(vet[i].nome);

        printf("Tipo (ex: controle, suporte, propulsao): ");
        if (!fgets(vet[i].tipo, TIPO_TAM, stdin)) vet[i].tipo[0] = '\0';
        trim_newline(vet[i].tipo);

        // ler prioridade com fgets -> atoi
        printf("Prioridade (1 a 10): ");
        if (!fgets(buffer, sizeof(buffer), stdin)) buffer[0] = '\0';
        int p = atoi(buffer);
        if (p < 1) p = 1;
        if (p > 10) p = 10;
        vet[i].prioridade = p;
    }
    *n = q;
    printf("\nCadastro concluído: %d componentes registrados.\n", *n);
}

/* ------ Programa principal com menu ------ */
int main() {
    Componente componentes[MAX_COMPONENTES];
    Componente copia[MAX_COMPONENTES];
    int n = 0;
    int opcao;
    int ordenadoPorNome = 0; // flag que indica se o vetor atual está ordenado por nome

    printf("=== TORRE DE RESGATE: MÓDULO AVANÇADO ===\n\n");

    // cadastrar inicialmente
    cadastrarComponentes(componentes, &n);

    do {
        printf("\n--- MENU PRINCIPAL ---\n");
        printf("1 - Mostrar componentes\n");
        printf("2 - Ordenar com Bubble Sort (por NOME)\n");
        printf("3 - Ordenar com Insertion Sort (por TIPO)\n");
        printf("4 - Ordenar com Selection Sort (por PRIORIDADE)\n");
        printf("5 - Buscar componente-chave por NOME (Busca Binária) [requer ordenação por NOME]\n");
        printf("6 - Re-cadastrar componentes (limpar e inserir novos)\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        if (scanf("%d", &opcao) != 1) { // leitura defensiva
            while (getchar() != '\n'); // limpar stdin
            opcao = -1;
        }
        while (getchar() != '\n'); // limpar newline pendente

        if (opcao == 1) {
            mostrarComponentes(componentes, n);
        }
        else if (opcao == 2) {
            // bubble sort por nome - fazemos em cópia para mostrar tempo sem perder dados originais, mas
            // também aplicamos no original para que buscas posteriores assumam ordenação
            clonarVetor(componentes, copia, n);
            long comp = 0;
            double tempo = medirTempoOrdenacao(bubbleSortNome, copia, n, &comp);
            // aplicar no original também para consistência de fluxo
            bubbleSortNome(componentes, n, &comp);
            ordenadoPorNome = 1;
            printf("\nResultado (Bubble Sort por Nome):\n");
            mostrarComponentes(componentes, n);
            printf("Comparações: %ld | Tempo: %.6f segundos\n", comp, tempo);
        }
        else if (opcao == 3) {
            // insertion sort por tipo
            clonarVetor(componentes, copia, n);
            long comp = 0;
            double tempo = medirTempoOrdenacao(insertionSortTipo, copia, n, &comp);
            insertionSortTipo(componentes, n, &comp);
            ordenadoPorNome = 0; // pois ordenou por tipo, não por nome
            printf("\nResultado (Insertion Sort por Tipo):\n");
            mostrarComponentes(componentes, n);
            printf("Comparações: %ld | Tempo: %.6f segundos\n", comp, tempo);
        }
        else if (opcao == 4) {
            // selection sort por prioridade (maior -> menor)
            clonarVetor(componentes, copia, n);
            long comp = 0;
            double tempo = medirTempoOrdenacao(selectionSortPrioridade, copia, n, &comp);
            selectionSortPrioridade(componentes, n, &comp);
            ordenadoPorNome = 0;
            printf("\nResultado (Selection Sort por Prioridade - maior->menor):\n");
            mostrarComponentes(componentes, n);
            printf("Comparações: %ld | Tempo: %.6f segundos\n", comp, tempo);
        }
        else if (opcao == 5) {
            if (!ordenadoPorNome) {
                printf("Atenção: para usar busca binária é necessário ordenar por NOME (opção 2).\n");
            } else {
                char chave[NOME_TAM];
                printf("Digite o NOME do componente-chave a buscar: ");
                if (!fgets(chave, NOME_TAM, stdin)) chave[0] = '\0';
                trim_newline(chave);

                long compBin = 0;
                clock_t t0 = clock();
                int idx = buscaBinariaPorNome(componentes, n, chave, &compBin);
                clock_t t1 = clock();
                double tempo = (double)(t1 - t0) / (double)CLOCKS_PER_SEC;

                if (idx >= 0) {
                    printf("\nComponente-CHAVE encontrado na posição %d:\n", idx + 1);
                    mostrarComponentes(&componentes[idx], 1);
                    printf("Comparações na busca binária: %ld | Tempo: %.6f s\n", compBin, tempo);
                    printf(">> Componente confirmado. Você pode iniciar a montagem da torre!\n");
                } else {
                    printf("Componente não encontrado. Comparações: %ld | Tempo: %.6f s\n", compBin, tempo);
                }
            }
        }
        else if (opcao == 6) {
            // recadastrar
            cadastrarComponentes(componentes, &n);
            ordenadoPorNome = 0;
        }
        else if (opcao == 0) {
            printf("Encerrando o módulo. Boa sorte na fuga!\n");
        }
        else {
            printf("Opção inválida. Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}
