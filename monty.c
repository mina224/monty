#include <stdio.h>
#include <stdlib.h>
#include "monty.h"

stack_t *stack = NULL;  // Global stack

/* Function Definitions */

void push(stack_t **stack, unsigned int line_number, int value)
{
    stack_t *new_node = malloc(sizeof(stack_t));
    if (new_node == NULL)
    {
        fprintf(stderr, "Error: malloc failed\n");
        exit(EXIT_FAILURE);
    }

    new_node->n = value;
    new_node->prev = NULL;
    new_node->next = *stack;

    if (*stack != NULL)
        (*stack)->prev = new_node;

    *stack = new_node;
}

void pall(stack_t **stack, unsigned int line_number)
{
    stack_t *current = *stack;

    while (current != NULL)
    {
        printf("%d\n", current->n);
        current = current->next;
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "USAGE: monty file\n");
        return EXIT_FAILURE;
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL)
    {
        fprintf(stderr, "Error: Can't open file %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    char *line = NULL;
    size_t line_len = 0;
    unsigned int line_number = 0;

    while (getline(&line, &line_len, file) != -1)
    {
        line_number++;

        // Split the line into opcode and argument (if applicable)
        char *opcode = strtok(line, " \t\n");
        char *arg = strtok(NULL, " \t\n");

        // Find the corresponding function for the opcode
        instruction_t *instruction = NULL;
        for (int i = 0; i < INSTRUCTION_COUNT; i++)
        {
            if (strcmp(opcode, instructions[i].opcode) == 0)
            {
                instruction = &instructions[i];
                break;
            }
        }

        // Execute the instruction if found, otherwise print error and exit
        if (instruction != NULL)
        {
            if (arg != NULL)
            {
                int value = atoi(arg);
                instruction->f(&stack, line_number, value);
            }
            else
            {
                fprintf(stderr, "L%u: usage: %s integer\n", line_number, opcode);
                fclose(file);
                free(line);
                return EXIT_FAILURE;
            }
        }
        else
        {
            fprintf(stderr, "L%u: unknown instruction %s\n", line_number, opcode);
            fclose(file);
            free(line);
            return EXIT_FAILURE;
        }
    }

    fclose(file);
    free(line);
    return EXIT_SUCCESS;
}
