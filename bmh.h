#ifndef BMH_H
#define BMH_H
#include "huff.h"
#include <time.h>
#include <stdbool.h>

#define TAMANHO_ALFABETO 256

int bmhBuscaNaoComprimido(const unsigned char *texto, size_t tamanhoTexto, const unsigned *padrao, size_t tamanhoPadrao, long long *numeroComparacoes, double *tempoExecução, size_t *ocorrenciasEncontradas, size_t maximoOcorrencias);
int bmhBuscaComprimido(const unsigned char *textoComprimido, size_t tamanhoTextoComprimido, const unsigned *padraoComprimido, size_t tamanhoPadraoCmp, long long *numCmp, double *tempExecucao, size_t *ocorrenciasEncontradas, size_t maximoOcorrencias, const NoHuff *raizHuff);
int bmhLerBits(const unsigned char *buffer, size_t bitOffSet, size_t numBits, unsigned long long *bitsLidos);


#endif