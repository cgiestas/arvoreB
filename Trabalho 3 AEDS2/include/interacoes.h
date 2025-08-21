#ifndef INTERACOES_H
#define INTERACOES_H

#include <stdio.h>
#include "componentes.h"

void alugarComputador(FILE *f_comp, FILE *f_clie, FILE *f_loca, FILE *f_log, int cod_clie, int cod_comp, int quantidade, const char *data_inicial, const char *data_final);

void exibirMenu();
void realizarLocacaoUI(FILE *f_comp, FILE *f_clie, FILE *f_loca, FILE *f_log);
void finalizarLocacaoUI(FILE *f_comp, FILE *f_loca, FILE *f_log);
void visualizarLocacoesClienteUI(FILE *f_loca, FILE *f_log);
void menuBuscaSequencial(FILE *f_comp, FILE *f_clie, FILE *f_loca, FILE *f_log);
void menuBuscaBinaria(FILE *f_comp, FILE *f_clie, FILE *f_loca, FILE *log);

#endif