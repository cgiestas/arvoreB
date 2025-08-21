// buscabinaria.h

#ifndef BUSCA_BINARIA_H
#define BUSCA_BINARIA_H

#include <stdio.h>
#include <stddef.h>
#include "componentes.h"
#include "buscaSequencial.h"

void *buscaBinariaGenerica(FILE *arquivo, int chave, size_t tamanho_struct, int (*compara)(const void *, int), FILE *log);

TComp *buscaComputadorBinario(FILE *arquivo, int chave, FILE *log);
TClie *buscaClienteBinario(FILE *arquivo, int chave, FILE *log);
TLoca *buscaLocacaoBinario(FILE *arquivo, int chave, FILE *log);

#endif // BUSCA_BINARIA_H