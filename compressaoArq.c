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
    for(int i=0; i<MAXIMO_SIMBOLAS; i++){
        fila->dados[i] = NULL;
    }
    return fila;
}