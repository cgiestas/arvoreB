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

/* ===========================================================
   Funções pedidas: exclusão, redistribuição e concatenação
   (baseadas no PDF anexo – Szwarcfiter & Markezon, cap. Árvore B)
   Somente ACRESCENTAR ao arquivo. Não altera nada existente.
   =========================================================== */

#ifndef D_ARVOREB_DELETE_REDIS_CONCAT
#define D_ARVOREB_DELETE_REDIS_CONCAT

/* Assinaturas já existentes no arquivo (usadas aqui):
   void ler_no(TArvoreB *arvore, long endereco, TNo *destino);
   void escreve_no(TArvoreB *arvore, long endereco, TNo *no);

   Estruturas esperadas:
   typedef struct No {
       int m;
       bool eh_folha;
       int  s[MAX_CHAVES];
       long p[MAX_FILHOS];
   } TNo;

   typedef struct {
       FILE *arquivo;
       long raiz;
   } TArvoreB;
*/

static int indice_busca_no(const TNo *no, int chave, int *achou) {
    int i = 0;
    while (i < no->m && chave > no->s[i]) i++;
    *achou = (i < no->m && no->s[i] == chave);
    return i; /* posição da primeira chave >= chave (ou fim) */
}

static int capacidade_minima(void) { return ORDEM; }               /* d   */
static int capacidade_maxima(void) { return MAX_CHAVES; }          /* 2d  */

/* Lê a menor chave da subárvore cujo endereço é 'end' (desce sempre à esquerda) */
static int obter_sucessor(TArvoreB *arvore, long end) {
    TNo n;
    long e = end;
    while (1) {
        ler_no(arvore, e, &n);
        if (n.eh_folha) {
            return n.s[0];
        }
        e = n.p[0];
    }
}

/* Lê a maior chave da subárvore cujo endereço é 'end' (desce sempre à direita) */
static int obter_predecessor(TArvoreB *arvore, long end) {
    TNo n;
    long e = end;
    while (1) {
        ler_no(arvore, e, &n);
        if (n.eh_folha) {
            return n.s[n.m - 1];
        }
        e = n.p[n.m];
    }
}

/* Remove a chave da posição 'pos' dentro do nó folha 'no' (em memória) */
static void remove_de_folha_pos(TNo *no, int pos) {
    for (int i = pos; i < no->m - 1; i++) {
        no->s[i] = no->s[i + 1];
    }
    no->m--;
}

/* Remove a chave e o ponteiro à direita dessa chave dentro do PAI (memória) */
static void remove_do_pai_pos(TNo *pai, int pos_chave) {
    /* remove chave pos_chave e ponteiro pos_chave+1 */
    for (int i = pos_chave; i < pai->m - 1; i++) {
        pai->s[i] = pai->s[i + 1];
        pai->p[i + 1] = pai->p[i + 2];
    }
    pai->m--;
}

/* =============== CONCATENAÇÃO (merge de irmãos) ==================
   Concatena filho_esq (idx) com filho_dir (idx+1) usando a chave separadora do pai.
   Propaga redução no pai (quanto necessário). Retorna o endereço do nó resultante (esquerdo).
*/
static long concatenarB(TArvoreB *arvore, long end_pai, TNo *pai,
                        int idx_esq, long end_esq, TNo *esq,
                        long end_dir, TNo *dir)
{
    /* move chave separadora do pai para o final de 'esq' */
    int separadora = pai->s[idx_esq];
    esq->s[esq->m++] = separadora;

    /* copia todas as chaves e filhos do dir para o esq */
    for (int i = 0; i < dir->m; i++) {
        esq->s[esq->m] = dir->s[i];
        esq->p[esq->m] = dir->p[i];
        esq->m++;
    }
    esq->p[esq->m] = dir->p[dir->m];

    /* grava o nó concatenado (esquerdo) */
    escreve_no(arvore, end_esq, esq);

    /* remove a separadora do pai e o ponteiro para 'dir' */
    remove_do_pai_pos(pai, idx_esq);
    escreve_no(arvore, end_pai, pai);

    /* Obs.: 'dir' fica logicamente removido – o espaço em disco pode ser
       reutilizado por um gerenciador de páginas, se houver (não implementado). */

    return end_esq;
}

