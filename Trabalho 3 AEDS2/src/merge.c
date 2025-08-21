#include <stdio.h>
#include "componentes.h"
#include <string.h>

// funções de ordenação EM DISCO para cada estrutura.
//  lê o máximo possível de dados que cabe em um array e ordena na memória ram.
//  depois, escreve um arquivo temporario. repete até todos os dados serem ordenados.
//  no final, junte tudo em um unico arquivo ordenado.

int divide_blocos_comp(char *arquivo, int tam_bloco)
{
    FILE *entrada = fopen(arquivo, "rb");
    if (!entrada)
        return 0;

    // principal lógica: criação de blocos (arrays) para ordenação
    TComp **bloco = malloc(sizeof(TComp *) * tam_bloco);
    int total_blocos = 0;

    while (1)
    {
        int lidos = 0;
        for (int i = 0; i < tam_bloco; i++)
        {
            TComp *comp = lecomp(entrada);
            if (!comp)
                break;
            bloco[lidos++] = comp;
        }
        if (lidos == 0)
            break;

        // ordena usando quicksort da biblioteca padrão da linguagem C
        qsort(bloco, lidos, sizeof(TComp *), compara_comp);

        // cria um arquivo temporario com esse bloco ordenado, salva e libera o array
        char nome_bloco[64];
        sprintf(nome_bloco, "temp_comp_%d.dat", total_blocos);
        FILE *saida = fopen(nome_bloco, "wb");
        for (int i = 0; i < lidos; i++)
        {
            salvacomp(bloco[i], saida);
            free(bloco[i]);
        }
        fclose(saida);
        total_blocos++;
    }

    free(bloco);
    fclose(entrada);
    return total_blocos;
}

// para clientes
int divide_blocos_clie(char *arquivo, int tam_bloco)
{
    FILE *entrada = fopen(arquivo, "rb");
    if (!entrada)
        return 0;

    TClie **bloco = malloc(sizeof(TClie *) * tam_bloco);
    int total_blocos = 0;

    while (1)
    {
        int lidos = 0;
        for (int i = 0; i < tam_bloco; i++)
        {
            TClie *clie = leclie(entrada);
            if (!clie)
                break;
            bloco[lidos++] = clie;
        }
        if (lidos == 0)
            break;

        qsort(bloco, lidos, sizeof(TClie *), compara_clie);

        char nome_bloco[64];
        sprintf(nome_bloco, "temp_clie_%d.dat", total_blocos);
        FILE *saida = fopen(nome_bloco, "wb");
        for (int i = 0; i < lidos; i++)
        {
            salvaclie(bloco[i], saida);
            free(bloco[i]);
        }
        fclose(saida);
        total_blocos++;
    }

    free(bloco);
    fclose(entrada);
    return total_blocos;
}

// para locações
int divide_blocos_loca(char *arquivo, int tam_bloco)
{
    FILE *entrada = fopen(arquivo, "rb");
    if (!entrada)
        return 0;

    TLoca **bloco = malloc(sizeof(TLoca *) * tam_bloco);
    int total_blocos = 0;

    while (1)
    {
        int lidos = 0;
        for (int i = 0; i < tam_bloco; i++)
        {
            TLoca *loca = leloca(entrada);
            if (!loca)
                break;
            bloco[lidos++] = loca;
        }
        if (lidos == 0)
            break;

        qsort(bloco, lidos, sizeof(TLoca *), compara_loca);

        char nome_bloco[64];
        sprintf(nome_bloco, "temp_loca_%d.dat", total_blocos);
        FILE *saida = fopen(nome_bloco, "wb");
        for (int i = 0; i < lidos; i++)
        {
            salvaloca(bloco[i], saida);
            free(bloco[i]);
        }
        fclose(saida);
        total_blocos++;
    }

    free(bloco);
    fclose(entrada);
    return total_blocos;
}

