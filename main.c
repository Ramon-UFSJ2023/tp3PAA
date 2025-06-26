#include <stdio.h>
#include <stdlib.h>
#include "bmh.h"
#include "huff.h"

#define MAX_TAMANHO_PADRAO 256
#define MAX_OCORRENCIAS 10000

int main(int argc, char *argv[]){
    if(argc != 3){fprintf(stderr, "Error de leitura de entrada.\n");return 1;}
    char *nomeArqTexto = argv[1];
    char *nomeArqPadrao = argv[2];

    FILE *arqTextoOriginal = fopen(nomeArqTexto,"r");
    if(arqTextoOriginal == NULL){ perror("Erro ao abrir arquivo de texto"); return 1;}
    fseek(arqTextoOriginal,0,SEEK_END);
    long tamanhoArqTexto = ftell(arqTextoOriginal);
    fseek(arqTextoOriginal,0,SEEK_SET);

    unsigned char *textoOriginal = (unsigned char*)malloc(tamanhoArqTexto+1);
    if(textoOriginal == NULL){
        perror("Erro na alocação de memoria");
        fclose(arqTextoOriginal);
        return 1;
    }

    fread(textoOriginal,1,tamanhoArqTexto,arqTextoOriginal);
    textoOriginal[tamanhoArqTexto]='\0';
    fclose(arqTextoOriginal);

    NoHuff *raizArvoreHuff = NULL;
    mapaCodigo *mapaCodHuff = NULL;
    int numCaracteresUnicos = 0;

    unsigned char *textoComprimido = NULL;
    size_t tamTextoComprimidoByts = 0;
    size_t totalBitsTextoComprimido = 0;
    
    costruirHuffGeraCodigo(textoOriginal, tamanhoArqTexto, &raizArvoreHuff, &mapaCodHuff, &numCaracteresUnicos);
    if(raizArvoreHuff == NULL || mapaCodHuff == NULL){
        fprintf(stderr, "Erro na construção da Arvore ou do Mapa");
        free(textoOriginal);
        return 1;
    }

    int resultadoCompressao = comprimirTexto(textoOriginal, tamanhoArqTexto, mapaCodHuff, numCaracteresUnicos, &textoComprimido, &tamTextoComprimidoByts, &totalBitsTextoComprimido);
    if(resultadoCompressao != 0){
        fprintf(stderr, "Erro na compressão do texto.");
        free(textoOriginal);
        huffLiberarArvore(raizArvoreHuff);
        huffLiberarMapa(&mapaCodHuff, numCaracteresUnicos);
        return 1;
    }

    FILE *arqPadrao = fopen(nomeArqPadrao, "r");
    if(arqPadrao == NULL){
        perror("Erro abertura arquivo padrao.");
        free(textoOriginal);
        free(textoComprimido);
        huffLiberarArvore(raizArvoreHuff);
        huffLiberarMapa(&mapaCodHuff, numCaracteresUnicos);
        return 1;
    }
    char padraoBuffer[MAX_TAMANHO_PADRAO];
    size_t *ocoEncontrada = (size_t *)malloc(MAX_OCORRENCIAS*sizeof(size_t));
    if(ocoEncontrada == NULL){
        perror("Erro em alocar memoria de ocorrencias.");
        free(textoOriginal);
        free(textoComprimido);
        huffLiberarArvore(raizArvoreHuff);
        huffLiberarMapa(&mapaCodHuff, numCaracteresUnicos);
        fclose(arqPadrao);
        return 1;
    }

    while(fgets(padraoBuffer, sizeof(padraoBuffer), arqPadrao) != NULL){
        padraoBuffer[strcspn(padraoBuffer, "\n")] = '\0';
        const unsigned char *padraoAtual = (const unsigned char *)padraoBuffer;
        size_t tamanhoPadraoAtual = strlen((const char*) padraoAtual);

        if(tamanhoPadraoAtual == 0){printf("Padrao vazio, pulando.\n"); continue;}
        
        long long numCmpNaoCompri = 0;
        double tempoExeNCmp = 0;
        long long numCmpCompri = 0;
        double tempoExeCompri = 0;
        int numOco;

        printf("%s ", padraoAtual);
        numOco = bmhBuscaNaoComprimido(textoOriginal, tamanhoArqTexto, padraoAtual, tamanhoPadraoAtual, &numCmpNaoCompri, &tempoExeNCmp, ocoEncontrada, MAX_OCORRENCIAS);
        for(int i=0; i<numOco;i++){
            printf("%zu ", ocoEncontrada[i]);
        }printf("\n Comparacoes: %lld, Tempo: %.6fs\n", numCmpNaoCompri, tempoExeNCmp);


        //texto comprimido:

        unsigned char *padraoCmp = NULL;
        size_t tamanhoPadraoCmp = 0;

        int resultadoCmpPadrao = huffComprimirPadrao(padraoAtual, tamanhoPadraoAtual, mapaCodHuff, numCaracteresUnicos, &padraoCmp, &tamanhoPadraoCmp);
        if(resultadoCmpPadrao != 0){
            fprintf(stderr, "Erro ao comprimir padrao.");
            if(padraoCmp) free(padraoCmp);
            continue;
        }
        printf("%s (Busca Comprimida)", padraoAtual);
        numOco = bmhBuscaComprimido(textoComprimido, totalBitsTextoComprimido, padraoCmp, tamanhoPadraoCmp, &numCmpCompri, &tempoExeCompri, ocoEncontrada, MAX_OCORRENCIAS, raizArvoreHuff);
        if(numOco == -1){
            fprintf(stderr, "Erro na busca comprimida para o padrao.\n");
        }else{
            for(int i=0; i<numOco;i++){
                printf("%zu ", ocoEncontrada[i]);
            }
            printf("\n Comparacoes: %lld, Tempo: %.6fs\n", numCmpCompri, tempoExeCompri);
        }
        if(padraoCmp){
            free(padraoCmp);
            padraoCmp = NULL;
        }
        printf("\n");
    }
    fclose(arqPadrao);
    free(textoOriginal);
    free(textoComprimido);
    free(ocoEncontrada);
    huffLiberarArvore(raizArvoreHuff);
    huffLiberarMapa(&mapaCodHuff, numCaracteresUnicos);

    return 0;
}