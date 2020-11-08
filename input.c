#include "input.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *get_input(void)
{
    char *input = malloc(sizeof(char) * 1024);
    if (input == NULL)
    {
        printf("Error: malloc() failed\n");
        return NULL;
    }
    memset(input, '\0', 1024);
    printf("::: ");
    if (fgets(input, 1024, stdin) == NULL)
    {
        printf("Error: no input was read\n");
        free(input);
        return NULL;
    }
    return input;
}

char *format_input(char *input)
{
    char *finput = malloc(sizeof(char) * 1024);
    if (finput == NULL)
    {
        printf("Error: malloc() failed\n");
        return NULL;
    }
    memset(finput, '\0', 1024);
    int i = 0;
    int j = 0;
    while (input[i] != '\0')
    {
        if (input[i] == ' ' || input[i] == '\n')
        {
            i++;
            continue;
        }
        finput[j] = input[i];
        i++;
        j++;
    }
    finput[j] = '\0';
    return finput;
}