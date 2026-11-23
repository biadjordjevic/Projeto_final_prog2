#include <stdio.h>
#include <string.h>
#include <time.h>
#include "persistencia.h"
#include "relatorios.h"
#include "entrada.h"
#define ARQ_PACIENTES "dados/pacientes.csv"
#define ARQ_ESPERA    "dados/lista-espera.csv"
#define ARQ_CONSULTAS "dados/consultas.csv"

// descarta o que esta na memoria e le de novo os CSVs, para trabalhar
// sempre com os dados mais recentes (inclusive os gravados por outra janela)
static void carregar_tudo(noavl **avl, noabb **abb, fila *espera, ListaDupla *consultas, int *proximo_id){
    avl_liberar(*avl);  *avl = NULL;
    abb_liberar(*abb);  *abb = NULL;
    fila_liberar(espera); fila_inicializar(espera);
    ld_liberar(consultas);
    carregar_pacientes(ARQ_PACIENTES, avl, abb);
    carregar_espera(ARQ_ESPERA, espera, *avl);
    carregar_consultas(ARQ_CONSULTAS, consultas, proximo_id);
}

// grava tudo nos CSVs logo apos cada alteracao
static void salvar_tudo(noavl *avl, fila *espera, ListaDupla *consultas){
    int ok = salvar_pacientes(ARQ_PACIENTES, avl);
    ok = salvar_espera(ARQ_ESPERA, espera) && ok;
    ok = salvar_consultas(ARQ_CONSULTAS, consultas) && ok;
    if(!ok) printf("ATENCAO: nao foi possivel salvar os dados na pasta 'dados'\n");
}

