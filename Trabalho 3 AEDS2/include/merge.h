#ifndef MERGE_SORT
#define MERGE_SORT

#include <stdio.h>
#include <string.h>
#include "componentes.h"

int divide_blocos_comp(char *arquivo, int tam_bloco);
int divide_blocos_clie(char *arquivo, int tam_bloco);
int divide_blocos_loca(char *arquivo, int tam_bloco);

void intercala_comp(int num_blocos, int tam_bloco, char *nome_saida);
void intercala_clie(int num_blocos, int tam_bloco, char *nome_saida);
void intercala_loca(int num_blocos, int tam_bloco, char *nome_saida);

void merge_externo_comp(char *arquivo, int tam_bloco);
void merge_externo_clie(char *arquivo, int tam_bloco);
void merge_externo_loca(char *arquivo, int tam_bloco);

#endif