#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// ==========================================================
//          ÁRVORE BINÁRIA (Mapa da Mansão – Novato)
// ==========================================================

typedef struct Sala {
    char nome[40];
    struct Sala *esq, *dir;
} Sala;

Sala* criarSala(const char* nome) {
    Sala* nova = (Sala*) malloc(sizeof(Sala));
    strcpy(nova->nome, nome);
    nova->esq = NULL;
    nova->dir = NULL;
    return nova;
}

Sala* montarMansao() {
    Sala* hall = criarSala("Hall de Entrada");

    hall->esq = criarSala("Sala de Estar");
    hall->dir = criarSala("Corredor Principal");

    hall->esq->esq = criarSala("Biblioteca");
    hall->esq->dir = criarSala("Cozinha");

    hall->dir->esq = criarSala("Quarto de Hóspedes");
    hall->dir->dir = criarSala("Porão");

    return hall;
}

void explorarMansao(Sala* atual) {
    char opcao;

    while (1) {
        if (atual == NULL) {
            printf("\nFim do caminho — sala inexistente.\n");
            return;
        }

        printf("\n=== Você está em: %s ===\n", atual->nome);
        printf("(e) Ir para a esquerda\n");
        printf("(d) Ir para a direita\n");
        printf("(s) Sair da exploração\n");
        printf("Escolha: ");
        scanf(" %c", &opcao);

        if (opcao == 'e') atual = atual->esq;
        else if (opcao == 'd') atual = atual->dir;
        else if (opcao == 's') return;
        else printf("Opção inválida!\n");
    }
}


// ==========================================================
//          ÁRVORE DE BUSCA (Pistas – Aventureiro)
// ==========================================================

typedef struct NoBST {
    char pista[40];
    struct NoBST *esq, *dir;
} NoBST;

NoBST* criarPista(char* pista) {
    NoBST* nova = (NoBST*) malloc(sizeof(NoBST));
    strcpy(nova->pista, pista);
    nova->esq = nova->dir = NULL;
    return nova;
}

NoBST* inserirBST(NoBST* raiz, char* pista) {
    if (raiz == NULL) return criarPista(pista);

    if (strcmp(pista, raiz->pista) < 0)
        raiz->esq = inserirBST(raiz->esq, pista);
    else
        raiz->dir = inserirBST(raiz->dir, pista);

    return raiz;
}

void emOrdem(NoBST* r) {
    if (r != NULL) {
        emOrdem(r->esq);
        printf("- %s\n", r->pista);
        emOrdem(r->dir);
    }
}

void encontrarPistasAuto(Sala* local, NoBST** raiz) {
    if (strcmp(local->nome, "Biblioteca") == 0)
        *raiz = inserirBST(*raiz, "Livro rasgado");

    if (strcmp(local->nome, "Cozinha") == 0)
        *raiz = inserirBST(*raiz, "Faca suja");

    if (strcmp(local->nome, "Quarto de Hóspedes") == 0)
        *raiz = inserirBST(*raiz, "Pulseira dourada");

    if (strcmp(local->nome, "Porão") == 0)
        *raiz = inserirBST(*raiz, "Pegadas de lama");
}


// ==========================================================
//          TABELA HASH (Suspeitos – Mestre)
// ==========================================================

#define TAM 10

typedef struct HashNode {
    char pista[40];
    char suspeito[40];
    struct HashNode* prox;
} HashNode;

HashNode* tabela[TAM];

int hashFunc(const char* chave) {
    int soma = 0;
    for (int i = 0; chave[i] != '\0'; i++)
        soma += chave[i];
    return soma % TAM;
}

void inserirHash(char* pista, char* suspeito) {
    int h = hashFunc(pista);

    HashNode* novo = (HashNode*) malloc(sizeof(HashNode));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->prox = tabela[h];
    tabela[h] = novo;
}

void mostrarHash() {
    printf("\n=== Relações Pista → Suspeito ===\n");
    for (int i = 0; i < TAM; i++) {
        HashNode* aux = tabela[i];
        if (aux != NULL) {
            printf("[%d]\n", i);
            while (aux != NULL) {
                printf("  %s → %s\n", aux->pista, aux->suspeito);
                aux = aux->prox;
            }
        }
    }
}

char* suspeitoMaisCitado() {
    static char nomeFinal[40];
    char nomes[30][40];
    int cont[30];
    int total = 0;

    for (int i = 0; i < 30; i++) cont[i] = 0;

    for (int i = 0; i < TAM; i++) {
        HashNode* aux = tabela[i];
        while (aux != NULL) {
            int pos = -1;
            for (int j = 0; j < total; j++) {
                if (strcmp(nomes[j], aux->suspeito) == 0)
                    pos = j;
            }
            if (pos == -1) {
                strcpy(nomes[total], aux->suspeito);
                cont[total]++;
                total++;
            } else cont[pos]++;

            aux = aux->prox;
        }
    }

    int max = 0, idx = 0;
    for (int i = 0; i < total; i++) {
        if (cont[i] > max) {
            max = cont[i];
            idx = i;
        }
    }

    strcpy(nomeFinal, nomes[idx]);
    return nomeFinal;
}


// ==========================================================
//                    MENU PRINCIPAL
// ==========================================================

int main() {
    Sala* mansao = montarMansao();
    NoBST* pistas = NULL;

    int opcao;
    Sala* atual = mansao;

    while (1) {
        printf("\n====== DETECTIVE QUEST ======\n");
        printf("1 - Explorar mansão\n");
        printf("2 - Ver pistas coletadas (BST)\n");
        printf("3 - Registrar pistas com suspeitos (Hash)\n");
        printf("4 - Mostrar tabela hash\n");
        printf("5 - Ver suspeito mais citado\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        if (opcao == 1) {
            explorarMansao(atual);
            encontrarPistasAuto(atual, &pistas);
        }
        else if (opcao == 2) {
            printf("\n=== Pistas encontradas ===\n");
            emOrdem(pistas);
        }
        else if (opcao == 3) {
            char pista[40], suspeito[40];
            printf("Pista: ");
            scanf(" %[^\n]", pista);
            printf("Suspeito: ");
            scanf(" %[^\n]", suspeito);
            inserirHash(pista, suspeito);
        }
        else if (opcao == 4) {
            mostrarHash();
        }
        else if (opcao == 5) {
            printf("\nSuspeito mais citado: %s\n", suspeitoMaisCitado());
        }
        else if (opcao == 0) {
            printf("\nSaindo...\n");
            break;
        }
        else {
            printf("Opção inválida!\n");
        }
    }

    return 0;
}