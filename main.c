#include "input.h"
#include "tokenizer.h"
#include "interpreter.h"

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>


// Would be better to malloc once and just reuse the same block of
// memory, but that means changing some stuff, so i'm not interested
// in doing that right now


// NO SUPPORT
// ^    POWER

int main(void)
{
    char *input;
    char *finput;
    symbol *expression;
    symbol *synt;
    int *order;
    double solution;
    while (1)
    {
        // Get input
        input = get_input();
        if (input == NULL)
        {
            return 1;
        }
        else if (!strcasecmp(input, "exit\n"))
        {
            free(input);
            puts("Exiting...");
            return 0;
        }

        // Format input
        finput = format_input(input);
        if (finput == NULL)
        {
            free(input);
            return 1;
        }

        // Tokenize
        expression = tokenizer(finput);
        if (expression == NULL)
        {
            free(input);
            free(finput);
            return 1;
        }

        // Check syntax
        synt = syntax(expression);
        if (synt == NULL)
        {
            free(input);
            free(finput);
            free(expression);
            return 1;
        }

        // Assign importance and pointers
        assignment(expression);

        // Order operations
        order = get_operators(expression);
        if (order == NULL)
        {
            free(input);
            free(finput);
            free(expression);
            return 1;
        }

        // Solve expression
        solution = interpreter(order, expression);

        // Prints solution
        printf(": %f\n", solution);

        // Frees memory
        free(input);
        free(finput);
        free(expression);
        free(order);
    }

    return 0;
}