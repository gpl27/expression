#include "tokenizer.h"
#include "helpers.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>

// Globals
// Total number of symbols in expression
int symb_ctr = 0;
// Total number of operators in expression
int operators = 0;

// Local Flags
static bool read_element = true;
static bool read_operator = false;



symbol *tokenizer(char *finput)
{
    // Defaults the flags
    symb_ctr = 0;
    operators = 0;
    read_element = true;
    read_operator = false;

    // Creates symbol array based on estimated number of symbols present
    int estimate = get_estimate(finput);
    symbol *expression = (symbol *) malloc(sizeof(symbol) * estimate);
    if (expression == NULL)
    {
        printf("Error: 0 symbols or malloc() failed\n");
        return NULL;
    }
    memset(expression, 0, (sizeof(symbol) * estimate));
    // Counter for symbols initialised globally

    // Interprets expression (i.e. finput)
    int i = 0;
    while (finput[i] != '\0')
    {
        if (finput[i] == '(' || finput[i] == ')')
        {
            expression[symb_ctr].type = 'P';
            expression[symb_ctr].op = finput[i];
            symb_ctr++;
            i++;
        }
        else if (read_element == true)
        {
            int number = get_digits(finput, &i);
            if (errno == -1 && number == -1)
            {
                free(expression);
                return NULL;
            }
            set_element(&expression[symb_ctr], number);
            symb_ctr++;
            read_element = false;
            read_operator = true;
        }
        else if (read_operator == true)
        {
            if (!isoperator(finput[i]))
            {
                free(expression);
                printf("Syntax Error: operator error\n");
                return NULL;
            }
            set_operator(&expression[symb_ctr], finput[i]);
            symb_ctr++;
            operators++;
            read_operator = false;
            read_element = true;
            i++;
        }
    }
    return expression;
}

symbol *syntax(symbol *expression)
{
    // Check if ends with an operator
    if (expression[symb_ctr - 1].type == 'O')
    {
        printf("Syntax Error: expected type 'E'/'P'\n");
        return NULL;
    }
    // Count open and close paren
    int open = 0;
    int close = 0;
    for (int i = 0; i < symb_ctr; i++)
    {
        if (expression[i].type == 'P')
        {
            open = (expression[i].op == '(') ? open + 1 : open;
            close = (expression[i].op == ')') ? close + 1 : close;
        }
    }
    if (open != close)
    {
        printf("Syntax Error: incorrect parentheses\n");
        return NULL;
    }
    // Correct parentheses use
    for (int i = 0; i < symb_ctr; i++)
    {
        if (expression[i].type == 'O')
        {
            // Check for preceding or succeeding operator
            if (expression[i].op == '(' && i != 0)
            {
                if (expression[i - 1].type != 'O' || expression[i - 1].type != 'P')
                {
                    printf("Syntax Error: expected type 'O'/'P'\n");
                    return NULL;
                }
            }
            if (expression[i].op == ')' && i != (symb_ctr - 1))
            {
                if (expression[i + 1].type != 'O' || expression[i + 1].type != 'P')
                {
                    printf("Syntax Error: expected type 'O'/'P'\n");
                    return NULL;
                }
            }
        }
    }

    // Might have to check for empty parentheses

    return expression;
}

static symbol *get_ptr(symbol *expression, int index, char way);

void assignment(symbol *expression)
{
    // Point pointers
    for (int i = 0; i < symb_ctr; i++)
    {
        if (expression[i].type == 'O')
        {
            expression[i].left = get_ptr(expression, i, 'l');
            expression[i].right = get_ptr(expression, i, 'r');
        }
    }
    // Determine operator weight
    int paren_mult = 1;
    char op;
    for (int i = 0; i < symb_ctr; i++)
    {
        if (expression[i].type == 'P')
        {
            paren_mult = (expression[i].op == '(') ? paren_mult * 4 : paren_mult / 4;
        }
        if (expression[i].type == 'O')
        {
            op = expression[i].op;
            expression[i].weight = (op == '+' || op == '-') ? 2 * paren_mult : 4 * paren_mult;
        }
    }
    return;
}

static symbol *get_ptr(symbol *expression, int index, char way)
{
    do
    {
        index = (way == 'l') ? index - 1 : index + 1;
    }
    while (expression[index].type != 'E');

    return &expression[index];
}


int *get_operators(symbol *expression)
{
    // Allocates memory for indices of operators
    int *operator_arr = malloc(sizeof(int) * operators);
    if (operator_arr == NULL)
    {
        printf("Error: malloc() failed \n");
        return NULL;
    }

    // Get indices to array
    int ind = 0;
    for (int i = 0; i < symb_ctr; i++)
    {
        if (expression[i].type == 'O')
        {
            operator_arr[ind] = i;
            ind++;
        }
    }

    // Sort array using bubble sort  (TEST USING ONLY ONE ELEMENT)
    int swap = 1;
    int tmp;
    while (swap)
    {
        swap = 0;
        for (int j = 1; j < operators; j++)
        {
            if (expression[operator_arr[j - 1]].weight < expression[operator_arr[j]].weight)
            {
                tmp = operator_arr[j];
                operator_arr[j] = operator_arr[j - 1];
                operator_arr[j - 1] = tmp;
                swap = 1;
            }
        }
    }
    return operator_arr;
}
