#ifndef AVL_H
#define AVL_H
#include "modelos.h"

typedef struct noavl {
    Paciente dados;
    int altura;
    struct noavl *esq, *dir;
}noavl;

noavl* avl_inserir(noavl *raiz, Paciente p,  int *ok);
noavl* avl_remover(noavl *raiz, const char *cpf, int *ok);
Paciente* avl_buscar(noavl *raiz, const char *cpf);
void avl_ordenada(noavl *raiz, void (*visitar)(Paciente *));
int avl_contar(noavl *raiz);
void avl_liberar(noavl *raiz);
#endif
