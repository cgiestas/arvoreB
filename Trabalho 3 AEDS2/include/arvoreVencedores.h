#ifndef ARVORE_VENCEDORES
#define ARVORE_VENCEDORES
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "particoes.h"
#include "componentes.h"
#include <time.h>

typedef struct
{
    TComp *comp;
    int origem;
} HeapItem;

void trocar(HeapItem *a, HeapItem *b);
void min_heapify(HeapItem heap[], int n, int i);
void construir_heap(HeapItem heap[], int n);
void vencedores_comp(int num_particoes, const char *nome_saida);
void escreve_log_vencedores(const char *mensagem);
void imprimeVencedores(const char *nomeArquivo);
#endif