// com os blocos divididos, junta todos em um só
void intercala_comp(int num_blocos, int tam_bloco, char *nome_saida)
{
    FILE **arquivos = malloc(sizeof(FILE *) * num_blocos);//armazena os arquivos temporarios
    TComp **buffer = malloc(sizeof(TComp *) * num_blocos);//armazena o prox elemento de cada bloco (topo da pilha)

    // abre cada arquivo temporario e lê o primeiro item do arquivo
    for (int i = 0; i < num_blocos; i++)
    {
        char nome[64];
        sprintf(nome, "temp_comp_%d.dat", i);
        arquivos[i] = fopen(nome, "rb");
        buffer[i] = lecomp(arquivos[i]);
    }

    // cria arquivo final ordenado
    FILE *saida = fopen(nome_saida, "wb");

    while (1)
    {
        int menor = -1; // menor guarda o INDICE do menor item atual
        // percore todos os blocos, redefinindo qual o indice menor
        for (int i = 0; i < num_blocos; i++)
        {
            if (buffer[i])
            {
                if (menor == -1 || compara_comp(buffer[i], buffer[menor]) < 0)
                {
                    menor = i;
                }
            }
        }
        if (menor == -1)
            break;                       // se nenhum buffer tem dados, sai do loop
        salvacomp(buffer[menor], saida); // salva o menor item no arquivo final
        free(buffer[menor]);
        buffer[menor] = lecomp(arquivos[menor]); // le o proximo item do mesmo bloco de onde veio o menor
    }
    // apaga o arquivo temporario
    for (int i = 0; i < num_blocos; i++)
    {
        if (arquivos[i])
            fclose(arquivos[i]);
        char nome[64];
        sprintf(nome, "temp_comp_%d.dat", i);
        remove(nome);
    }

    fclose(saida);
    free(arquivos);
    free(buffer);
}

// para clientes
void intercala_clie(int num_blocos, int tam_bloco, char *nome_saida)
{
    FILE **arquivos = malloc(sizeof(FILE *) * num_blocos);
    TClie **buffer = malloc(sizeof(TClie *) * num_blocos);

    for (int i = 0; i < num_blocos; i++)
    {
        char nome[64];
        sprintf(nome, "temp_clie_%d.dat", i);
        arquivos[i] = fopen(nome, "rb");
        buffer[i] = leclie(arquivos[i]);
    }

    FILE *saida = fopen(nome_saida, "wb");

    while (1)
    {
        int menor = -1;
        for (int i = 0; i < num_blocos; i++)
        {
            if (buffer[i])
            {
                if (menor == -1 || compara_clie(buffer[i], buffer[menor]) < 0)
                {
                    menor = i;
                }
            }
        }
        if (menor == -1)
            break;
        salvaclie(buffer[menor], saida);
        free(buffer[menor]);
        buffer[menor] = leclie(arquivos[menor]);
    }

    for (int i = 0; i < num_blocos; i++)
    {
        if (arquivos[i])
            fclose(arquivos[i]);
        char nome[64];
        sprintf(nome, "temp_clie_%d.dat", i);
        remove(nome);
    }

    fclose(saida);
    free(arquivos);
    free(buffer);
}
// para locações
void intercala_loca(int num_blocos, int tam_bloco, char *nome_saida)
{
    FILE **arquivos = malloc(sizeof(FILE *) * num_blocos);
    TLoca **buffer = malloc(sizeof(TLoca *) * num_blocos);

    for (int i = 0; i < num_blocos; i++)
    {
        char nome[64];
        sprintf(nome, "temp_loca_%d.dat", i);
        arquivos[i] = fopen(nome, "rb");
        buffer[i] = leloca(arquivos[i]);
    }

    FILE *saida = fopen(nome_saida, "wb");

    while (1)
    {
        int menor = -1;
        for (int i = 0; i < num_blocos; i++)
        {
            if (buffer[i])
            {
                if (menor == -1 || compara_loca(buffer[i], buffer[menor]) < 0)
                {
                    menor = i;
                }
            }
        }
        if (menor == -1)
            break;
        salvaloca(buffer[menor], saida);
        free(buffer[menor]);
        buffer[menor] = leloca(arquivos[menor]);
    }

    for (int i = 0; i < num_blocos; i++)
    {
        if (arquivos[i])
            fclose(arquivos[i]);
        char nome[64];
        sprintf(nome, "temp_loca_%d.dat", i);
        remove(nome);
    }

    fclose(saida);
    free(arquivos);
    free(buffer);
}

void merge_externo_comp(char *arquivo, int tam_bloco)
{
    int num_blocos = divide_blocos_comp(arquivo, tam_bloco);
    intercala_comp(num_blocos, tam_bloco, "computadores.dat");
}

void merge_externo_clie(char *arquivo, int tam_bloco)
{
    int num_blocos = divide_blocos_clie(arquivo, tam_bloco);
    intercala_clie(num_blocos, tam_bloco, "clientes.dat");
}

void merge_externo_loca(char *arquivo, int tam_bloco)
{
    int num_blocos = divide_blocos_loca(arquivo, tam_bloco);
    intercala_loca(num_blocos, tam_bloco, "locacoes.dat");
}