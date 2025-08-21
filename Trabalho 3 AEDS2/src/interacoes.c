#include "interacoes.h"
#include "componentes.h"
#include "buscaSequencial.h"
#include "buscaBinaria.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
// Locar computador, finalizar locação, lista de computadores e locações feitas por cliente

void alugarComputador(FILE *f_comp, FILE *f_clie, FILE *f_loca, FILE *f_log, int cod_clie, int cod_comp, int quantidade, const char *data_inicial, const char *data_final)
{

    TClie *cliente = (TClie *)buscaSequencialGenerica(cod_clie, f_clie, f_log, comparaTClie, sizeof(TClie));
    if (cliente == NULL)
    {
        printf("Erro: Cliente com o codigo %d nao encontrado.\n", cod_clie);
        fprintf(f_log, "Tentativa de aluguel falhou: Cliente com codigo %d nao existe.\n", cod_clie);
        return;
    }
    free(cliente);

    TComp *computador = (TComp *)buscaSequencialGenerica(cod_comp, f_comp, f_log, comparaTComp, sizeof(TComp));
    if (computador == NULL)
    {
        printf("Erro: Computador com o codigo %d nao encontrado.\n", cod_comp);
        fprintf(f_log, "Tentativa de aluguel falhou: Computador com codigo %d nao existe.\n", cod_comp);
        return;
    }

    if (computador->disponivel == 0)
    {
        printf("Erro: O computador %s (cod: %d) nao esta disponivel para locacao.\n", computador->modelo, cod_comp);
        fprintf(f_log, "Tentativa de aluguel falhou: Computador %d indisponivel.\n", cod_comp);
        free(computador);
        return;
    }

    int proximo_cod_loca = 1;
    fseek(f_loca, 0, SEEK_END);
    long tamanho_arquivo = ftell(f_loca);

    if (tamanho_arquivo >= sizeof(TLoca))
    {
        fseek(f_loca, -(long int)sizeof(TLoca), SEEK_END);
        TLoca *ultima_locacao = leloca(f_loca);
        if (ultima_locacao != NULL)
        {
            proximo_cod_loca = ultima_locacao->cod + 1;
            free(ultima_locacao);
        }
    }

    float valor_total = computador->valor * quantidade;
    TLoca *nova_locacao = locacao(proximo_cod_loca, cod_comp, cod_clie, quantidade, (char *)data_inicial, (char *)data_final, valor_total, 1);

    salvaloca(nova_locacao, f_loca);
    printf("\nLocacao do computador %s para o cliente de codigo %d realizada com sucesso!\n", computador->modelo, cod_clie);
    fprintf(f_log, "Nova locacao (cod: %d) criada para o cliente %d e computador %d.\n", nova_locacao->cod, cod_clie, cod_comp);

    computador->disponivel = 0;

    rewind(f_comp);
    TComp *comp_temp;
    long pos = 0;
    while ((comp_temp = lecomp(f_comp)) != NULL)
    {
        if (comp_temp->cod == cod_comp)
        {
            fseek(f_comp, pos * sizeof(TComp), SEEK_SET);
            salvacomp(computador, f_comp);
            fprintf(f_log, "Status do computador %d atualizado para indisponivel.\n", cod_comp);
            free(comp_temp);
            break;
        }
        pos++;
        free(comp_temp);
    }

    free(computador);
    free(nova_locacao);
}

void exibirMenu()
{
    printf("\n\n====== MENU PRINCIPAL ======\n");
    printf("1. Realizar uma Locacao\n");
    printf("2. Finalizar uma Locacao\n");
    printf("3. Visualizar Locacoes de um Cliente\n");
    printf("4. Visualizar Lista de Computadores Atualizada\n");
    printf("5. Executar Busca Sequencial\n");
    printf("6. Executar Busca Binaria (Apos Ordenar)\n");
    printf("7. Imprimir Dados Ordenados\n");
    printf("8. Gerar Particoes(Selecao com Substituicao)\n");
    printf("9. Intercalar por Arvore de Vencedores\n");
    printf("0. Sair\n");
    printf("==========================\n");
    printf("Escolha uma opcao: ");
}

// Interface para pegar dados do usuário e chamar a função de aluguel
void realizarLocacaoUI(FILE *f_comp, FILE *f_clie, FILE *f_loca, FILE *f_log)
{
    int cod_clie, cod_comp, quantidade;
    char data_inicio[11], data_final[11];

    printf("\n--- Realizar Locacao ---\n");
    printf("Digite o codigo do Cliente: ");
    scanf("%d", &cod_clie);
    printf("Digite o codigo do Computador: ");
    scanf("%d", &cod_comp);
    printf("Digite a quantidade: ");
    scanf("%d", &quantidade);
    printf("Digite a data de inicio (dd/mm/aaaa): ");
    scanf("%10s", data_inicio);
    printf("Digite a data final (dd/mm/aaaa): ");
    scanf("%10s", data_final);

    alugarComputador(f_comp, f_clie, f_loca, f_log, cod_clie, cod_comp, quantidade, data_inicio, data_final);
}

