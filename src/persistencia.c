#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "persistencia.h"

// Está parte do código é responsável por escrever os campos quebrando a linha e adicionando ponto e vírgula no final de cada um dos campo
static void escrever_campo(FILE  *f, const char *s){
    for (; *s; s++){
        char c = *s;
        if (c == ';') c = ',';
        else if (c == '\n' || c == '\r') c = ' ';
    fputc(c,f);

    }
}
static int dividir (char *linha, char *campos[], int max){
    int n = 0;
    campos[n++]= linha;
    for (char *p = linha; *p && n < max; p++) {
        if (*p == ';'){
            *p = '\0';
            campos[n++] = p + 1;
        
        }
    }
    return n;
}

static void copiar(char *dest, size_t tam, const char *src){
    strncpy(dest, src, tam -1);
    dest[tam-1]='\0';

}
static void tirar_quebra(char *s){s[strcspn(s, "\r\n")] = '\0';}

static FILE *arq; 
static void gravar_paciente(Paciente *p){
    escrever_campo(arq, p->cpf); fputc(';', arq);
    escrever_campo(arq, p->nome);fputc (';', arq);
    escrever_campo(arq, p->cep); fputc(';', arq);
    escrever_campo(arq, p->endereco); fputc(';', arq);
    escrever_campo(arq, p->bairro); fputc(';', arq);
    fprintf(arq, "%d", p->idade); fputc(';', arq);
    escrever_campo(arq, p->data_nasc); fputc(';', arq);
    escrever_campo(arq, p->enfermidade); fputc(';', arq);
    escrever_campo(arq, p->docente); fputc(';', arq);
    escrever_campo(arq, p->area_estudo); fputc(';', arq);
    escrever_campo(arq, p->observacoes);
    fputc('\n', arq);
}

int salvar_pacientes(const char *caminho, noavl *raiz){
    arq = fopen(caminho, "w");
    if (!arq) return 0;
    fprintf(arq, "cpf;nome;cep;endereco;bairro;idade;data_nasc;enfermidade;docente;area_estudo;observacoes\n");
    avl_ordenada(raiz, gravar_paciente);
    fclose(arq);
    arq = NULL;
    return 1;
}

int carregar_pacientes(const char *caminho, noavl **avl, noabb **abb){
    FILE *f = fopen (caminho, "r");
    if(!f)return 1;
    char linha[1024], *c[11];
    fgets(linha, sizeof linha, f);
    while(fgets(linha, sizeof linha, f)){
        tirar_quebra(linha);
        if (linha[0] == '\0') continue;
        if (dividir(linha, c, 11) <11) continue;

        Paciente p ={0};
        copiar(p.cpf, sizeof p.cpf, c[0]);
        copiar(p.nome, sizeof p.nome, c[1]);
        copiar(p.cep, sizeof p.cep, c[2] );
        copiar(p.endereco, sizeof p.endereco, c[3]);
        copiar(p.bairro, sizeof p.bairro, c[4]);
        p.idade = atoi(c[5]);
        copiar(p.data_nasc, sizeof p.data_nasc, c[6]);
        copiar(p.enfermidade, sizeof p.enfermidade, c[7]);
        copiar(p.docente, sizeof p.docente, c[8]);
        copiar(p.area_estudo, sizeof p.area_estudo, c[9]);
        copiar(p.observacoes, sizeof p.observacoes, c[10]);
        int ok;
        *avl = avl_inserir(*avl, p, &ok);
        if (ok) *abb = abb_inserir(*abb, p.nome, p.cpf);

    }
    fclose(f);
    return 1;
}


int salvar_espera(const char *caminho, fila *fl){
    FILE *f =fopen(caminho, "w");
    if(!f) return 0;
    fprintf(f, "cpf;data_entrada\n");
    for(nofila *p = fl->inicio; p;p=p->prox)
    fprintf(f,"%s;%s\n", p->cpf, p->data_entrada);
fclose(f);
return 1;
}

int carregar_espera(const char *caminho, fila *fl, noavl *pacientes){
FILE *f = fopen(caminho, "r");
if(!f) return 1;
char linha[256], *c[2];
fgets(linha, sizeof linha, f);
while(fgets(linha,sizeof linha, f)){
    tirar_quebra(linha);
    if(dividir(linha, c,2)<2)continue;
    if(fila_enfileirar(fl,c[0], c[1])){
        Paciente *p = avl_buscar(pacientes, c[0]);
        if(p) p->em_espera =1;
    }
}
fclose(f);
return 1;
}

int salvar_consultas(const char *caminho, ListaDupla *l){
    FILE *f = fopen(caminho, "w");
    if(!f) return 0;
    fprintf(f, "id;cpf;data_consulta;hora_consulta;chave_data;observacoes\n");
    for(NoLD *p = l->inicio;p;p=p->prox){
    fprintf(f, "%d;%s;%s;%s;%lld;", p->dados.id, p->dados.cpf, p->dados.data_consulta, p->dados.hora_consulta, p->dados.chave_data);
    escrever_campo(f, p->dados.observacoes);
    fputc('\n', f);
    }
    fclose(f);
    return 1;
}   

int carregar_consultas(const char *caminho, ListaDupla *l, int*proximo_id){
    *proximo_id=1;
    FILE *f = fopen(caminho, "r");
    if(!f) return 1;
    char linha [512], *c[6];
    fgets(linha, sizeof linha, f);
    while(fgets(linha, sizeof linha, f)){
        tirar_quebra(linha);
        if(dividir(linha, c, 6) < 6) continue;
        Consulta cons = {0};
        cons.id =atoi(c[0]);
        copiar(cons.cpf, sizeof cons.cpf, c[1]);
        copiar(cons.data_consulta, sizeof cons.data_consulta, c[2]);
        copiar(cons.hora_consulta, sizeof cons.hora_consulta, c[3]);
        cons.chave_data =atoll(c[4]);
        copiar(cons.observacoes, sizeof cons.observacoes, c[5]);
        ld_inserir_ordenado(l, cons);
        if(cons.id >= *proximo_id) *proximo_id = cons.id +1;

    }
    fclose(f);
    return 1;

}
