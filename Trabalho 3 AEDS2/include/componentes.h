#ifndef COMPONENTES_H
#define COMPONENTES_H

#include <stdlib.h>
#include <stdio.h>

#pragma pack(push, 1)

typedef struct Computador
{
    int cod;
    char marca[50];
    char modelo[50];
    char processador[3];
    float valor;
    int disponivel;
} TComp;

typedef struct Cliente
{
    int cod;
    char nome[50];
    char cpf_cnpj[15];
    int telefone;
    char email[50];
} TClie;

typedef struct Locacao
{
    int cod;
    int cod_comp;
    int cod_clie;
    int quantidade;
    char data_inicial[11];
    char data_final[11];
    float valor_total;
    int ativa;
} TLoca;

#pragma pack(pop)

// Retorna tamanho do funcionario em bytes
int tamanho_registrocomp();
int tamanho_registrocli();
int tamanho_registroloc();

// Cria computador.
TComp *computador(int cod, char *marca, char *modelo, char *processador, float valor, int disponivel);

// Cria cliente.
TClie *cliente(int cod, char *nome, char *cpf_cnpj, int telefone, char *email);

// Cria Locacao.
TLoca *locacao(int cod, int cod_comp, int cod_clie, int quantidade, char *data_inicial, char *data_final, float valor_total, int ativa);

// Salva funcionario no arquivo out, na posicao atual do cursor
void salvacomp(TComp *comp, FILE *out);

// Salva funcionario no arquivo out, na posicao atual do cursor
void salvaclie(TClie *clie, FILE *out);

// Salva funcionario no arquivo out, na posicao atual do cursor
void salvaloca(TLoca *loca, FILE *out);

// retorna a quantidade de registros no arquivo
// int tamanho_arquivo(FILE *arq);

// Le um funcionario do arquivo in na posicao atual do cursor
// Retorna um ponteiro para funcionario lido do arquivo
TComp *lecomp(FILE *in);

// Le um funcionario do arquivo in na posicao atual do cursor
// Retorna um ponteiro para funcionario lido do arquivo
TClie *leclie(FILE *in);

// Le um funcionario do arquivo in na posicao atual do cursor
// Retorna um ponteiro para funcionario lido do arquivo
TLoca *leloca(FILE *in);

// Imprime funcionario
void imprimecomp(TComp *comp);

// Imprime funcionario
void imprimeclie(TClie *clie);

// Imprime funcionario
void imprimeloca(TLoca *loca);

// Cria a base de dados
void criarBase(FILE *out, int tam, int escolha);

// embaralha base de dados
void shuffle(int *vet, int MAX, int MIN);

void embaralha(int *vet, int tam);

// Imprime a base de dados
void imprimirBase(FILE *out, int escolha);

// Compara dois funcionarios
// Retorna 1 se os valores dos atributos de ambos forem iguais
// e 0 caso contrario
int compara_comp(const void *a, const void *b);
int compara_clie(const void *a, const void *b);
int compara_loca(const void *a, const void *b);

#endif