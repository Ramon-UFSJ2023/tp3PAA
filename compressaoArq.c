#include "compressaoArq.h"

No* criarNo(char* palavra, unsigned int frequencia){
    No* noCriado = malloc(sizeof(No));
    if(!noCriado) exit(1);

    if(palavra){
        noCriado->simbolo = strdup(palavra);
        if(!noCriado->simbolo){
            printf("Error.\n");
            free(noCriado);
            exit(1);
        }
    }else noCriado->simbolo = NULL;

    noCriado->frequencia =frequencia;
    noCriado->esquerda = NULL;
    noCriado->direita = NULL;
    return noCriado;
}

FilaPrioridade* criarFila(){
    FilaPrioridade *fila = malloc(sizeof(FilaPrioridade));
    if(!fila) exit(1);

    fila->tamanho=0;
    for(int i=0; i<MAXIMO_SIMBOLOS; i++){
        fila->dados[i] = NULL;
    }
    return fila;
}

int insereFilaPrioridade(FilaPrioridade *fila, No* noNovo){
    if(fila->tamanho >= MAXIMO_SIMBOLOS) exit(1);

    int posicaoFila = fila->tamanho;
    fila->dados[posicaoFila] = noNovo;
    fila->tamanho++;

    while(posicaoFila!= 0){
        int pai = (posicaoFila-1)/2;
        if(fila->dados[pai]->frequencia >= fila->dados[posicaoFila]->frequencia) break;

        No* temporario = fila->dados[posicaoFila];
        fila->dados[posicaoFila] = fila->dados[pai];
        fila->dados[pai] = temporario;

        posicaoFila = pai;
    }

    return 1;
}
