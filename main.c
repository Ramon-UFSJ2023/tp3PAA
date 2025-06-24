#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[]){
    if(argc != 3){fprintf(stderr, "Uso: %s entrada.txt padrao.txt.\n", argv[0]);}

    char *textoEntrada = argv[1];
    char *padrao = argv[2];

    FILE *arqTexto = fopen(textoEntrada,"r");
    if(arqTexto == NULL){
        perror("Erro em abertura de do texto");
        return 1;
    }

    fseek(arqTexto, 0, SEEK_END);
    long tamanhoArqTexto = ftell(arqTexto);
    fseek(arqTexto,0, SEEK_SET);

    char *alocArqTexto =(char*)malloc(tamanhoArqTexto+1);
    if(alocArqTexto == NULL){
        perror("Erro na alocação.");
        fclose(arqTexto);
        return 1;
    }
    fread(alocArqTexto, 1, tamanhoArqTexto, arqTexto);
    alocArqTexto[tamanhoArqTexto] = '\0';
    fclose(arqTexto);

    return 0;
}