// Função para finalizar uma locação (muda ativa para 0) e devolver o computador (disponivel para 1)
void finalizarLocacaoUI(FILE *f_comp, FILE *f_loca, FILE *f_log)
{
    int cod_locacao;
    printf("\n--- Finalizar Locacao ---\n");
    printf("Digite o codigo da Locacao a ser finalizada: ");
    scanf("%d", &cod_locacao);

    TLoca *locacao_encontrada = NULL;
    TLoca *loca_temp;
    long pos_loca = 0;

    rewind(f_loca);
    // Procura a locação
    while ((loca_temp = leloca(f_loca)) != NULL)
    {
        if (loca_temp->cod == cod_locacao)
        {
            locacao_encontrada = loca_temp;
            break;
        }
        pos_loca++;
        free(loca_temp);
    }

    if (locacao_encontrada == NULL)
    {
        printf("Locacao com codigo %d nao encontrada.\n", cod_locacao);
        fprintf(f_log, "Falha ao finalizar: locacao %d nao encontrada.\n", cod_locacao);
        return;
    }

    if (locacao_encontrada->ativa == 0)
    {
        printf("Esta locacao ja esta finalizada.\n");
        fprintf(f_log, "Info: Tentativa de finalizar locacao %d que ja esta inativa.\n", cod_locacao);
        free(locacao_encontrada);
        return;
    }
    // Atualiza a locação para inativa
    locacao_encontrada->ativa = 0;
    fseek(f_loca, pos_loca * sizeof(TLoca), SEEK_SET);
    salvaloca(locacao_encontrada, f_loca);
    printf("Locacao %d finalizada com sucesso.\n", cod_locacao);
    fprintf(f_log, "Locacao %d finalizada.\n", cod_locacao);

    TComp *comp_temp;
    long pos_comp = 0;
    rewind(f_comp);
    while ((comp_temp = lecomp(f_comp)) != NULL)
    {
        if (comp_temp->cod == locacao_encontrada->cod_comp)
        {
            comp_temp->disponivel = 1; // Torna disponível
            fseek(f_comp, pos_comp * sizeof(TComp), SEEK_SET);
            salvacomp(comp_temp, f_comp);
            printf("Computador de codigo %d agora esta disponivel.\n", comp_temp->cod);
            fprintf(f_log, "Computador %d disponibilizado.\n", comp_temp->cod);
            free(comp_temp);
            break;
        }
        pos_comp++;
        free(comp_temp);
    }

    free(locacao_encontrada);
}

// Função para listar todas as locações de um cliente específico
void visualizarLocacoesClienteUI(FILE *f_loca, FILE *f_log)
{
    int cod_clie;
    printf("\n--- Visualizar Locacoes por Cliente ---\n");
    printf("Digite o codigo do Cliente: ");
    scanf("%d", &cod_clie);

    TLoca *loca_temp;
    int encontradas = 0;
    rewind(f_loca);

    printf("\nLocacoes encontradas para o cliente %d:\n", cod_clie);
    while ((loca_temp = leloca(f_loca)) != NULL)
    {
        if (loca_temp->cod_clie == cod_clie)
        {
            imprimeloca(loca_temp);
            encontradas++;
        }
        free(loca_temp);
    }

    if (encontradas == 0)
    {
        printf("Nenhuma locacao encontrada para este cliente.\n");
    }
    fprintf(f_log, "Busca por locacoes do cliente %d realizada. %d encontradas.\n", cod_clie, encontradas);
}

