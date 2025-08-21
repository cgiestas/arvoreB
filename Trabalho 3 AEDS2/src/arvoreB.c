#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// nó folha não possui filhos(não aponta para outros nós)
// nó não folha possui filhos(ponteiros para outros nós)
#define ORDEM 2 // cada nó vai ter no mínimo 2 e no máximo 4 chaves
#define MAX_CHAVES (2 * ORDEM)
#define MAX_FILHOS (2 * ORDEM + 1)

typedef struct No
{
    int m;              // qtd de chaves armazenadas no nó
    bool eh_folha;      // 1 se for folha, 0 se não for
    int s[MAX_CHAVES];  // array para as chaves
    long p[MAX_FILHOS]; // ponteiro para array de ponteiros para os nós filhos
} TNo;

typedef struct ArvoreB
{
    FILE *arquivo;      // ponteiro para arvoreB.dat
    long endereco_raiz; // endereço do nó raiz no arquivo
} TArvoreB;

void le_no(TArvoreB *arvore, long endereco, TNo *destino)
{
    fseek(arvore->arquivo, endereco, SEEK_SET);
    fread(destino, sizeof(TNo), 1, arvore->arquivo);
    // posiciona o cursor do arquivo no endereço desejado e lê o bloco de dados
}

void escreve_no(TArvoreB *arvore, long endereco, TNo *no)
{
    fseek(arvore->arquivo, endereco, SEEK_SET);
    fwrite(no, sizeof(TNo), 1, arvore->arquivo);
}

long busca(TArvoreB *arvore, long endereco, int ch)
{
    if (endereco == -1)
    {
        return -1; // chegamos em um nó invalido, para a busca
    }

    TNo no;
    le_no(arvore, endereco, &no);

    int i = 0;
    while (i < no.m && ch > no.s[i])
    {
        i++;
    }

    if (i < no.m && ch == no.s[i])
    {
        return endereco; // o nó foi encontrado
    }

    if (no.eh_folha)
    {
        return -1; // se não encontrou e o nó é folha, a chave não existe
    }

    return busca(arvore, no.p[i], ch);
}

int encontra_pos(TNo *no, int ch)
{
    int i = 0;
    // compara até que a chave nova não seja maior que as chaves no array
    while (i < no->m && ch > no->s[i])
    {
        i++;
    }
    return i; // retorna o índice onde a chave deve ficar
}

