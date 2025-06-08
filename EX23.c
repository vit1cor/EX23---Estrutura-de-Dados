#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* * Nomes e Contribuições:
 * * Henrique Corrêa Duarte - Matrícula:202310769 - E-mail: Henrique.202310769@unilasalle.edu.br - implementou as funções de busca (valor e nome)
 * Thiago Korrosky Kunrath - Matrícula:202312236 - E-mail: thiago.202312236@unilasalle.edu.br - implementou a função contabilidade e revisou a leitura de dados.
 * Victor Gomes Fanti - Matrícula: 202220831 - E-mail: victor.202220831@unilasalle.edu.br - implementou a função desaloca e testou todos os casos com arquivos txt.
 * */

// Arquivo com definições da estrutura e outros defines
#include "definicoes.h"

// Arquivo com as funções já programadas pelo professor
#include "funcoesProf.c"

#ifdef GABARITO
#include "resp.c"
#endif

// Protótipos das funções implementadas
ARVORE *busca_valor(ARVORE *r, int valor);
ARVORE *busca_nome(ARVORE *r, char *nome);
double contabiliza(ARVORE *r);
void desaloca(ARVORE *r);

/**
 * @brief Busca um nó na árvore a partir de uma chave (valor).
 * @param r Ponteiro para a raiz da árvore.
 * @param valor O valor a ser buscado.
 * @return Retorna o ponteiro para o nó encontrado ou NULL se não existir.
 */
ARVORE *busca_valor(ARVORE *r, int valor) {
    if (r == NULL) {
        return NULL;
    }
    if (r->valor == valor) {
        return r;
    }
    if (valor < r->valor) {
        return busca_valor(r->e, valor);
    } else {
        return busca_valor(r->d, valor);
    }
}

/**
 * @brief Busca um nó na árvore a partir de um nome.
 * @param r Ponteiro para a raiz da árvore.
 * @param nome A string com o nome a ser buscado.
 * @return Retorna o ponteiro para o nó encontrado ou NULL se não existir.
 */
ARVORE *busca_nome(ARVORE *r, char *nome) {
    if (r == NULL) {
        return NULL;
    }
    if (strcmp(r->nome, nome) == 0) {
        return r;
    }
    ARVORE *encontrado_esq = busca_nome(r->e, nome);
    if (encontrado_esq != NULL) {
        return encontrado_esq;
    }
    return busca_nome(r->d, nome);
}

/**
 * @brief Soma os salários de todos os nós da árvore.
 * @param r Ponteiro para a raiz da árvore.
 * @return A soma total (double) dos salários.
 */
double contabiliza(ARVORE *r) {
    if (r == NULL) {
        return 0.0;
    }
    return r->salario + contabiliza(r->e) + contabiliza(r->d);
}

/**
 * @brief Libera a memória de todos os nós da árvore (pós-ordem).
 * @param r Ponteiro para a raiz da árvore a ser desalocada.
 */
void desaloca(ARVORE *r) {
    if (r != NULL) {
        desaloca(r->e);
        desaloca(r->d);
        free(r);
    }
}

int main(int argc, char *argv[])
{
    ARVORE *r = NULL;
    ARVORE *no = NULL;
    ARVORE *resp = NULL;
    double totsal = 0.0;
    int total = 0;
    FILE *arq;
    int valor;
    char nome[MAXSTR + 1];

    if (argc < 2) {
        fprintf(stderr, "ERRO. Você precisa fornecer o nome do arquivo com os dados\n");
        fprintf(stderr, "Sintaxe: %s arquivo.txt\n", argv[0]);
        return 1;
    }

    arq = fopen(argv[1], "r");
    if (arq == NULL) {
        fprintf(stderr, "ERRO. Não pude abrir o arquivo \"%s\" para leitura\n", argv[1]);
        return 2;
    }

    while (!feof(arq)) {
        no = ledados(arq);
        if (no == NULL) {
            break;
        }
        if (r == NULL) {
            r = no;
            total = 1;
            continue;
        }
        if (insere(no, r) == NULL) {
            free(no);
        } else {
            total++;
        }
    }
    fclose(arq);

    printf("\n==================================\n");
    printf("Lido %d nos do arquivo \"%s\"", total, argv[1]);
    printf("\n==================================\n\n");

    printf("Depuração: percurso pre-ordem\n");
    pre(r);
    printf("\n\n");

    printf("Depuração: percurso em-ordem\n");
    em(r);
    printf("\n\n");

    printf("Depuração: percurso em pós-ordem\n");
    pos(r);
    printf("\n\n");

    printf("Digite um valor a ser buscado:\n");
    scanf("%d", &valor);
    getchar(); // Consome o \n deixado pelo scanf

    resp = busca_valor(r, valor);
    if (resp == NULL) {
        printf("%d não encontrado na árvore\n", valor);
    } else {
        printf("Achei %d:\n", valor);
        imprimeNO(resp);
    }

    printf("\nDigite um nome a ser encontrado:\n");
    lestringARQ(nome, MAXSTR, stdin);

    resp = busca_nome(r, nome);
    if (resp == NULL) {
        printf("'%s' não encontrado na árvore\n", nome);
    } else {
        printf("Achei '%s':\n", nome);
        imprimeNO(resp);
    }

    totsal = contabiliza(r);
    printf("\nTotal de salarios = R$ %.2lf\n", totsal);
    
    printf("\nDesalocando a arvore...\n");
    desaloca(r);
    r = NULL;
    printf("Arvore desalocada.\n");
    
    return 0;
}
