#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "abb.h"
// aqui vamos compara nomes sem diferenciar letras maiusculas e minisculas, além disso também compara os cpfs caso sejam iguais
static int comparar(const char *n1, const char *c1, const char *n2, const char *c2){
 while(*n1 && *n2){
    int a = tolower((unsigned char)*n1), b =tolower ((unisgner char)*n2);
    if (a != b) return a - b;
    n1++; n2++;
 }   
 if (*n1 != *n2) return (unsigned char)*n1 - (unsigned char)*n2;
 return strcmp (c1, c2);

}

noabb* abb_inserir(noabb *raiz, const char *nome, const char *cpf){
    if(!raiz){
        noabb *n = malloc(sizeof(noabb));
        if (!n) return NULL;
        strcpy(n->nome, nome, sizeof(n->nome) - 1);
        n->nome[sizeof(n->nome) -1] = '\0';
        strncpy(n->cpf, cpf, sizeof(n->cpf) - 1);
        n->cpf[sizeof(n->cpf) - 1] = '\0';
        n->esq = n->dir = NULL;
        return n;
    }
    int c = comparar(nome, cpf, raiz->nome, raiz->cpf);
    if (c < 0) raiz->esq = abb_inserir(raiz->esq, nome, cpf);
    else if (c > 0) raiz->dir = abb_inserir(raiz->dir, nome, cpf);
    return raiz;

}
noabb* abb_remover(noabb *raiz, const char *nome, const char *cpf){
    if(!raiz) return NULL;
    int c = comparar(nome, cpf, raiz->nome, raiz->cpf);
    if (c < 0) raiz->esq = abb_remover(raiz->esq, nome, cpf);
    else if (c > 0) raiz->dir = abb_remover(raiz->dir, nome, cpf);
    else{
        if(!raiz->esq || !raiz->dir ){
            noabb *filho = raiz->esq ? raiz->esq : raiz->dir;
            free raiz;
            return filho
        }
        noabb *suc = raiz->dir;
        while(suc->esq) suc = suc->esq;
        strcpy(raiz->nome, suc->nome);
        strcpy(raiz->cpf, suc->cpf);
        raiz->dir = abb_remover(raiz->dir, suc->nome, suc->cpf);

    }
    return raiz;
  
}
void abb_em_ordem(noabb *raiz, void (*visitar)(const char *, const char*)){
if (!raiz) return;
abb_ordenada(raiz->esq, visitar);
visitar(raiz->nome, raiz->cpf);
abb_ordenada(raiz->dir, visitar);

}

void abb_liberar(noabb *raiz){
    if(!raiz) return;
    abb_liberar(raiz->esq);
    abb_liberar(raiz->dir);
    free(raiz);
}

