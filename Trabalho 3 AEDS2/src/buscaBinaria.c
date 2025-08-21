#include "buscaBinaria.h"
#include "buscaSequencial.h"
#include "componentes.h"
#include <stdlib.h>
#include <string.h> 

int compara_bin_comp(const void *elem, int chave) {
    const TComp *comp = (const TComp *)elem;
    if (comp->cod < chave) return -1;
    if (comp->cod > chave) return 1;
    return 0;
}

int compara_bin_clie(const void *elem, int chave) {
    const TClie *clie = (const TClie *)elem;
    if (clie->cod < chave) return -1;
    if (clie->cod > chave) return 1;
    return 0;
}

int compara_bin_loca(const void *elem, int chave) {
    const TLoca *loca = (const TLoca *)elem;
    if (loca->cod < chave) return -1;
    if (loca->cod > chave) return 1;
    return 0;
}


void *buscaBinariaGenerica(FILE *arquivo, int chave, size_t tamanho_struct, int (*compara)(const void *, int), FILE *log) {
    fseek(arquivo, 0, SEEK_END);
    long tamanho_arquivo = ftell(arquivo);
    long num_registros = tamanho_arquivo / tamanho_struct;
    long esquerda = 0, direita = num_registros - 1;

    void *registro = malloc(tamanho_struct);
    if (!registro) {
        fprintf(log, "Erro ao alocar memoria na busca binaria!\n");
        return NULL;
    }

    while (esquerda <= direita) {
        long meio = esquerda + (direita - esquerda) / 2;
        fseek(arquivo, meio * tamanho_struct, SEEK_SET);
        fread(registro, tamanho_struct, 1, arquivo);

        int cmp = compara(registro, chave);
        if (cmp == 0) {
            fprintf(log, "Elemento com chave %d encontrado na posicao %ld.\n", chave, meio);
            return registro;  // achou, retorna o registro alocado
        } else if (cmp < 0) {
            esquerda = meio + 1;
        } else {
            direita = meio - 1;
        }
    }

    free(registro);
    fprintf(log, "Elemento com chave %d nao encontrado.\n", chave);
    return NULL;
}

TComp *buscaComputadorBinario(FILE *arquivo, int chave, FILE *log) {
    return (TComp *) buscaBinariaGenerica(arquivo, chave, sizeof(TComp), compara_bin_comp, log);
}

TClie *buscaClienteBinario(FILE *arquivo, int chave, FILE *log) {
    return (TClie *) buscaBinariaGenerica(arquivo, chave, sizeof(TClie), compara_bin_clie, log);
}

TLoca *buscaLocacaoBinario(FILE *arquivo, int chave, FILE *log) {
    return (TLoca *) buscaBinariaGenerica(arquivo, chave, sizeof(TLoca), compara_bin_loca, log);
}
