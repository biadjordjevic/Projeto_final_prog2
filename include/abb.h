#ifndef ABB_H
#define ABB_H

typedef struct Noabb{
    char nome[100];
    char cpf[12];
    struct Noabb *esq, *dir;
}noabb;

noabb* abb_inserir(noabb *raiz, const char *nome, const char *cpf);
noabb* abb_remover(noabb *raiz, const char *nome, const char *cpf);
void abb_ordenada(noabb *raiz, void (*visitar)(const char *nome, const char *cpf));
void abb_liberar(noabb *raiz);

#endif

//Ponto de atenção: se os nomes forem inseridos já em ordem alfebetica no sistema, vira uma lista encadeada 