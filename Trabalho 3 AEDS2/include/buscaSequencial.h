// buscasequencial.h

#ifndef BUSCA_SEQUENCIAL_H
#define BUSCA_SEQUENCIAL_H

#include <stdio.h>  // Para FILE*
#include <stddef.h> // Para size_t
// Protótipo da função genérica de busca sequencial
void *buscaSequencialGenerica(int chave, FILE *in, FILE *log, int (*compara)(const void *elemento, int chave), size_t tamanho_struct);

// Protótipos das funções de comparação específicas para cada struct
int comparaTComp(const void *elemento, int chave);
int comparaTClie(const void *elemento, int chave);
int comparaTLoca(const void *elemento, int chave);

#endif // BUSCA_SEQUENCIAL_H