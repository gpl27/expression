#ifndef HELPERS_H
#define HELPERS_H

#include "tokenizer.h"

// The only purpose of this library is to clean tokenizer.h
// These are all the secondary functions it needs

void set_element(symbol *symb, int num);

void set_operator(symbol *symb, char op);

void set_right_left(symbol *symb, symbol *left, symbol *right);


// debugging
void print_symbols(symbol *expression);


int get_digits(char *finput, int *index);

int get_estimate(char *finput);

int isoperator(char character);

int issign(char character);

#endif