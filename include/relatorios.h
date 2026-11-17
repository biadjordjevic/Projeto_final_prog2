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