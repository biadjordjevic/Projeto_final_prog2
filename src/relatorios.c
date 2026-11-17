#include <stdio.h>
#include <time.h>
#include "relatorios.h"
static noavl *avl_relatorio;
static void mostrar_nome(const char *nome, const char *cpf){
    Paciente *p =avl_buscar(avl_relatorio,cpf);
    printf("%-30s %-12s %s\n", nome, cpf, p ? p->enfermidade : "");

}

void relatorio_pacientes_nome(noabb *abb, noavl *avl){
    avl_relatorio = avl;
    printf("%-30s %-12s %s\n", "NOME", "CPF", "ENFERMIDADE");
    abb_ordenada(abb,mostrar_nome);


}

static void mostrar_paciente(Paciente *p){
    printf("%-12s %-30s %s\n", p->cpf, p->nome, p->em_espera ? "em espera" :"");

}

void relatorio_pacientes_cpf(noavl *avl){
    printf("%-12s %-30s %s\n", "CPF", "NOME", "SITUACAO");
avl_ordenada(avl, mostrar_paciente);
}

void relatorio_espera(fila *fl, noavl *avl){
    int pos=1;
    printf("pacientes na lista de espera: %d\n", fl->tamanho);
    for(nofila *n = fl->inicio; n;n=n->prox){
        Paciente*p =avl_buscar(avl, n->cpf);
        printf("%d. %-30s desde %s\n", pos++, p ?p->nome:n->cpf, n->data_entrada);
    }
}

static void mostrar_consulta(Consulta *c, noavl *avl){
    Paciente *p =avl_buscar(avl, c->cpf);
    printf("%-4d %s %s  %-30s %s\n", c->id, c->data_consulta, c->hora_consulta,
    p ? p->nome : c->cpf, c->observacoes);
}

void relatorio_consultas(ListaDupla *l, noavl *avl){
    printf("Total de consultas: %d\n", l->tamanho);
    for(NoLD *n = l->inicio;n;n=n->prox)
    mostrar_consulta(&n->dados, avl);
}

static long long chave_agora(void){
    time_t t = time (NULL);
    struct tm *d = localtime(&t);
    return(d->tm_year + 1900) * 100000000LL + (d->tm_mon+ 1) *  1000000LL
    + d->tm_mday * 10000LL + d->tm_hour * 100LL + d->tm_min;
    
}

void relatorio_proximas_consultas(ListaDupla *l, noavl *avl){
    int qtd = 0;
    for(NoLD *n = ld_primeiro(l, chave_agora()); n; n = n ->prox){
        mostrar_consulta(&n->dados, avl);
        qtd++;
    }
    printf("Consultas pendentes: %d\n", qtd);
    
}