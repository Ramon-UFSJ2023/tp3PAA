#include "huff.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_CARACTERES 256
#define MAX_CODIGO_BITS (MAX_CARACTERES * CHAR_BIT)

//Estruturas auxiliares

typedef struct noHeap{
    NoHuff *noHuffMan;
}noHeap;

typedef struct filaPrioridade{
    noHeap *elementos;
    int tamanho;
    int capacidade;
} filaPrioridade;

filaPrioridade *CriaFila(int capacidade){
    filaPrioridade *filaCriada = (filaPrioridade*)malloc(sizeof(filaPrioridade));
    filaCriada->tamanho=0;
    filaCriada->capacidade = capacidade;
    filaCriada->elementos = (noHeap*)malloc(filaCriada->capacidade*sizeof(noHeap));
    return filaCriada;
}

void trocarNos(noHeap *a, noHeap *b){
    noHeap temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

void filaSubir(filaPrioridade *fila, int indice){
    int pai = (indice-1)/2;
    while(indice >0 && fila->elementos[indice].noHuffMan->frequencia <fila->elementos[pai].noHuffMan->frequencia){
        trocarNos(&fila->elementos[indice], &fila->elementos[pai]);
        indice =pai;
        pai = (indice-1)/2;
    }
}

void filaDescer(filaPrioridade *fila,int indice){
    int menor = indice;
    int esq = 2*indice +1;
    int dir = 2*indice+2;

    if(esq <fila->tamanho && fila->elementos[esq].noHuffMan->frequencia <fila->elementos[menor].noHuffMan->frequencia) menor = esq;
    if(dir <fila->tamanho && fila->elementos[dir].noHuffMan->frequencia < fila->elementos[menor].noHuffMan->frequencia) menor = dir;
    if(menor != indice){
        trocarNos(&fila->elementos[indice], &fila->elementos[menor]);
        filaDescer(fila, menor);
    }

}

void insereFilaPrioridade(filaPrioridade *fila, NoHuff *noNovo){
    if(fila->tamanho == fila->capacidade) return;
    fila->elementos[fila->tamanho].noHuffMan = noNovo;
    fila->tamanho++;
    filaSubir(fila, (fila->tamanho-1));
}

NoHuff *menorFila(filaPrioridade *fila){
    if(fila->tamanho == 0) return NULL;
    NoHuff *menNo = fila->elementos[0].noHuffMan;
    fila->elementos[0] = fila->elementos[fila->tamanho-1];
    fila->tamanho--;
    filaDescer(fila, 0);
    return menNo;
}

NoHuff *criaNo(unsigned char caractere, int frequencia, NoHuff *esquerda, NoHuff *direita){
    NoHuff *novoNo = (NoHuff*)malloc(sizeof(NoHuff));
    if(novoNo == NULL) exit(1);
    novoNo->caractere = caractere;
    novoNo->direita = direita;
    novoNo->esquerda = esquerda;
    novoNo->frequencia = frequencia;
    return novoNo;
}

void geraCod(NoHuff *no, char *caminho, int tamanhoCaminho, mapaCodigo *mapaCodSaida, int *indiceMapa){
    if(no->esquerda == NULL && no->direita == NULL){
        mapaCodSaida[*indiceMapa].caractere = no->caractere;
        mapaCodSaida[*indiceMapa].codigo = (char*)malloc(tamanhoCaminho+1);
        if(mapaCodSaida[*indiceMapa].codigo == NULL) exit(1);
        strncpy(mapaCodSaida[*indiceMapa].codigo, caminho, tamanhoCaminho);
        mapaCodSaida[*indiceMapa].codigo[tamanhoCaminho] = '\0';
        mapaCodSaida[*indiceMapa].tamanhoCodigo = tamanhoCaminho;
        (*indiceMapa)++;
        return;
    }
    if(no->esquerda){
        caminho[tamanhoCaminho] = '0';
        geraCod(no->esquerda, caminho, tamanhoCaminho+1, mapaCodSaida, indiceMapa);
    }
    if(no->direita){
        caminho[tamanhoCaminho] = '1';
        geraCod(no->direita, caminho, tamanhoCaminho+1, mapaCodSaida, indiceMapa);
    }
}

void costruirHuffGeraCodigo(const unsigned char *textoEntrada, size_t tamanhoArqTexto, NoHuff **raizArvore, mapaCodigo ** mapaCod, int *numCaracteresUnicos){
    int frequencias[MAX_CARACTERES] = {0};
    for(size_t i=0; i<tamanhoArqTexto;i++) frequencias[textoEntrada[i]]++;

    filaPrioridade *fila = CriaFila(MAX_CARACTERES);
    *numCaracteresUnicos = 0;

    for(int i=0;i<MAX_CARACTERES;i++){
        if(frequencias[i]>0){
            insereFilaPrioridade(fila, criaNo((unsigned char)i, frequencias[i], NULL, NULL));
            (*numCaracteresUnicos)++;
        }
    }
    while(fila->tamanho >1){
        NoHuff *esquerda = menorFila(fila);
        NoHuff *diretira = menorFila(fila);
        NoHuff *pai = criaNo('\0', esquerda->frequencia+diretira->frequencia, esquerda, diretira);
        insereFilaPrioridade(fila, pai);
    }
    *raizArvore = menorFila(fila);
    free(fila->elementos);
    free(fila);

    *mapaCod = (mapaCodigo*)malloc(*numCaracteresUnicos * sizeof(mapaCod));
    if(*mapaCod == NULL) exit(1);

    char caminho[MAX_CODIGO_BITS];
    int indiceMapaCodigo = 0;
    geraCod(*raizArvore, caminho, 0, *mapaCod, &indiceMapaCodigo);
}

static const mapaCodigo *encontraCod(unsigned char caractere, const mapaCodigo *mapaCod, int numeroCaracteresUnicos){
    for(int i=0; i<numeroCaracteresUnicos; i++){
        if(mapaCod[i].caractere == caractere){
            return &mapaCod[i];
        }
    }
    return NULL;
}