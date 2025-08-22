#include "buscaBinaria.h"
#include "buscaSequencial.h"
#include "componentes.h"
#include "interacoes.h"
#include "merge.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "particoes.h"
#include "arvoreVencedores.h"

int main()
{
    FILE *computadores, *clientes, *locacoes, *log;

    if ((log = fopen("../logs/log.txt", "a+")) == NULL)
    {
        printf("Erro ao abrir arquivo log\n");
        exit(1);
    }

    if ((computadores = fopen("computadores.dat", "w+b")) == NULL ||
        (locacoes = fopen("locacoes.dat", "w+b")) == NULL ||
        (clientes = fopen("clientes.dat", "w+b")) == NULL)
    {
        printf("Erro ao abrir arquivo\n");
        exit(1);
    }
    else
    {
        criarBase(computadores, 5000, 1);
        rewind(computadores);
        imprimirBase(computadores, 1);

        criarBase(clientes, 5000, 2);
        rewind(clientes);
        imprimirBase(clientes, 2);

        // criarBase(locacoes, 1000, 3);
        // rewind(locacoes);
        // imprimirBase(locacoes, 3);
    }

    int opcao;
    int dados_ordenados = 0;

    TComp *comp = NULL;
    TClie *clie = NULL;
    TLoca *loca = NULL;
    int tam_bloco = 100;                               // para fazer o merge externo
    const char *arquivo_de_dados = "computadores.dat"; // para gerar particoes
    int cont_part = 0;
    do
    {
        exibirMenu();
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            realizarLocacaoUI(computadores, clientes, locacoes, log);
            dados_ordenados = 0;

            if (comp)
                free(comp);
            if (clie)
                free(clie);
            if (loca)
                free(loca);
            comp = NULL;
            clie = NULL;
            loca = NULL;
            break;

        case 2:
            finalizarLocacaoUI(computadores, locacoes, log);
            dados_ordenados = 0;

            if (comp)
                free(comp);
            if (clie)
                free(clie);
            if (loca)
                free(loca);
            comp = NULL;
            clie = NULL;
            loca = NULL;
            break;

        case 3:
            visualizarLocacoesClienteUI(locacoes, log);
            break;

        case 4:
            imprimirBase(computadores, 1);
            break;

        case 5:
            menuBuscaSequencial(computadores, clientes, locacoes, log);
            break;

        case 6:
        {
            if (dados_ordenados)
            {
                FILE *f_comp = fopen("computadores.dat", "rb");
                FILE *f_clie = fopen("clientes.dat", "rb");
                FILE *f_loca = fopen("locacoes.dat", "rb");
                FILE *log = fopen("../logs/log.txt", "a");

                menuBuscaBinaria(f_comp, f_clie, f_loca, log);

                fclose(f_comp);
                fclose(f_clie);
                fclose(f_loca);
                fclose(log);
            }
            else
            {
                printf("\nERRO: Os dados precisam ser ordenados primeiro!\n");
                printf("Por favor, execute a Opcao 7 antes de tentar uma busca binaria.\n");
            }
            break;
        }
        case 7:
            // tamanho máximo de indices de cada bloco na ram
            merge_externo_comp("computadores.dat", tam_bloco);
            merge_externo_clie("clientes.dat", tam_bloco);
            merge_externo_loca("locacoes.dat", tam_bloco);
            // reabre os arquivos para leitura após ordenação
            FILE *computadores = fopen("computadores.dat", "rb");
            FILE *clientes = fopen("clientes.dat", "rb");
            FILE *locacoes = fopen("locacoes.dat", "rb");

            imprimirBase(computadores, 1);
            imprimirBase(clientes, 2);
            imprimirBase(locacoes, 3);

            fclose(computadores);
            fclose(clientes);
            fclose(locacoes);
            dados_ordenados = 1; // marca que os dados agora estão ordenados
            break;
        case 8:
            int tamanho_memoria = 5;
            gerar_particoes_computador(arquivo_de_dados, tamanho_memoria, &cont_part);
            printf("Total de particoes: %d\n", cont_part);
            break;
        case 9:
            vencedores_comp(cont_part, "computadores.dat");
            imprimeVencedores("computadores.dat");
            break;
        case 10:
            TArvoreB arvore;
            criarArvore(&arvore, "dados.dat");

            inserirB(&arvore, 10);
            inserirB(&arvore, 20);
            inserirB(&arvore, 5);
            inserirB(&arvore, 6);
            inserirB(&arvore, 12);
            inserirB(&arvore, 30);

            printf("Árvore após inserções:\n");
            imprimirArvore(&arvore);

            // ===== Testando exclusão =====
            excluirB(&arvore, 6);
            printf("\nÁrvore após excluir 6:\n");
            imprimirArvore(&arvore);

            excluirB(&arvore, 12);
            printf("\nÁrvore após excluir 12:\n");
            imprimirArvore(&arvore);

            excluirB(&arvore, 30);
            printf("\nÁrvore após excluir 30:\n");
            imprimirArvore(&arvore);

            fecharArvore(&arvore);
        case 0:
            printf("Saindo do programa...\n");
            break;
        default:
            printf("Opcao invalida! Tente novamente.\n");
            break;
        }

    } while (opcao != 0);

    if (comp)
        free(comp);
    if (clie)
        free(clie);
    if (loca)
        free(loca);

    fclose(computadores);
    fclose(clientes);
    fclose(locacoes);
    fclose(log);

    return 0;

}