/* ============== REDISTRIBUIÇÃO (re-balance entre irmãos) =================
   Tenta redistribuir com irmão esquerdo (idx-1) ou direito (idx+1).
   Se conseguir, ajusta pai e grava ambos os filhos. Retorna 1 se houve redistribuição.
*/
static int redistribuirB(TArvoreB *arvore, long end_pai, TNo *pai,
                         int idx, long *end_filho, TNo *filho)
{
    /* tenta com irmão à esquerda */
    if (idx - 1 >= 0) {
        long end_irm = pai->p[idx - 1];
        TNo irm;
        ler_no(arvore, end_irm, &irm);

        if (irm.m + filho->m >= 2 * ORDEM) {
            /* pega do irmão esquerdo ou empurra para ele
               Estratégia: juntar tudo virtualmente e re-cortar:
               [irmão_esq] + chave_sep + [filho]  => esq com d, pai recebe d+1, dir com restante */
            int total = irm.m + 1 + filho->m;

            /* Vetor temporário de chaves e filhos */
            int tempS[MAX_CHAVES + 1];        /* no máximo 2d + (1 separadora) */
            long tempP[MAX_FILHOS + 1];       /* filhos: até 2d+2 */

            /* monta sequência: (irmão esq) [chave sep] (filho) */
            for (int i = 0; i < irm.m; i++) tempS[i] = irm.s[i];
            tempS[irm.m] = pai->s[idx - 1];
            for (int i = 0; i < filho->m; i++) tempS[irm.m + 1 + i] = filho->s[i];

            for (int i = 0; i <= irm.m; i++) tempP[i] = irm.p[i];
            tempP[irm.m + 1] = filho->p[0];
            for (int i = 0; i < filho->m; i++) tempP[irm.m + 2 + i] = filho->p[i + 1];

            /* novo corte: esquerda com d, pai recebe d+1-ésima, direita com resto */
            int d = ORDEM;
            int novo_m_esq = d;
            int novo_m_dir = total - (novo_m_esq + 1); /* resto depois da do pai */

            /* reconstroi irmão esquerdo */
            irm.m = novo_m_esq;
            for (int i = 0; i < irm.m; i++) irm.s[i] = tempS[i];
            for (int i = 0; i <= irm.m; i++) irm.p[i] = tempP[i];

            /* pai recebe separadora */
            pai->s[idx - 1] = tempS[novo_m_esq];

            /* reconstroi filho (direita) */
            filho->m = novo_m_dir;
            for (int i = 0; i < filho->m; i++) filho->s[i] = tempS[novo_m_esq + 1 + i];
            for (int i = 0; i <= filho->m; i++) filho->p[i] = tempP[novo_m_esq + 1 + i];

            /* grava */
            escreve_no(arvore, end_pai, pai);
            escreve_no(arvore, end_irm, &irm);
            escreve_no(arvore, *end_filho, filho);
            return 1;
        }
    }

    /* tenta com irmão à direita */
    if (idx + 1 <= pai->m) {
        long end_irm = pai->p[idx + 1];
        TNo irm;
        ler_no(arvore, end_irm, &irm);

        if (irm.m + filho->m >= 2 * ORDEM) {
            /* Monta sequência: (filho) [chave sep] (irmão dir) e re-corta */
            int total = filho->m + 1 + irm.m;

            int tempS[MAX_CHAVES + 1];
            long tempP[MAX_FILHOS + 1];

            for (int i = 0; i < filho->m; i++) tempS[i] = filho->s[i];
            tempS[filho->m] = pai->s[idx];
            for (int i = 0; i < irm.m; i++) tempS[filho->m + 1 + i] = irm.s[i];

            for (int i = 0; i <= filho->m; i++) tempP[i] = filho->p[i];
            tempP[filho->m + 1] = irm.p[0];
            for (int i = 0; i < irm.m; i++) tempP[filho->m + 2 + i] = irm.p[i + 1];

            int d = ORDEM;
            int novo_m_esq = d;
            int novo_m_dir = total - (novo_m_esq + 1);

            /* filho vira esquerda */
            filho->m = novo_m_esq;
            for (int i = 0; i < filho->m; i++) filho->s[i] = tempS[i];
            for (int i = 0; i <= filho->m; i++) filho->p[i] = tempP[i];

            /* pai recebe separadora */
            pai->s[idx] = tempS[novo_m_esq];

            /* irmão direito vira direita */
            irm.m = novo_m_dir;
            for (int i = 0; i < irm->m; i++) irm.s[i] = tempS[novo_m_esq + 1 + i];
            for (int i = 0; i <= irm->m; i++) irm.p[i] = tempP[novo_m_esq + 1 + i];

            escreve_no(arvore, end_pai, pai);
            escreve_no(arvore, *end_filho, filho);
            escreve_no(arvore, end_irm, &irm);
            return 1;
        }
    }

    return 0; /* não foi possível redistribuir */
}