int main(void){
    noavl *avl = NULL;
    noabb *abb = NULL;
    fila espera;
    ListaDupla consultas;
    int proximo_id;
    fila_inicializar(&espera);
    ld_inicializar(&consultas);

    // avisa se o programa foi aberto fora da pasta do projeto
    FILE *teste = fopen(ARQ_CONSULTAS, "a");
    if(teste) fclose(teste);
    else printf("ATENCAO: pasta 'dados' nao encontrada. Execute o programa a partir da pasta do projeto.\n");

    int op, ok;
    char cpf[12];
    do{
        printf("\n1 - Cadastrar paciente\n2 - Buscar paciente\n3 - Remover paciente\n");
        printf("4 - Colocar na lista de espera\n5 - Chamar proximo da espera\n");
        printf("6 - Agendar consulta\n7 - Cancelar consulta\n");
        printf("8 - Pacientes por nome\n9 - Pacientes por CPF\n10 - Lista de espera\n");
        printf("11 - Consultas\n12 - Proximas consultas\n13 - Totais\n14 - Editar consulta\n0 - Sair\nOpcao: ");
        if(scanf("%d", &op) != 1) break;
        carregar_tudo(&avl, &abb, &espera, &consultas, &proximo_id);
        switch(op){
        case 1: {
            Paciente p = {0};
            ler_texto("CPF: ", p.cpf, 11);
            printf("Nome: "); scanf(" %99[^\n]", p.nome);
            ler_texto("CEP: ", p.cep, 9);
            printf("Endereco: "); scanf(" %149[^\n]", p.endereco);
            printf("Bairro: "); scanf(" %59[^\n]", p.bairro);
            printf("Idade: "); scanf("%d", &p.idade);
            ler_data("Data de nascimento (dd/mm/aaaa): ", p.data_nasc, 1);
            printf("Enfermidade: "); scanf(" %99[^\n]", p.enfermidade);
            printf("Docente: "); scanf(" %99[^\n]", p.docente);
            printf("Area de estudo: "); scanf(" %59[^\n]", p.area_estudo);
            printf("Observacoes: "); scanf(" %199[^\n]", p.observacoes);
            avl = avl_inserir(avl, p, &ok);
            if(ok){
                abb = abb_inserir(abb, p.nome, p.cpf);
                printf("Paciente cadastrado\n");
            }
            else printf("CPF ja cadastrado\n");
            break;
        }
        case 2: {
            ler_texto("CPF: ", cpf, 11);
            Paciente *p = avl_buscar(avl, cpf);
            if(p){
                printf("Nome: %s\nIdade: %d\nNascimento: %s\n", p->nome, p->idade, p->data_nasc);
                printf("Endereco: %s, %s - CEP %s\n", p->endereco, p->bairro, p->cep);
                printf("Enfermidade: %s\nDocente: %s\nArea: %s\n", p->enfermidade, p->docente, p->area_estudo);
                printf("Observacoes: %s\n%s\n", p->observacoes, p->em_espera ? "Na lista de espera" : "");
            }
            else printf("Paciente nao encontrado\n");
            break;
        }
        case 3: {
            ler_texto("CPF: ", cpf, 11);
            Paciente *p = avl_buscar(avl, cpf);
            if(!p){ printf("Paciente nao encontrado\n"); break; }
            abb =abb_remover(abb, p->nome, cpf);
            fila_remover_cpf(&espera, cpf);
            avl = avl_remover(avl, cpf, &ok);
            printf("Paciente removido\n");
            break;
        }
        case 4: {
            char data[11];
            time_t t = time(NULL);
            ler_texto("CPF: ", cpf, 11);
            Paciente *p = avl_buscar(avl, cpf);
            if(!p){ printf("Paciente nao encontrado\n"); break; }
            strftime(data, sizeof data, "%d/%m/%Y", localtime(&t));
            if(fila_enfileirar(&espera, cpf, data)){
                p->em_espera = 1;
                printf("Paciente colocado na lista de espera\n");
            }
            else printf("Paciente ja esta na lista de espera\n");
            break;
        }
        case 5:
            if(fila_desenfileirar(&espera, cpf)){
                Paciente *p = avl_buscar(avl, cpf);
                if(p){
                    p->em_espera = 0;
                    printf("Proximo paciente: %s (%s)\n", p->nome, cpf);
                }
            }
            else printf("Lista de espera vazia\n");
            break;
        case 6: {
            Consulta c = {0};
            int d, m, a, h, mi;
            ler_texto("CPF: ", c.cpf, 11);
            if(!avl_buscar(avl, c.cpf)){ printf("Paciente nao encontrado\n"); break; }
            ler_data("Data (dd/mm/aaaa): ", c.data_consulta, 1);
            ler_hora("Hora (hh:mm): ", c.hora_consulta, 1);
            printf("Observacoes: "); scanf(" %299[^\n]", c.observacoes);
            if(sscanf(c.data_consulta, "%d/%d/%d", &d, &m, &a) != 3 || sscanf(c.hora_consulta, "%d:%d", &h, &mi) != 2){
                printf("Data ou hora invalida\n");
                break;
            }
            c.chave_data = a*100000000LL + m*1000000LL + d*10000LL + h*100LL + mi;
            c.id = proximo_id++;
            ld_inserir_ordenado(&consultas, c);
            printf("Consulta %d agendada\n", c.id);
            break;
        }
        case 7: {
            int id;
            printf("Id da consulta: "); scanf("%d", &id);
            if(ld_remover(&consultas, id)) printf("Consulta cancelada\n");
            else printf("Consulta nao encontrada\n");
            break;
        }
        case 8: relatorio_pacientes_nome(abb, avl); break;
        case 9: relatorio_pacientes_cpf(avl); break;
        case 10: relatorio_espera(&espera, avl); break;
        case 11: relatorio_consultas(&consultas, avl); break;
        case 12: relatorio_proximas_consultas(&consultas, avl); break;
        case 13:
            printf("Pacientes cadastrados: %d\n", avl_contar(avl));
            printf("Na lista de espera: %d\n", espera.tamanho);
            printf("Consultas agendadas: %d\n", consultas.tamanho);
            break;
        case 14: {
            int id, d, m, a, h, mi;
            char data[11], hora[6], obs[300];
            printf("Id da consulta: "); scanf("%d", &id);
            NoLD *n = ld_buscar_id(&consultas, id);
            if(!n){ printf("Consulta nao encontrada\n"); break; }
            Consulta c = n->dados;
            printf("Data atual: %s  Hora atual: %s\n", c.data_consulta, c.hora_consulta);
            printf("Observacoes atuais: %s\n", c.observacoes);
            if(ler_data("Nova data (Enter ou - para manter): ", data, 0)) strcpy(c.data_consulta, data);
            if(ler_hora("Nova hora (Enter ou - para manter): ", hora, 0)) strcpy(c.hora_consulta, hora);
            printf("Novas observacoes (- para manter): "); scanf(" %299[^\n]", obs);
            if(strcmp(obs, "-") != 0) strcpy(c.observacoes, obs);
            if(sscanf(c.data_consulta, "%d/%d/%d", &d, &m, &a) != 3 || sscanf(c.hora_consulta, "%d:%d", &h, &mi) != 2){
                printf("Data ou hora invalida\n");
                break;
            }
            c.chave_data = a*100000000LL + m*1000000LL + d*10000LL + h*100LL + mi;
            ld_remover(&consultas, id);
            ld_inserir_ordenado(&consultas, c);
            printf("Consulta %d alterada\n", c.id);
            break;
        }
        case 0: break;
        default: printf("Opcao invalida\n");
        }
        // opcoes que alteram dados: grava na hora
        if(op == 1 || op == 3 || op == 4 || op == 5 || op == 6 || op == 7 || op == 14)
            salvar_tudo(avl, &espera, &consultas);
    }while(op != 0);

    avl_liberar(avl);
    abb_liberar(abb);
    fila_liberar(&espera);
    ld_liberar(&consultas);
    return 0;
}
