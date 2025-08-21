#include "componentes.h"
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <time.h>

// Cria computador.
TComp *computador(int cod, char *marca, char *modelo,char *processador, float valor, int disponivel) {
    TComp *comp = (TComp *) malloc(sizeof(TComp));
    //inicializa espaco de memoria com ZEROS
    if (comp) memset(comp, 0, sizeof(TComp));
    //copia valores para os campos de func
    comp->cod = cod;
    strcpy(comp->marca, marca);
    strcpy(comp->modelo, modelo);
    strcpy(comp->processador, processador);
    comp->valor = valor;
    comp->disponivel = disponivel;
 
    return comp;
}


TClie *cliente(int cod, char *nome, char *cpf_cnpj, int telefone, char *email) {
    TClie *clie = (TClie *) malloc(sizeof(TClie));
    //inicializa espaco de memoria com ZEROS
    if (clie) memset(clie, 0, sizeof(TClie));
    //copia valores para os campos de func
    clie->cod = cod;
    strcpy(clie->nome, nome);
    strcpy(clie->cpf_cnpj, cpf_cnpj);
    clie->telefone = telefone;
    strcpy(clie->email, email);
    return clie;
}

TLoca *locacao(int cod, int cod_comp ,int cod_clie,int quantidade, char *data_inicial, char *data_final, float valor_total, int ativa) {
    TLoca *loca = (TLoca *) malloc(sizeof(TLoca));
    //inicializa espaco de memoria com ZEROS
    if (loca) memset(loca, 0, sizeof(TLoca));
    //copia valores para os campos de func
    loca->cod = cod;
    loca->cod_comp = cod_comp;
    loca->cod_clie = cod_clie;
    loca->quantidade = quantidade;
    strcpy(loca->data_inicial, data_inicial);
    strcpy(loca->data_final, data_final);
    loca->valor_total = valor_total;
    loca->ativa = ativa;
    return loca;
}


// Salva funcionario no arquivo out, na posicao atual do cursor
void salvacomp(TComp *comp, FILE *out) {
    fwrite(&comp->cod, sizeof(int), 1, out);
    //func->nome ao inves de &func->nome, pois string ja eh um ponteiro
    fwrite(comp->marca, sizeof(char), sizeof(comp->marca), out);
    fwrite(comp->modelo, sizeof(char), sizeof(comp->modelo), out);
    fwrite(comp->processador, sizeof(char), sizeof(comp->processador), out);
    fwrite(&comp->valor, sizeof(float), 1, out);
    fwrite(&comp->disponivel, sizeof(int),1, out);
}

// Salva funcionario no arquivo out, na posicao atual do cursor
void salvaclie(TClie *clie, FILE *out) {
    fwrite(&clie->cod, sizeof(int), 1, out);
    //func->nome ao inves de &func->nome, pois string ja eh um ponteiro
    fwrite(clie->nome, sizeof(char), sizeof(clie->nome), out);
    fwrite(clie->cpf_cnpj, sizeof(char), sizeof(clie->cpf_cnpj), out);
    fwrite(&clie->telefone, sizeof(int), 1, out);
    fwrite(clie->email, sizeof(char), sizeof(clie->email), out);
    
}