TNo *cria_no(TArvoreB *arvore, bool eh_folha, long *endereco_novo)
{
    TNo *novo_no = malloc(sizeof(TNo));
    novo_no->eh_folha = eh_folha; // inicializa se é folha ou não(passado como arg)
    novo_no->m = 0;               // inicializa 0 chaves no nó
    // inicializa o array de chaves e os ponteiros como NULL(-1 é null lógico)
    for (int i = 0; i < MAX_CHAVES; i++)
    {
        novo_no->s[i] = -1;
    }
    for (int i = 0; i < MAX_FILHOS; i++)
    {
        novo_no->p[i] = -1;
    }
    // marca a posição correta no arquivo
    fseek(arvore->arquivo, 0, SEEK_END);
    // o endereço do novo nó é a posição dele no arquivo
    *endereco_novo = ftell(arvore->arquivo);
    // escreve o nó no arquivo
    escreve_no(arvore, endereco_novo, novo_no);
    // retorna o novo nó
    return novo_no;
}
// se o nó está cheio, divide em 2 nós com ordem-1 chaves
// no_dividir é o nó a ser dividido passado como argumento
void dividir_no(TArvoreB *arvore, TNo *pai, long endereco_pai, TNo *filho, long endereco_filho)
{
    long endereco_novo; // criando o endereço do novo nó
    TNo *novo_no = cria_no(arvore, filho->eh_folha, &endereco_novo);
    int chave_meio = ORDEM; // variavel aux para a divisão
    // copiando metade das chaves para o novo nó(apenas chaves maiores do que a chave do meio)
    for (int i = 0; i < ORDEM - 1; i++)
    {
        novo_no->s[i] = filho->s[chave_meio + 1 + i]; // copia chaves para o novo nó
        novo_no->p[i] = filho->p[chave_meio + 1 + i]; // copia os ponteiros para o novo nó
    }
    // caso o filho que estamos dividindo não seja folha(ou seja, tem filhos abaixo dele),
    // o novo nó recebe o último ponteiro do filho que dividimos
    if (!filho->eh_folha)
    {
        novo_no->p[ORDEM - 1] = filho->p[MAX_CHAVES];
    }
    // define que agora ambos nós tem ordem-1 chaves
    novo_no->m = ORDEM - 1;
    filho->m = ORDEM - 1;
    // transferindo a chave do  meio para o pai
    le_no(arvore, &endereco_pai, pai);
    // passa cada chave 1 casa para frente (direita) até que a chave do meio possa entrar no array
    int j = pai->m - 1;
    while (j >= 0 && pai->s[j] > filho->s[chave_meio])
    {
        pai->s[j + 1] = pai->s[j];
        pai->p[j + 2] = pai->p[j + 1];
        j--;
    }
    // insere a chave do meio no array
    pai->s[j + 1] = filho->s[chave_meio];
    pai->p[j + 2] = endereco_novo;
    pai->m++;
    // atualiza os valores no arquivo
    escreve_no(arvore, endereco_filho, filho);
    escreve_no(arvore, endereco_novo, novo_no);
    escreve_no(arvore, endereco_pai, pai);
}

void inserir(TArvoreB *arvore, TNo *raiz, int ch)
{
    if (raiz->m == MAX_CHAVES)
    {
        long novo_end_raiz;
        TNo *nova_raiz = cria_no(arvore, 0, novo_end_raiz);
        dividir_no(arvore, nova_raiz, arvore->endereco_raiz, raiz, arvore->endereco_raiz);
        raiz = nova_raiz;
    }
    // inserir_naoCheio
}

void inserir_naoCheio(TArvoreB *arvore, long endereco_no, int ch)
{
    TNo *no;
    le_no(arvore, endereco_no, no);
    // percorrendo o array para descobrir o índice em que a nova chave deve entrar
    int i = encontra_pos(no, ch);
    int duplicado = busca(arvore, endereco_no, ch);
    if (duplicado != -1)
    {
        printf("\nEssa chave ja existe!");
        return;
    }

    // caso 1: se nó for folha
    if (no->eh_folha)
    {
        // movendo as chaves para posicionar a nova no espaço correto
        for (int j = no->m - 1; j >= i; j--)
        {
            no->s[j + 1] = no->s[j];
        }
        no->s[i] = ch;                       // atribui a chave ao indice correto
        no->m++;                             // incrementa a contagem de chaves existentes
        escreve_no(arvore, endereco_no, no); // atualiza os dados no aquivo
        return;
    }

    // caso 2: o nó é interno(tem filhos)
    if (!no->eh_folha)
    {
        long endereco_filho = no->p[i];
        TNo *filho;
        le_no(arvore, endereco_filho, filho);
        // verifica se o nó original(agora pai) está cheio
        if (filho->m == MAX_CHAVES)
        {
            dividir_no(arvore, no, endereco_no, filho, endereco_filho);
            escreve_no(arvore, endereco_no, no); // atualiza no arquivo
            le_no(arvore, endereco_no, no);      // lê o nó pai novamente, uma vez que foi atualizado
            // decidindo se a chave desce para a esquerda ou direita
            if (ch > no->s[i])
            {
                endereco_filho = no->p[i + 1];
            }
            else
            {
                endereco_filho = no->p[i];
            }
        }
        inserir_naoCheio(arvore, endereco_filho, ch);
    }
}

// implementar função excluir
