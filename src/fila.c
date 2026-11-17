#include <stdlib.h>
#include <string.h>
#include "fila.h"

void fila_inicializar(fila *f){
    f->inicio = f->fim=NULL;
    f->tamanho = 0;
}
nofila* fila_buscar(fila *f, const char *cpf){
    for (nofila *p = f->inicio; p;p=p->prox)
    if(strcmp(p->cpf,cpf)==0)return p;
    return NULL;
}

int fila_enfileirar(fila *f, const char *cpf, const char *data_entrada){
    if(fila_buscar(f, cpf))return 0;
    nofila *n = malloc(sizeof(nofila));
    if(!n) return 0;
    strncpy(n->cpf, cpf, sizeof(n->cpf)-1);
    n->cpf[sizeof(n->cpf)-1] = '\0';
    strncpy(n->data_entrada, data_entrada, sizeof(n->data_entrada)-1);
    n->data_entrada[sizeof(n->data_entrada)-1]='\0';
    n->prox = NULL;
    if (f->fim) f->fim->prox = n; else f->inicio = n;
    f->fim = n;
    f->tamanho++;
    return 1;
}
int fila_desenfileirar (fila *f, char *cpf_out){
    if (!f->inicio)return 0;
    nofila *n = f->inicio;
    if(cpf_out) strcpy(cpf_out, n->cpf);
    f->inicio = n->prox;
    if(!f->inicio) f->fim = NULL;
    free(n);
    f->tamanho--;
    return 1;}

int fila_remover_cpf(fila *f, const char *cpf){
    nofila *ant = NULL, *p = f->inicio;
    while(p && strcmp(p->cpf,cpf) !=0 ){ ant = p; p = p->prox;}
if(!p) return 0;
if (ant) ant->prox = p->prox; else f->inicio = p->prox;
if (p == f->fim) f->fim = ant;
free(p);
f->tamanho--;
return 1;
}

void fila_liberar(fila *f){
    while(f->inicio) fila_desenfileirar(f, NULL);

}
