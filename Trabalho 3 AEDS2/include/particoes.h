#ifndef PARTICOES_H
#define PARTICOES_H

#include <stdbool.h>
#include <stdio.h>
#include "componentes.h"
#include <time.h>

typedef struct
{
    TComp comp;
    bool valido;
    bool congelado;
} ElementoMemoria;

TComp *lecomp(FILE *in);
void escreve_log_selecao(const char *mensagem);
void gerar_particoes_computador(const char *nome_arquivo_entrada, int tamanho_maximo_memoria, int *cont_part);
void imprimir_particao(const char *nome_arquivo);
#endif // PARTICOES_H