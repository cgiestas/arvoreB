#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "particoes.h" 
#include <time.h>

void escreve_log_selecao(const char *mensagem){
    FILE *log_particao = fopen("../logs/log_particao.txt", "a");
    if(log_particao){
        time_t agora = time(NULL);
        struct tm *tm_info = localtime(&agora);
        char data_hora[26];
        strftime(data_hora, 26, "%Y-%m-%d %H:%M:%S", tm_info);
        fprintf(log_particao, "[%s] %s\n", data_hora, mensagem);
        fclose(log_particao);
    }
}

// Função que verifica se todos os elementos da memória estão congelados
static bool todos_congelados(ElementoMemoria memoria[], int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        if (memoria[i].valido && !memoria[i].congelado)
            return false;
    }
    return true;
}

// Função para encontrar índice do menor elemento não congelado
static int encontrar_menor_nao_congelado(ElementoMemoria memoria[], int tamanho) {
    int menor_idx = -1;
    for (int i = 0; i < tamanho; i++) {
        if (memoria[i].valido && !memoria[i].congelado) {
            if (menor_idx == -1 || memoria[i].comp.cod < memoria[menor_idx].comp.cod) {
                menor_idx = i;
            }
        }
    }
    return menor_idx;
}

void imprimir_particao(const char *nome_arquivo) {
    FILE *f = fopen(nome_arquivo, "rb");
    if (!f) {
        escreve_log_selecao("Erro ao abrir particao para impressao.");
        perror("Erro ao abrir particao para impressao.");
        return;
    }
    printf("Conteudo da particao '%s':\n", nome_arquivo);
    TComp comp;
    int count = 0;
    while (fread(&comp, sizeof(TComp), 1, f) == 1) {
        printf("  Computador %d: cod=%d, marca=%s, modelo=%s, processador=%s, valor=%.2f, disponivel=%d\n",
            ++count, comp.cod, comp.marca, comp.modelo, comp.processador, comp.valor, comp.disponivel);
    }
    if(count == 0)
        printf("  (Vazia)\n");
    printf("\n");
    fclose(f);
}

void gerar_particoes_computador(const char *nome_arquivo_entrada, int tamanho_maximo_memoria, int *cont_part) {
    clock_t inicio = clock();
    escreve_log_selecao("Iniciando geracao de particoes por selecao com substituicao.");

    printf("Iniciando geracao das particoes ordenadas...\n");

    FILE *entrada = fopen(nome_arquivo_entrada, "rb");
    if (!entrada) {
        escreve_log_selecao("Erro ao abrir arquivo de entrada.");
        perror("Erro ao abrir arquivo de entrada.");
        return;
    }

    ElementoMemoria *memoria = malloc(sizeof(ElementoMemoria) * tamanho_maximo_memoria);
    if (!memoria) {
        escreve_log_selecao("Erro ao alocar memoria.");
        perror("Erro ao alocar memoria.");
        fclose(entrada);
        return;
    }

    for (int i = 0; i < tamanho_maximo_memoria; i++) {
        memoria[i].valido = false;
        memoria[i].congelado = false;
    }

    // Preenche a memória inicialmente
    int i;
    for (i = 0; i < tamanho_maximo_memoria; i++) {
        TComp *comp = lecomp(entrada);
        if (!comp) break;

        memoria[i].comp = *comp;
        memoria[i].valido = true;
        memoria[i].congelado = false;
        free(comp);
    }

    int particao_atual = 0;
    (*cont_part) = 0;

    TComp ultimo_gravado = {0};

    while (true) {
        bool dados_validos = false;
        for (int j = 0; j < tamanho_maximo_memoria; j++) {
            if (memoria[j].valido) {
                dados_validos = true;
                break;
            }
        }
        if (!dados_validos) break;

        // Cria nova partição
        char nome_arquivo_saida[256];
        sprintf(nome_arquivo_saida, "particao_%d.bin", particao_atual++);
        FILE *saida = fopen(nome_arquivo_saida, "wb");
        if (!saida) {
            escreve_log_selecao("Erro ao criar arquivo de saida.");
            perror("Erro ao criar arquivo de saida.");
            break;
        }

        printf("Criando particao: %s\n", nome_arquivo_saida);

        // Resetar congelamento
        for (int j = 0; j < tamanho_maximo_memoria; j++) {
            if (memoria[j].valido)
                memoria[j].congelado = false;
        }

        bool escreveu_na_particao = false;

        while (!todos_congelados(memoria, tamanho_maximo_memoria)) {
            int menor_idx = encontrar_menor_nao_congelado(memoria, tamanho_maximo_memoria);
            if (menor_idx == -1) break;

            fwrite(&memoria[menor_idx].comp, sizeof(TComp), 1, saida);
            ultimo_gravado = memoria[menor_idx].comp;
            escreveu_na_particao = true;

            TComp *novo = lecomp(entrada);
            if (novo) {
                if (novo->cod < ultimo_gravado.cod) {
                    memoria[menor_idx].congelado = true;
                }
                memoria[menor_idx].comp = *novo;
                memoria[menor_idx].valido = true;
                free(novo);
            } else {
                memoria[menor_idx].valido = false;
            }
        }

        fflush(saida);
        fclose(saida);

        if (escreveu_na_particao) {
            (*cont_part)++;
            imprimir_particao(nome_arquivo_saida);
        } else {
            // Remove arquivo vazio
            remove(nome_arquivo_saida);
        }
    }

    free(memoria);
    fclose(entrada);

    printf("Geracao de particoes finalizada. Total de particoes criadas: %d\n", *cont_part);

    clock_t fim = clock();
    double tempo_exec = (double)(fim - inicio) / CLOCKS_PER_SEC;
    char msg[128];
    sprintf(msg, "%d particoes geradas com sucesso em %.2f segundos.", *cont_part, tempo_exec);
    escreve_log_selecao(msg);
}