// Salva funcionario no arquivo out, na posicao atual do cursor
void salvaloca(TLoca *loca, FILE *out) {
    fwrite(&loca->cod, sizeof(int), 1, out);
    fwrite(&loca->cod_comp, sizeof(int), 1, out);
    fwrite(&loca->cod_clie, sizeof(int), 1, out);
    fwrite(&loca->quantidade, sizeof(int), 1, out);
    //func->nome ao inves de &func->nome, pois string ja eh um ponteiro
    fwrite(loca->data_inicial, sizeof(char), sizeof(loca->data_inicial), out);
    fwrite(loca->data_final, sizeof(char), sizeof(loca->data_final), out);
    fwrite(&loca->valor_total, sizeof(float), 1, out);
    fwrite(&loca->ativa, sizeof(int), 1, out);
}
// Le um funcionario do arquivo in na posicao atual do cursor
// Retorna um ponteiro para funcionario lido do arquivo
TComp *lecomp(FILE *in) {
    TComp *comp = (TComp *) malloc(sizeof(TComp));
    if (0 >= fread(&comp->cod, sizeof(int), 1, in)) {
        free(comp);
        return NULL;
    }
    fread(comp->marca, sizeof(char), sizeof(comp->marca), in);
    fread(comp->modelo, sizeof(char), sizeof(comp->modelo), in);
    fread(comp->processador, sizeof(char), sizeof(comp->processador), in);
    fread(&comp->valor, sizeof(float), 1, in);
    fread(&comp->disponivel, sizeof(int), 1, in);
    return comp;
}

// Le um funcionario do arquivo in na posicao atual do cursor
// Retorna um ponteiro para funcionario lido do arquivo
TClie *leclie(FILE *in) {
    TClie *clie = (TClie *) malloc(sizeof(TClie));
    if (0 >= fread(&clie->cod, sizeof(int), 1, in)) {
        free(clie);
        return NULL;
    }
    fread(clie->nome, sizeof(char), sizeof(clie->nome), in);
    fread(clie->cpf_cnpj, sizeof(char), sizeof(clie->cpf_cnpj), in);
    fread(&clie->telefone, sizeof(int), 1, in);
    fread(&clie->email, sizeof(char), sizeof(clie->email), in);
    return clie;
}

// Le um funcionario do arquivo in na posicao atual do cursor
// Retorna um ponteiro para funcionario lido do arquivo
TLoca *leloca(FILE *in) {
    TLoca *loca = (TLoca *) malloc(sizeof(TLoca));
    if (0 >= fread(&loca->cod, sizeof(int), 1, in)) {
        free(loca);
        return NULL;
    }
    fread(&loca->cod_comp, sizeof(int), 1, in);
    fread(&loca->cod_clie, sizeof(int), 1, in);
    fread(&loca->quantidade, sizeof(int), 1, in);
    fread(loca->data_inicial, sizeof(char), sizeof(loca->data_inicial), in);
    fread(loca->data_final, sizeof(char), sizeof(loca->data_final), in);
    fread(&loca->valor_total, sizeof(float), 1, in);
    fread(&loca->ativa, sizeof(int), 1, in);
    return loca;
}

// Imprime computadores
void imprimecomp(TComp *comp) {
    printf("**********************************************");
    printf("\nNotebook de codigo ");
    printf("%d", comp->cod);
    printf("\nMarca: ");
    printf("%s", comp->marca);
    printf("\nmodelo: ");
    printf("%s", comp->modelo);
    printf("\nProcessador: ");
    printf("%s", comp->processador);
    printf("\nValor: ");
    printf("%4.2f", comp->valor);
    printf("\nDisponivel: ");
    printf("%d", comp->disponivel);
    printf("\n**********************************************");
}

// Imprime clientes
void imprimeclie(TClie *clie) {
    printf("**********************************************");
    printf("\nCliente de codigo ");
    printf("%d", clie->cod);
    printf("\nNome: ");
    printf("%s", clie->nome);
    printf("\nCPF/CNPJ: ");
    printf("%s", clie->cpf_cnpj);
    printf("\nTelefone: ");
    printf("%d", clie->telefone);
    printf("\nEmail: ");
    printf("%s", clie->email);
    printf("\n**********************************************");
}

