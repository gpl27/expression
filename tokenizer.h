#ifndef TOKENIZER_H
#define TOKENIZER_H

//typedef enum operators {MULT = 1, DIV = 1, ADD = 2, SUB = 2};

typedef struct symbol
{
    char type;  // 'E'lement or 'O'perator or 'P'arenthesis
    // Only used for elements
    double num;                // Value 
    struct symbol *fallback;   // Where to go after used
    // Only used for operators
    char op;                // Operator type: +, -, *, /,
    int weight;             // Operator weight for Order of Operations
    struct symbol *left;    // Symbol to the left
    struct symbol *right;   // Symbol to the right
    
    int inside;
}
symbol;

symbol *tokenizer(char *finput);

symbol *syntax(symbol *expression);

void assignment(symbol *expression);

int *get_operators(symbol *expression);

extern int symb_ctr;

extern int operators;

#endif



/* Future struct attributes
    int index;  // Index in expression
    char side;  // Side of equation 'r'ight or 'l'eft
    //char var;                  // Variable of element
    //int power;                 // Power of element
*/