#include <stdlib.h>
#include "lista-dupla.h"

void ld_inicializar(ListaDupla *l){
    l->inicio = l->fim = NULL;
    l->tamanho = 0;
}

int ld_inserir_ordenado(ListaDupla *l, Consulta c){
    NoLD *novo = malloc(sizeof(NoLD));
    if(!novo) return 0;
    novo->dados = c;

    // aqui será a busca do nó com data maior
NoLD *p = l->inicio;
while (p && p->dados.chave_data <= c.chave_data)p=p->prox;

if(!p){
    novo->prox = NULL;
    novo->ant = l->fim;
    if (l->fim) l->fim->prox = novo; else l->inicio = novo;
    l->fim=novo;
} 
else {
    novo->prox = p;
    novo ->ant = p->ant;
    if(p->ant) p->ant->prox = novo; else l->inicio = novo;
    p->ant= novo;
}
l->tamanho++;
return 1;
}

NoLD* ld_buscar_id(ListaDupla *l, int id){
    for (NoLD *p = l->inicio; p; p=p->prox)
    if(p->dados.id == id) return p;
    return NULL;

}

int ld_remover(ListaDupla *l, long chave){
    NoLD *p = ld_buscar_id(l, id);
    if (!p) return 0;
    if (p->ant) p->ant->prox = p->prox; else l->inicio = p->prox;
    if (p->prox) p->prox->ant = p->ant; else l->fim = p->ant;
    free(p);
    l->tamanho--;
    return 1;
}

NoLD* ld_primeiro_a_partir(ListaDupla *l, long chave){
    NoLD *p = l ->inicio;
    while (p && p->dados.chave_data < chave) p =p->prox;
    return p
}

void ld_liberar(ListaDupla *l){
    NoLD *p = l->inicio;
    while(p){
        NoLD *prox = p->prox;
        free(p);
        p = prox;
    }
    ld_inicializar(l);
}
