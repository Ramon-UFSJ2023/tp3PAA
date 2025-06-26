#include "bmh.h"

static void preProcessarBmh(const unsigned char *padrao, size_t tamanhoPadrao, int tabelaDesloc[]){
    for(int i=0; i<TAMANHO_ALFABETO; i++) tabelaDesloc[i] = tamanhoPadrao;
    for(size_t i=0; i<tamanhoPadrao-1;i++) tabelaDesloc[padrao[i]] = tamanhoPadrao-1-i;
}

int bmhBuscaNaoComprimido(const unsigned char *texto, size_t tamanhoTexto, const unsigned char *padrao, size_t tamanhoPadrao, long long *numeroComparacoes, double *tempoExecução, size_t *ocorrenciasEncontradas, size_t maximoOcorrencias){
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
            i += (tabDesloc[padrao[tamanhoPadrao-1]] > 1) ? tabDesloc[padrao[tamanhoPadrao-1]] : 1;

        }else{
            i+= tabDesloc[texto[i+tamanhoPadrao-1]];
        }
    }
    *tempoExecução = (double)(clock()-IniTempo)/CLOCKS_PER_SEC;
    return numOco;
}

int bmhLerBits(const unsigned char *buffer, size_t bitOffSet, size_t numBits, unsigned long long *bitsLidos){
    if(numBits > sizeof(unsigned long long)*CHAR_BIT || numBits ==0) return -1;
    *bitsLidos = 0;
    // size_t bytesLidos = 0; // Removido: variável não utilizada
    size_t bitsRestante = numBits;
    size_t byteInicial = bitOffSet/CHAR_BIT;
    int offsetByte = bitOffSet%CHAR_BIT;

    unsigned char primeiroByte = buffer[byteInicial];
    // Correção: Casts explicitos para size_t para resolver warnings de signed/unsigned
    size_t bitsPrimByte = ((size_t)CHAR_BIT - (size_t)offsetByte > numBits) ? numBits : ((size_t)CHAR_BIT - (size_t)offsetByte);
    unsigned char mascara = ((1 << bitsPrimByte) -1);
    mascara <<= ((size_t)CHAR_BIT -(size_t)offsetByte -bitsPrimByte); // Correção: Casts explicitos
    *bitsLidos = (primeiroByte & mascara) >> ((size_t)CHAR_BIT-(size_t)offsetByte-bitsPrimByte); // Correção: Casts explicitos
    bitsRestante -= bitsPrimByte;

    if(bitsRestante>0){
        byteInicial++;
        unsigned char proxByte = buffer[byteInicial];
        *bitsLidos = (*bitsLidos << bitsRestante) | (proxByte >> ((size_t)CHAR_BIT -bitsRestante)); // Correção: Casts explicitos
    }
    return 0;
}

int bmhBuscaComprimido(const unsigned char *textoComprimido, size_t tamanhoTextoComprimido, const unsigned char *padraoComprimido, size_t tamanhoPadraoCmp, long long *numCmp, double *tempExecucao, size_t *ocorrenciasEncontradas, size_t maximoOcorrencias, const NoHuff *raizHuff){
    clock_t iniTempo = clock();
    *numCmp = 0;
    int numOco = 0;
    size_t ocorrenInd=0;

    if(tamanhoPadraoCmp == 0 || tamanhoPadraoCmp >tamanhoTextoComprimido){
        *tempExecucao = (double)(clock()-iniTempo)/ CLOCKS_PER_SEC;
        return 0;
    }
    size_t i=0;
    while(i <= tamanhoTextoComprimido-tamanhoPadraoCmp){
        bool match= false;
        for(size_t j=0; j<tamanhoPadraoCmp;j++){
            (*numCmp)++;
            unsigned long long bitPadrao, bitTexto;
            if(bmhLerBits(padraoComprimido,j,1,&bitPadrao) != 0){
                fprintf(stderr,"Erro de leitura do bit do padrao comprimido.\n");
                match = true;
                break;
            }
            if((i+j) >tamanhoTextoComprimido){
                match = true;
                break;
            }
            if(bmhLerBits(textoComprimido,i+j,1,&bitTexto) !=0){
                fprintf(stderr,"Erro de leitura do bit do texto comprimido.\n");
                match = true;
                break;
            }
            if(bitPadrao != bitTexto){
                match =true;
                break;
            }
        }
        if(!match){
            size_t posiOriginalCaractere = 0;
            const NoHuff *tempNo = raizHuff;

            size_t bitAtualPos = 0;
            size_t caracDecodCont=0;
            if(raizHuff != NULL){
                tempNo = raizHuff;
                while(bitAtualPos<i){
                    unsigned long long bitAtualVal;
                    if(bmhLerBits(textoComprimido,bitAtualPos,1,&bitAtualVal)!=0) break;

                    if(bitAtualVal == 0){
                        tempNo = tempNo->esquerda;
                    }else{
                        tempNo = tempNo->direita;
                    }
                    bitAtualPos++;
                    if(tempNo->esquerda == NULL && tempNo->direita == NULL){
                        caracDecodCont++;
                        tempNo = raizHuff;
                    }
                }
                posiOriginalCaractere = caracDecodCont;
            }
            if(ocorrenInd <maximoOcorrencias) ocorrenciasEncontradas[ocorrenInd++] = posiOriginalCaractere;
            numOco++;
            i += tamanhoPadraoCmp;
        }else{
            i++;

        }
    }
    *tempExecucao = (double)(clock()- iniTempo)/CLOCKS_PER_SEC;
    return numOco;
}
