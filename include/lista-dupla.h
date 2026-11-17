#ifndef LISTA_DUPLA_H
#define LISTA_DUPLA_H

#include "modelos.h"

typedef struct NoLD {
    Consulta dados;
    struct NoLD *ant, *prox;
} NoLD;

typedef struct {
    NoLD *inicio, *fim;
    int tamanho;
} ListaDupla;

void ld_inicializar(ListaDupla *l);
int ld_inserir_ordenado(ListaDupla *l, Consulta c);
int ld_remover(ListaDupla *l, int id);
NoLD* ld_buscar_id(ListaDupla *l, int id);
NoLD* ld_primeiro(ListaDupla *l, long long chave);
void ld_liberar(ListaDupla *l);

#endif