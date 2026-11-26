#ifndef PERSISTENCIA_H
#define PERSISTENCIA_H
#include "avl.h"
#include "abb.h"
#include "fila.h"
#include "lista-dupla.h"

int salvar_pacientes(const char *caminho, noavl *raiz);
int carregar_pacientes(const char *caminho, noavl **avl, noabb **abb);
int salvar_espera(const char *caminho, fila *f);
int carregar_espera(const char *caminho, fila *f, noavl *pacientes);
int salvar_consultas(const char *caminho, ListaDupla *l);
int carregar_consultas(const char *caminho, ListaDupla *l, int *proximo_id);
#endif

//insere os dados nas planilhas  conforme o fluxo de cada estrutura solicita