// Imprime locacoes
void imprimeloca(TLoca *loca) {
    printf("**********************************************");
    printf("\nLocacao de codigo ");
    printf("%d", loca->cod);
    printf("\nCodigo cliente: ");
    printf("%d", loca->cod_clie);
    printf("\nCodigo do computador escolhido: ");
    printf("%d", loca->cod_comp);
    printf("\nQuantidade de computadores: ");
    printf("%d", loca->quantidade);
    printf("\nData de Inicial do Contrato: ");
    printf("%s", loca->data_inicial);
    printf("\nData de Final do Contrato: ");
    printf("%s", loca->data_final);
    printf("\nValor Mensal Locacao: ");
    printf("%4.2f", loca->valor_total);
    printf("\nSituacao da Locacao: ");
    printf("%d", loca->ativa);
    printf("\n**********************************************");
}


// Cria a base de dados
void criarBase(FILE *out, int tam, int escolha){

    int vet[tam];
    TComp *com;
    TClie *cli;
    TLoca *loc;

    for(int i=0;i<tam;i++)
        vet[i] = i+1;
    
    embaralha(vet,tam);

    printf("\nGerando a base de dados...\n");

    switch(escolha){
        case 1:
            for (int i=0;i<tam;i++){
                com = computador(vet[i], "Dell", "Inspiron", "I5", (100*i)+100, 1);
                salvacomp(com, out);
                //imprimecomp(com);
                free(com);
            }
            
            break;
        case 2:
            for (int i=0;i<tam;i++){
            cli = cliente(vet[i], "A", "000.000.000-00", 1+i, "aleatorio@xyz.com");
            salvaclie(cli, out);
            //imprimeclie(cli);
            free(cli);
            }
            
            break;
        case 3:
            for (int i=0;i<tam;i++){
            loc = locacao(vet[i], vet[i], vet[i], 2, "01/01/1980","01/02/1980", (100*i)*2, 1);
            salvaloca(loc, out);
            //imprimeloca(loc);
            free(loc);
            }
        
            break;
    }
}

//embaralha base de dados
void embaralha(int *vet, int tam) {

    int tmp;

    srand(time(NULL));

    int trocas = (tam*60)/100;

    for (int t = 1; t<trocas; t++) {
        int i = rand() % tam;
        int j = rand() % tam;
        tmp = vet[i];
        vet[i] = vet[j];
        vet[j] = tmp;
    }
}

void imprimirBase(FILE *out, int escolha){
    printf("\nImprimindo a base de dados...\n");
    rewind(out);

    switch (escolha) {
        case 1: {
            TComp *comp;
            while ((comp = lecomp(out)) != NULL)
                imprimecomp(comp);
            free(comp);
            break;
        }
        case 2: {
            TClie *clie;
            while ((clie = leclie(out)) != NULL)
                imprimeclie(clie);
            free(clie);
            break;
        }
        case 3: {
            TLoca *loca;
            int ativas_encontradas = 0;
            printf("\nExibindo apenas locacoes ATIVAS...\n");
            
            while ((loca = leloca(out)) != NULL) {
                // Verifica se a locação está ativa antes de imprimir
                if (loca->ativa == 1) {
                    imprimeloca(loca);
                    ativas_encontradas++;
                }
                free(loca); // Libera a memória alocada por leloca, independentemente de imprimir ou não
            }
            
            if (ativas_encontradas == 0) {
                printf("Nenhuma locacao ativa no momento.\n");
            }
            break;
        }
    }

}

int compara_comp(const void *a, const void *b)
{
    const TComp *compA = *(const TComp **)a;
    const TComp *compB = *(const TComp **)b;
    if (compA->cod < compB->cod) return -1;
    if (compA->cod > compB->cod) return 1;
    return 0;
}


int compara_clie(const void *a, const void *b)
{
	const TClie *clieA = *(const TClie **)a;
    const TClie *clieB = *(const TClie **)b;
    if (clieA->cod < clieB->cod) return -1;
    if (clieA->cod > clieB->cod) return 1;
    return 0;
}

int compara_loca(const void *a, const void *b)
{
    const TLoca *locaA = *(const TLoca **)a;
    const TLoca *locaB = *(const TLoca **)b;
    if (locaA->cod < locaB->cod) return -1;
    if (locaA->cod > locaB->cod) return 1;
    return 0;
}