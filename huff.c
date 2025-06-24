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
