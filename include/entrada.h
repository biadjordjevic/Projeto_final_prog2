#ifndef ENTRADA_H
#define ENTRADA_H

// le uma linha de texto com no maximo "max" caracteres;
// se o usuario digitar mais, avisa e pede de novo (nada vaza para o proximo campo)
void ler_texto(const char *prompt, char *dest, int max);

// le uma data ja formatada como dd/mm/aaaa: o usuario digita so os numeros
// e as barras aparecem sozinhas. dest precisa ter 11 posicoes.
// se obrigatorio == 0, Enter ou '-' sem digitar nada devolve 0 (manter valor atual)
int ler_data(const char *prompt, char *dest, int obrigatorio);

// le um horario ja formatado como hh:mm. dest precisa ter 6 posicoes.
int ler_hora(const char *prompt, char *dest, int obrigatorio);

#endif
