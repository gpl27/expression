#include "interpreter.h"
#include "tokenizer.h"

#include <stdio.h>
#include <stdlib.h>

static symbol *get_ptr(symbol *next);
static int get_first(symbol *expression);

double interpreter(int *order, symbol *expression)
{
    symbol *left;
    symbol *right;
    char operation;
    for (int i = 0; i < operators; i++)
    {
        operation = expression[order[i]].op;
        left = get_ptr(expression[order[i]].left);
        right = get_ptr(expression[order[i]].right);

        left->num = (operation == '*') ? left->num * right->num : (operation == '/') ? left->num / right->num :
            (operation == '+') ? left->num + right->num : left->num - right->num;

        right->fallback = left;
    }
    double result = expression[get_first(expression)].num;
    return result;
}

static symbol *get_ptr(symbol *next)
{
    if (next->fallback == NULL)
    {
        return next;
    }
    return get_ptr(next->fallback);
}

static int get_first(symbol *expression)
{
    for (int i = 0; i < symb_ctr; i++)
    {
        if (expression[i].type == 'E')
        {
            return i;
        }
    }
}

