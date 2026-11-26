#ifndef AVL_H
#define AVL_H
#include "modelos.h"

typedef struct noavl {
    // usa cpf como chave
    Paciente dados;
    int altura;
    //organiza cpf menores e maiores
    struct noavl *esq, *dir;
}noavl;
//procura um espaço nulo a ser inserido o novo cpf; se já foi inserido retorna ok = 0
noavl* avl_inserir(noavl *raiz, Paciente p,  int *ok);
// remoção dependendo do número de filhos daquele nó a ser removido
noavl* avl_remover(noavl *raiz, const char *cpf, int *ok);
Paciente* avl_buscar(noavl *raiz, const char *cpf);
void avl_ordenada(noavl *raiz, void (*visitar)(Paciente *));
int avl_contar(noavl *raiz);
void avl_liberar(noavl *raiz);
#endif


// O código h. e .c da avl servem para fazer o armazenamento de dos dados dos pacientes, essa estrutura foi escolhida 
// pois permite a manipulação de informações ideais quando se fala de desenvolvimento de software.
// Aqui conseguimos exergar a ideia de um crud, onde conseguimos criar, remover e buscar.