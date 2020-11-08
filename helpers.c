#include "helpers.h"
#include "tokenizer.h"

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <limits.h>

// ---------------- Helper Functions ------------------//

void set_element(symbol *symb, int num)
{
    symb->type = 'E';
    symb->num = (double) num;
    symb->fallback = NULL;
    return;
}

void set_operator(symbol *symb, char op)
{
    symb->type = 'O';
    symb->op = op;
    symb->left = NULL;
    symb->right = NULL;
    return;
}

void set_right_left(symbol *symb, symbol *left, symbol *right)
{
    symb->left = left;
    symb->right = right;
    return;
}


// ------------ Local Functions -------------//

int get_digits(char *finput, int *index)
{
    errno = 0;
    char *digits = malloc(sizeof(char) * 1024);
    if (digits == NULL)
    {
        printf("Error: malloc() failed\n");
        errno = -1;
        return errno;
    }

    // Check for starting '-' or '+'
    int i = (issign(finput[*index])) ? 1 : 0;
    if (issign(finput[*index]))
    {
        digits[0] = finput[*index];
        *index += 1;
    }

    while (isdigit(finput[*index]))
    {
        digits[i] = finput[*index];
        *index += 1;
        i++;
    }

    digits[i] = '\0';
    errno = 0;
    char *endptr;
    long number = strtol(digits, &endptr, 10);

    if (errno == ERANGE)
    {
        printf("Error: overflow/underflow\n");
        errno = -1;
        free(digits);
        return errno;
    }

    if (endptr == digits)
    {
        printf("Syntax Error: expected type 'E'\n");
        errno = -1;
        free(digits);
        return errno;
    }

    if (*endptr != '\0')
    {
        printf("Error: parsing digits failed: mixed input\n");
        errno = -1;
        free(digits);
        return errno;
    }

    if (number >= INT_MAX || number <= INT_MIN)
    {
        printf("Error: overflow/underflow\n");
        errno = -1;
        free(digits);
        return errno;
    }

    free(digits);
    return number;
}

int get_estimate(char *finput)
{
    int paren = 0;
    int estimate = 0;
    int i = 0;
    while (finput[i] != '\0')
    {
        if (isoperator(finput[i]))
        {
            estimate++;
        }
        if (finput[i] == ')' || finput[i] == '(')
        {
            paren++;
        }
        i++;
    }
    estimate = (estimate == 0) ? estimate : (estimate * 2) + 1;
    estimate += paren + 1;
    return estimate;
}

int isoperator(char character)
{
    if (character == '+' || character == '-' || character == '*' || character == '/')
    {
        return 1;
    }
    return 0;
}

int issign(char character)
{
    if (character == '+' || character == '-')
    {
        return 1;
    }
    return 0;
}

// debugging

void print_symbols(symbol *expression)
{
    for (int i = 0; i < symb_ctr; i++)
    {
        if (expression[i].type == 'E')
        {
            printf("[%i]   type = E   value = %f\n", i, expression[i].num);
            continue;
        }
        else if (expression[i].type == 'O')
        {
            printf("[%i]   type = O   operator = %c   weight = %i\n", i, expression[i].op, expression[i].weight);
            continue;
        }
        printf("[%i]   type = P   operator = %c\n", i, expression[i].op);
    }
    return;
}
