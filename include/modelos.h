#ifndef MODELOS_H
#define MODELOS_H
// vamos guardar as definições dos dados do paciente

typedef struct {
    char nome[100];
    char cpf[12];
    char cep[10];
    char endereco[150];
    char bairro[60];
    int idade;
    char data_nasc[11];
    char enfermidade[100];
    char docente[100];
    char area_estudo[60];
    char data_consulta[11];
    char observacoes[200];
    int em_espera;
}Paciente;