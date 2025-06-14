#ifndef COMPRESSAOARQ_H
#define COMPRESSAOARQ_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXIMO_SIMBOLAS 256
#define TAMANHO_MAXIMO_COD 64

typedef struct No{
    char *simbolo;
    unsigned int frequencia; // garante que não existe frequencia negativas
    struct No *esquerda, *direita;  
}No;

typedef struct FilaPrioridade{
    int tamanho;
    No* dados[MAXIMO_SIMBOLAS];
}FilaPrioridade;

typedef struct codificado{
    int comprimentoMaximo;
    char bitsCodificados[TAMANHO_MAXIMO_COD+1];
}codificado;


No* criarNo(char *simbolo, unsigned int frequencia);
FilaPrioridade* criarFila();
int insereFilaPrioridade(FilaPrioridade *fila, No* no);
No* extraiMinimo(FilaPrioridade *fila);
No* montaArvore(unsigned int tabelafrequencia[], int n);
void constroiTabelaCodificada(No* raiz, codificado tabela[], char buffer[], int profundidade);
void liberaArvore(No *raiz);


#endif