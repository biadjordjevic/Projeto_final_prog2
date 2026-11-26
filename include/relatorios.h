#ifndef RELATORIOS_H
#define RELATORIOS_H
#include "avl.h"
#include "abb.h"
#include "fila.h"
#include "lista-dupla.h"
void relatorio_pacientes_nome(noabb *abb, noavl *avl);
void relatorio_pacientes_cpf(noavl *avl);
void relatorio_espera(fila *fl,noavl *avl) ;
    void relatorio_consultas(ListaDupla *l, noavl *avl);
void relatorio_proximas_consultas(ListaDupla *l,noavl *avl);
void  relatorio_resumo(noavl *avl,fila *fl,ListaDupla *l)  ;
#endif

//Os relatórios não guardam dado nenhum: percorrem a estrutura que dá a ordem certa (ABB para nome, AVL para CPF, fila para espera, lista para data) e usam o CPF para buscar o restante na AVL.