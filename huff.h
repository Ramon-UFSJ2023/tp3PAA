#ifndef HUFF_H
#define HUFF_H
#include <stddef.h>

typedef struct NoHuff{
   unsigned char caractere;
   int frequencia;
   struct NoHuff *esquerda;
   struct NoHuff *direita;
}NoHuff;

typedef struct mapaCodigo{
    unsigned char caractere;
    char *codigo;
    int tamanhoCodigo;
} mapaCodigo;

void costruirHuffGeraCodigo(const unsigned char *arqTexto, size_t tamanhoArqTexto, NoHuff **raizArvore, mapaCodigo ** mapaCod, int *numCaracteresUnicos);
int comprimirTexto(const unsigned char *arqTexto, size_t tamanhoArqTexto, const mapaCodigo *mapaCod, int numeroCaracteresUnicos, unsigned char **dadosComprimidos, size_t *tamDadosCompriBytes, size_t *totalBitsCompri);
int huffmanComprimirPadrao(const unsigned char *arqPadrao,size_t tamanhoPadrao,const mapaCodigo *mapaCod,int numCaracteresUnicos,unsigned char **padraoComprimido,size_t *tamanhoPadraoCompriBits);
void huffLiberarArvore(NoHuff *raiz);
void huffLiberarMapa(mapaCodigo **mapaCod, int numeroCaracteresUni);

#endif