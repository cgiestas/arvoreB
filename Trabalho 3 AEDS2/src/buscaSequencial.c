#include "buscaSequencial.h"
#include <stdlib.h> 
#include <string.h> 
#include "componentes.h"

void *buscaSequencialGenerica(int chave, FILE *in, FILE *log, int (*compara)(const void *elemento, int chave), size_t tamanho_struct) {
    if (in == NULL) {
        fprintf(log, "Erro: Arquivo de entrada nulo na busca sequencial.\n");
        return NULL;
    }

    void *elemento = malloc(tamanho_struct);
    if (elemento == NULL) {
        fprintf(log, "Erro de alocacao de memoria na busca sequencial.\n");
        return NULL;
    }

    fseek(in, 0, SEEK_SET);

    while (fread(elemento, tamanho_struct, 1, in) == 1) {
        if (compara(elemento, chave) == 0) {
            fprintf(log, "Elemento com chave %d encontrado.\n", chave);
            return elemento;
        }
    }

    fprintf(log, "Elemento com chave %d nao encontrado.\n", chave);

    return NULL;
}

// Implementações das funções de comparação 
int comparaTComp(const void *elemento, int chave) {
    const TComp *comp = (const TComp *)elemento;
    if (comp->cod < chave) {
        return -1;
    } 
    else if (comp->cod > chave) {
        return 1;  
    } 
    else {
        return 0;  
    }
}

int comparaTClie(const void *elemento, int chave) {
    const TClie *clie = (const TClie *)elemento;
    if (clie->cod < chave) {
        return -1;
    } 
    else if (clie->cod > chave) {
        return 1;
    } 
    else {
        return 0;
    }
}

int comparaTLoca(const void *elemento, int chave) {
    const TLoca *loca = (const TLoca *)elemento;
    if (loca->cod < chave) {
        return -1;
    } 
    else if (loca->cod > chave) {
        return 1;
    } 
    else {
        return 0;
    }
}