void menuBuscaSequencial(FILE *f_comp, FILE *f_clie, FILE *f_loca, FILE *f_log)
{
    int escolha, chave;
    printf("\n--- Busca Sequencial ---\n");
    printf("1. Buscar Computador\n2. Buscar Cliente\n3. Buscar Locacao\nEscolha: ");
    scanf("%d", &escolha);
    printf("\nDigite o codigo (chave) a ser buscado: ");
    scanf("%d", &chave);

    switch (escolha)
    {
    case 1:
    {
        TComp *computador_encontrado = (TComp *)buscaSequencialGenerica(
            chave, f_comp, f_log, comparaTComp, sizeof(TComp));

        if (computador_encontrado != NULL)
        {
            printf("\nComputador encontrado: ID %d, Marca %s, Modelo %s, Processador %s, Valor %f, Disponivel %d\n", computador_encontrado->cod, computador_encontrado->marca, computador_encontrado->modelo, computador_encontrado->processador, computador_encontrado->valor, computador_encontrado->disponivel);
            free(computador_encontrado);
        }
        else
        {
            printf("\nComputador com ID %d nao encontrado.\n", chave);
        }
        break;
    }
    case 2:
    {
        TClie *cliente_encontrado = (TClie *)buscaSequencialGenerica(
            chave, f_clie, f_log, comparaTClie, sizeof(TClie));

        if (cliente_encontrado != NULL)
        {
            printf("\nCliente encontrado: ID %d, Nome %s, CNPJ/CPF %s, Telefone %d, Email %s\n", cliente_encontrado->cod, cliente_encontrado->nome, cliente_encontrado->cpf_cnpj, cliente_encontrado->telefone, cliente_encontrado->email);
            free(cliente_encontrado);
        }
        else
        {
            printf("\nCliente com ID %d nao encontrado.\n", chave);
        }
        break;
    }
    case 3:
    {
        TLoca *locacao_encontrada = (TLoca *)buscaSequencialGenerica(
            chave, f_loca, f_log, comparaTLoca, sizeof(TLoca));

        if (locacao_encontrada != NULL)
        {
            printf("\nLocacao encontrada: ID %d, com data de inicio %s e data de fim %s, e ativa %d\n", locacao_encontrada->cod, locacao_encontrada->data_inicial, locacao_encontrada->data_final, locacao_encontrada->ativa);
            int id_cliente_da_locacao = locacao_encontrada->cod_clie;
            TClie *cliente_da_locacao = (TClie *)buscaSequencialGenerica(
                id_cliente_da_locacao, f_clie, f_log, comparaTClie, sizeof(TClie));

            if (cliente_da_locacao != NULL)
            {
                printf("\nCliente: ID %d, Nome: %s\n", cliente_da_locacao->cod, cliente_da_locacao->nome);
                free(cliente_da_locacao);
            }
            else
            {
                printf("\nCliente com ID %d nao encontrado para esta locacao.\n", id_cliente_da_locacao);
            }
        }
        break;
    }
    default:
        printf("\nOpcao invalida.\n");
    }
}

void menuBuscaBinaria(FILE *f_comp, FILE *f_clie, FILE *f_loca, FILE *log)
{
    int escolha, chave;
    printf("\n--- Busca Binaria ---\n");
    printf("LEMBRE-SE: Os dados devem estar ordenados para a busca binaria funcionar.\n");
    printf("1. Buscar Computador\n2. Buscar Cliente\n3. Buscar Locacao\nEscolha: ");
    scanf("%d", &escolha);
    printf("Digite o codigo (chave) a ser buscado: ");
    scanf("%d", &chave);

    rewind(f_comp);
    rewind(f_clie);
    rewind(f_loca);

    switch (escolha)
    {
    case 1:
    {
        TComp *comp = buscaComputadorBinario(f_comp, chave, log);
        if (comp != NULL)
        {
            imprimecomp(comp);
            free(comp);
        }
        else
        {
            printf("Computador com ID %d nao encontrado.\n", chave);
        }
        break;
    }
    case 2:
    {
        TClie *clie = buscaClienteBinario(f_clie, chave, log);
        if (clie != NULL)
        {
            imprimeclie(clie);
            free(clie);
        }
        else
        {
            printf("Cliente com ID %d nao encontrado.\n", chave);
        }
        break;
    }
    case 3:
    {
        TLoca *loca = buscaLocacaoBinario(f_loca, chave, log);
        if (loca != NULL)
        {
            imprimeloca(loca);

            // Buscar cliente associado
            rewind(f_clie);
            TClie *clie = buscaClienteBinario(f_clie, loca->cod_clie, log);
            if (clie)
            {
                printf("\nCliente associado:\n");
                imprimeclie(clie);
                free(clie);
            }
            else
            {
                printf("Cliente com ID %d nao encontrado.\n", loca->cod_clie);
            }

            // Buscar computador associado
            rewind(f_comp);
            TComp *comp = buscaComputadorBinario(f_comp, loca->cod_comp, log);
            if (comp)
            {
                printf("\nComputador associado:\n");
                imprimecomp(comp);
                free(comp);
            }
            else
            {
                printf("Computador com ID %d nao encontrado.\n", loca->cod_comp);
            }

            free(loca);
        }
        else
        {
            printf("Locacao com ID %d nao encontrada.\n", chave);
        }
        break;
    }
    default:
        printf("Opcao invalida.\n");
    }
}
