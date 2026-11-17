#include <stdlib.h>
#include <string.h>
#include "avl.h"

static int altura(noavl *n) {return n? n->altura : 0;}
static int maior (int a, int b) {return a>b?a:b;}
static void atualizar (noavl *n) {n->altura = 1 + maior(altura(n->esq), altura(n->dir));}
static int fator (noavl *n) {return n ? altura(n->esq) - altura(n->dir):0;}
static noavl* rot_dir(noavl *y){
    noavl *x = y->esq;
    y->esq = x->dir;
    x->dir = y;
    atualizar(y);
    atualizar(x);
    return x;
}

static noavl* rot_esq(noavl *x){
    noavl *y = x->dir;
    x->dir = y->esq;
    y->esq =x;
    atualizar(x);
    atualizar(y);
    return y;
}

static noavl* balancear (noavl *n){
    atualizar(n);
    int f = fator(n);
    if(f>1){
        if(fator(n->esq) <0) n->esq = rot_esq(n->esq);
        return rot_dir(n);
    }
    if(f<-1){
        if(fator(n->dir)>0) n->dir = rot_dir(n->dir);
        return rot_esq(n);
    }
    return n;
}
noavl* avl_inserir(noavl *raiz, Paciente p, int *ok){
    if(!raiz){
        noavl *n =malloc(sizeof(noavl));
        if (!n) {*ok=0; return NULL;}
        n->dados =p;
        n->altura = 1;
        n->esq = n->dir = NULL;
        *ok=1;
        return n;
    }
    int c = strcmp(p.cpf, raiz->dados.cpf);
    if(c<0) raiz->esq =avl_inserir(raiz->esq, p, ok);
    else if (c>0) raiz->dir = avl_inserir(raiz->dir,p,ok);
    else{*ok=0;return raiz;}
    return balancear (raiz);

}

static noavl* menor_no(noavl *n){
    while(n->esq) n = n-> esq;
    return n;
}

noavl* avl_remover(noavl *raiz, const char *cpf, int *ok){
    if(!raiz) {*ok = 0; return NULL;}
    int c=strcmp(cpf, raiz->dados.cpf);
    if(c<0) raiz-> esq = avl_remover(raiz->esq,cpf, ok);
    else if(c>0)raiz->dir =avl_remover(raiz->dir, cpf, ok);
    else{
        *ok =1;
        if(!raiz->esq || !raiz->dir){
            noavl *filho =raiz->esq ? raiz->esq :raiz->dir;
            free(raiz);
            return filho;
        }
        noavl *suc = menor_no(raiz->dir);
        raiz->dados = suc->dados;
        int ignorar;
        raiz->dir = avl_remover(raiz->dir, suc->dados.cpf, &ignorar);

    }
    return balancear(raiz);
}

Paciente* avl_buscar(noavl *raiz, const char *cpf){
    while(raiz){
        int c = strcmp(cpf, raiz->dados.cpf);
        if(c==0) return &raiz->dados;
        raiz = (c<0) ? raiz->esq :raiz->dir;

    }
    return NULL;
}

void avl_ordenada(noavl *raiz, void(*visitar)(Paciente *)){
    if(!raiz)return;
    avl_ordenada(raiz->esq, visitar);
    visitar(&raiz->dados);
    avl_ordenada(raiz->dir, visitar);
}

int avl_contar(noavl *raiz){
    return raiz ? 1 + avl_contar(raiz->esq)+ avl_contar(raiz->dir):0;
}
void avl_liberar(noavl *raiz){
    if(!raiz)return;
    avl_liberar(raiz->esq);
    avl_liberar(raiz->dir);
    free(raiz);
}