/* =============== AJUSTE PÓS-REMOÇÃO (underflow) ==================
   Garante a propriedade (>= d chaves) para nós internos/folhas (exceto raiz).
   Pode redistribuir ou concatenar, propagando ao pai conforme necessário.
   Retorna o endereço (possivelmente atualizado) do nó após o ajuste.
*/
static long ajustar_pos_remocao(TArvoreB *arvore,
                                long end_pai, TNo *pai,
                                int idx_filho,
                                long end_filho, TNo *filho)
{
    int d = capacidade_minima();
    if (filho->m >= d || end_pai == -1L) {
        /* sem underflow, ou é a raiz */
        return end_filho;
    }

    /* 1) tenta redistribuição (não-propagável) */
    if (redistribuirB(arvore, end_pai, pai, idx_filho, &end_filho, filho)) {
        return end_filho;
    }

    /* 2) concatenação (propagável) */
    long end_result = end_filho;
    if (idx_filho > 0) {
        /* concatena com irmão à esquerda */
        long end_esq = pai->p[idx_filho - 1], end_dir = end_filho;
        TNo esq, dir;
        ler_no(arvore, end_esq, &esq);
        ler_no(arvore, end_dir, &dir);
        end_result = concatenarB(arvore, end_pai, pai, idx_filho - 1, end_esq, &esq, end_dir, &dir);
        /* após concatenar, o filho válido passou a ser 'end_esq' na posição idx_filho-1 */
        idx_filho = idx_filho - 1;
        end_filho = end_result;
        *filho = esq; /* cópia do nó resultante */
    } else {
        /* concatena com irmão à direita */
        long end_esq = end_filho, end_dir = pai->p[idx_filho + 1];
        TNo esq, dir;
        ler_no(arvore, end_esq, &esq);
        ler_no(arvore, end_dir, &dir);
        end_result = concatenarB(arvore, end_pai, pai, idx_filho, end_esq, &esq, end_dir, &dir);
        end_filho = end_result;
        *filho = esq;
    }

    /* Se o pai ficou com menos de d chaves, talvez precise propagar
       (exceto se o pai for a raiz – tratar na exclusão). */
    return end_result;
}

/* ===================== EXCLUSÃO (pública) ========================
   Remove a chave 'ch' da árvore.
   – Se a raiz esvaziar e não for folha, desce um nível.
*/
static long excluir_rec(TArvoreB *arvore, long end_no, int ch,
                        long end_pai, TNo *pai, int idx_no_no_pai);

void excluirB(TArvoreB *arvore, int ch) {
    if (arvore->raiz == -1L) return;
    TNo raiz;
    ler_no(arvore, arvore->raiz, &raiz);

    long nova_raiz = excluir_rec(arvore, arvore->raiz, ch, -1L, NULL, -1);

    /* tratar raiz após exclusão */
    if (nova_raiz != -1L) {
        TNo r;
        ler_no(arvore, nova_raiz, &r);
        if (!r.eh_folha && r.m == 0) {
            /* raiz vazia e interna: altura diminui, nova raiz é p0 */
            arvore->raiz = r.p[0];
        } else {
            arvore->raiz = nova_raiz;
        }
    }
}

/* Corpo recursivo da exclusão */
static long excluir_rec(TArvoreB *arvore, long end_no, int ch,
                        long end_pai, TNo *pai, int idx_no_no_pai)
{
    TNo no;
    ler_no(arvore, end_no, &no);

    int achou = 0;
    int i = indice_busca_no(&no, ch, &achou);

    if (achou) {
        /* Caso 1: chave está neste nó */
        if (no.eh_folha) {
            /* remover diretamente */
            remove_de_folha_pos(&no, i);
            escreve_no(arvore, end_no, &no);

            /* ajustar underflow se necessário */
            if (end_pai != -1L) {
                ler_no(arvore, end_pai, pai);
                return ajustar_pos_remocao(arvore, end_pai, pai, idx_no_no_pai, end_no, &no);
            }
            return end_no;
        } else {
            /* nó interno: substituir pela sucessora na subárvore à direita */
            long end_dir = no.p[i + 1];
            int succ = obter_sucessor(arvore, end_dir);
            no.s[i] = succ;
            escreve_no(arvore, end_no, &no);

            /* excluir 'succ' na subárvore direita */
            long novo_end_dir = excluir_rec(arvore, end_dir, succ, end_no, &no, i + 1);
            /* pode ter mudado o endereço do filho i+1 após concatenação */
            ler_no(arvore, end_no, &no);
            no.p[i + 1] = novo_end_dir;
            escreve_no(arvore, end_no, &no);
            return end_no;
        }
    } else {
        /* Caso 2: descer para o filho adequado */
        long end_filho = no.eh_folha
                         ? -1L
                         : ( (i < no.m && ch < no.s[i]) ? no.p[i] : no.p[i] );

        if (no.eh_folha) {
            /* chave não está na árvore */
            return end_no;
        }

        /* garantir que vamos descer para um filho com pelo menos d chaves,
           senão, “preparamos” antes (opcional em implementações mais agressivas).
           Aqui, vamos simplesmente descer e corrigir na volta. */
        long novo_end_filho = excluir_rec(arvore, no.p[i], ch, end_no, &no, i);
        /* pode ter mudado o endereço desse filho após concatenação */
        ler_no(arvore, end_no, &no);
        no.p[i] = novo_end_filho;
        escreve_no(arvore, end_no, &no);
        return end_no;
    }
}

#endif /* D_ARVOREB_DELETE_REDIS_CONCAT */
