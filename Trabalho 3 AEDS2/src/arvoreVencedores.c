#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "particoes.h"
#include "arvoreVencedores.h"
#include "componentes.h"
#include <time.h>

void escreve_log_vencedores(const char *mensagem){
    FILE *log_vencedores = fopen("../logs/log_vencedores.txt", "a");
    if(log_vencedores){
        time_t agora = time(NULL);
        struct tm *tm_info = localtime(&agora);
        char data_hora[26];
        strftime(data_hora, 26, "%Y-%m-%d %H:%M:%S", tm_info);
        fprintf(log_vencedores, "[%s] %s\n", data_hora, mensagem);
        fclose(log_vencedores);
    }
}

void trocar(HeapItem *a, HeapItem *b) {
    HeapItem temp = *a;
    *a = *b;
    *b = temp;
}

void min_heapify(HeapItem heap[], int n, int i) {
    int menor = i;
    int esq = 2 * i + 1;
    int dir = 2 * i + 2;

    if (esq < n && heap[esq].comp->cod < heap[menor].comp->cod)
        menor = esq;
    if (dir < n && heap[dir].comp->cod < heap[menor].comp->cod)
        menor = dir;

    if (menor != i) {
        trocar(&heap[i], &heap[menor]);
        min_heapify(heap, n, menor);
    }
}

void construir_heap(HeapItem heap[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--) {
        min_heapify(heap, n, i);
    }
}

void vencedores_comp(int num_particoes, const char *nome_saida) {
    clock_t inicio = clock();
    escreve_log_vencedores("Iniciando arvore de vencedores.");

    FILE *entradas[num_particoes];
    FILE *saida = fopen(nome_saida, "wb");
    if (!saida) {
        escreve_log_vencedores("Erro ao criar arquivo de saida.");
        perror("Erro ao criar arquivo de saida.");
        return;
    }

    HeapItem heap[num_particoes];
    int tamanho_heap = 0;

    for (int i = 0; i < num_particoes; i++) {
        char nome_arquivo[64];
        sprintf(nome_arquivo, "particao_%d.bin", i);
        entradas[i] = fopen(nome_arquivo, "rb");

        if (!entradas[i]) {
            char msg[100];
            sprintf(msg, "Erro ao abrir particao_%d.bin", i);
            escreve_log_vencedores(msg);
            perror("Erro ao abrir particao.");
            continue;
        }

        TComp *comp = lecomp(entradas[i]);
        if (comp != NULL) {
            heap[tamanho_heap].comp = comp;
            heap[tamanho_heap].origem = i;
            tamanho_heap++;
        }
    }

    construir_heap(heap, tamanho_heap);

    while (tamanho_heap > 0) {
        HeapItem menor = heap[0];
        salvacomp(menor.comp, saida);
        free(menor.comp);

        int origem = menor.origem;
        TComp *novo = lecomp(entradas[origem]);

        if (novo != NULL) {
            heap[0].comp = novo;
            heap[0].origem = origem;
        } else {
            heap[0] = heap[tamanho_heap - 1];
            tamanho_heap--;
        }

        min_heapify(heap, tamanho_heap, 0);
    }

    for (int i = 0; i < num_particoes; i++) {
        if (entradas[i])
            fclose(entradas[i]);
    }
    escreve_log_vencedores("Todos os arquivos de particoes foram fechados.");
    fclose(saida);
    clock_t fim = clock();
    double tempo_exec = (double)(fim - inicio) / CLOCKS_PER_SEC;

    char msg[128];
    sprintf(msg, "Intercalacao concluida com sucesso em %.2f segundos.", tempo_exec);
    escreve_log_vencedores(msg);
}

void imprimeVencedores(const char* nomeArquivo) {

    FILE* arquivo = fopen(nomeArquivo, "rb");
    if (!arquivo) {
        escreve_log_vencedores("Erro ao abrir o arquivo de registros vencedores.");
        perror("Erro ao abrir o arquivo de registros vencedores.");
        return;
    }

    TComp comp;
    int i = 0;
    while (fread(&comp, sizeof(TComp), 1, arquivo) == 1) {
        printf("\nRegistro %d:\n", ++i);
        imprimecomp(&comp);
    }
    fclose(arquivo);
}
