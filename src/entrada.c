#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "entrada.h"

#ifdef _WIN32
#include <conio.h>
#include <io.h>
#define EH_CONSOLE() _isatty(_fileno(stdin))
#else
#define EH_CONSOLE() 0
#endif

static void descartar_linha(void){
    int c;
    while((c = getchar()) != '\n' && c != EOF);
}

void ler_texto(const char *prompt, char *dest, int max){
    char fmt[20];
    sprintf(fmt, " %%%d[^\n]", max);
    while(1){
        printf("%s", prompt);
        if(scanf(fmt, dest) != 1){ dest[0] = '\0'; return; }
        int c = getchar();
        if(c == '\n' || c == EOF) return;   // coube no limite
        descartar_linha();                  // passou do limite: joga fora o resto
        printf("Maximo de %d caracteres. Digite novamente.\n", max);
    }
}

// le digitos encaixando-os na mascara (ex.: "__/__/____").
// devolve 1 se preencheu tudo, 0 se o usuario deixou vazio (so quando nao obrigatorio)
static int ler_mascara(const char *prompt, const char *mascara, char *dest, int obrigatorio){
    int len = (int)strlen(mascara);
#ifdef _WIN32
    if(EH_CONSOLE()){
        int n = 0;
        printf("%s", prompt); fflush(stdout);
        while(1){
            int c = _getch();
            if(c == 0 || c == 0xE0){ _getch(); continue; }  // setas, F1... ignora
            if(c == 3) exit(0);                                 // Ctrl+C
            if(c == '\r'){
                if(n == len) break;
                if(n == 0 && !obrigatorio) break;
                continue;                                       // incompleto: espera mais digitos
            }
            if(c == '-' && n == 0 && !obrigatorio){ putchar('-'); break; }
            if(c == '\b'){
                // apaga separadores automaticos e depois um digito
                while(n > 0 && mascara[n-1] != '_'){ n--; printf("\b \b"); }
                if(n > 0){ n--; printf("\b \b"); }
                fflush(stdout);
                continue;
            }
            if(c >= '0' && c <= '9' && n < len){
                dest[n++] = (char)c; putchar(c);
                // coloca a barra / dois-pontos sozinho
                while(n < len && mascara[n] != '_'){ dest[n] = mascara[n]; putchar(dest[n]); n++; }
                fflush(stdout);
            }
        }
        dest[n] = '\0';
        putchar('\n');
        return n > 0;
    }
#endif
    // entrada redirecionada (ou fora do Windows): le a linha e formata os digitos
    int qtd_digitos = 0;
    for(int i = 0; i < len; i++) if(mascara[i] == '_') qtd_digitos++;
    while(1){
        char linha[64], dig[16];
        int nd = 0;
        ler_texto(prompt, linha, 63);
        if(!obrigatorio && (linha[0] == '\0' || strcmp(linha, "-") == 0)) return 0;
        for(char *p = linha; *p && nd < 15; p++) if(*p >= '0' && *p <= '9') dig[nd++] = *p;
        if(nd == qtd_digitos){
            int k = 0;
            for(int i = 0; i < len; i++) dest[i] = (mascara[i] == '_') ? dig[k++] : mascara[i];
            dest[len] = '\0';
            return 1;
        }
        printf("Digite %d numeros.\n", qtd_digitos);
    }
}

static int data_valida(int d, int m, int a){
    int dias[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    if(a < 1900 || a > 2100 || m < 1 || m > 12) return 0;
    if((a % 4 == 0 && a % 100 != 0) || a % 400 == 0) dias[1] = 29;
    return d >= 1 && d <= dias[m-1];
}

int ler_data(const char *prompt, char *dest, int obrigatorio){
    int d, m, a;
    while(1){
        if(!ler_mascara(prompt, "__/__/____", dest, obrigatorio)) return 0;
        if(sscanf(dest, "%d/%d/%d", &d, &m, &a) == 3 && data_valida(d, m, a)) return 1;
        printf("Data invalida. Digite novamente.\n");
    }
}

int ler_hora(const char *prompt, char *dest, int obrigatorio){
    int h, mi;
    while(1){
        if(!ler_mascara(prompt, "__:__", dest, obrigatorio)) return 0;
        if(sscanf(dest, "%d:%d", &h, &mi) == 2 && h >= 0 && h <= 23 && mi >= 0 && mi <= 59) return 1;
        printf("Hora invalida. Digite novamente.\n");
    }
}
