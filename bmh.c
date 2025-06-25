#include "bmh.h"

static void preProcessarBmh(const unsigned char *padrao, size_t tamanhoPadrao, int tabelaDesloc[]){
    for(int i=0; i<TAMANHO_ALFABETO; i++) tabelaDesloc[i] = tamanhoPadrao;
    for(size_t i=0; i<tamanhoPadrao-1;i++) tabelaDesloc[padrao[i]] = tamanhoPadrao-1-i;
}

int bmhBuscaNaoComprimido(const unsigned char *texto, size_t tamanhoTexto, const unsigned *padrao, size_t tamanhoPadrao, long long *numeroComparacoes, double *tempoExecução, size_t *ocorrenciasEncontradas, size_t maximoOcorrencias){
    clock_t IniTempo = clock();
    *numeroComparacoes = 0;
    int numOco = 0;
    size_t ocoInd=0;

    if(tamanhoPadrao == 0 || tamanhoPadrao > tamanhoTexto){
        *tempoExecução = (double)(clock() - IniTempo)/CLOCKS_PER_SEC;
        return 0;
    }

    int tabDesloc[TAMANHO_ALFABETO];
    preProcessarBmh(padrao, tamanhoPadrao, tabDesloc);

    size_t i=0;
    while(i <= tamanhoTexto-tamanhoPadrao){
        int j=tamanhoPadrao-1;
        while(j>=0){
            (*numeroComparacoes)++;
            if(texto[i+j] != padrao[j]) break;
            j--;
        }
        if(j<0){
            if(ocoInd <maximoOcorrencias)ocorrenciasEncontradas[ocoInd++] = i;
            numOco++;
            i += (tabDesloc[padrao[tamanhoPadrao-1]] > 1) ? tabDesloc[padrao[tamanhoPadrao]] : 1;

        }else{
            i+= tabDesloc[texto[i+tamanhoPadrao-1]];
        }
        *tempoExecução = (double)(clock()-IniTempo)/CLOCKS_PER_SEC;
        return numOco;
    }
}