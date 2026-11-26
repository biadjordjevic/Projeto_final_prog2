#ifndef FILA_H
#define FILA_H
// determina oq que vai ter em cada nó da fila, e, como a fila vai se comportar
typedef struct nofila {
    char cpf[12];
    char data_entrada[11];
    struct nofila *prox;
} nofila;

typedef struct {
    nofila *inicio, *fim;
    int tamanho;
} fila;

void fila_inicializar(fila *f);
int fila_enfileirar(fila *f, const char *cpf, const char *data_entrada);
int fila_desenfileirar(fila *f, char *cpf_out);
int fila_remover_cpf(fila *f, const char *cpf);
nofila* fila_buscar(fila *f, const char *cpf);
void fila_liberar(fila *f);


#endif

// possui ligação com avl para obter o cpf, único dado do paciente que guarda 