#include <TXLib.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#pragma GCC diagnostic ignored "-Wredundant-tags"

#include "color.h"

typedef unsigned long long Stack_Elem_t;

enum code_teams
{
    PUSH    = 1,
    ADD     = 2,
    SUB     = 3,
    MULL    = 4,
    DIV     = 5,
    SQRT    = 6, 
    SIN     = 7,
    COS     = 8,
    OUTPUT  = 9,
    HLT     = 10
};

struct Stack
{
    int size;
    int capacity;
    Stack_Elem_t *data;
};

struct Spu
{
    int code[40];
    int ip;
    
};


void interpret_command (struct Stack* stack, int code[]);
void dump_spu          (struct Stack* stack, int ip, int code[], size_t size);

int stack_ctor (struct Stack *stack, int capacity);
void stack_dtor(struct Stack *stack);
int stack_push (struct Stack *stack, Stack_Elem_t value);
Stack_Elem_t stack_pop (struct Stack *stack);
int stack_is_empty (struct Stack *stack);

int main(void) 
{
    struct Stack stack = {};
    stack_ctor (&stack, 15);

    int code[40] = {};

    // FILE* file = fopen ("code.txt", "r");
    FILE* file = fopen ("code_machine.txt", "r");
    assert (file); // TODO сделать через if


    for (int i = 0; i < 40; i++)
    {
        fscanf (file, "%d", &code[i]);
    }

    fclose (file);

    // stack_push(&stack, 10);
    // stack_push(&stack, 20);
    // stack_push(&stack, 30);

    dump_spu (&stack, 0, code, 35);
    //return 0;


    // for (int i = 0; i < 40; i++)
    // {
    //     printf("code[%d] = %d\n", i, code[i]);
    // }

    interpret_command(&stack, code);

    dump_spu (&stack, 0, code, 35);


    stack_dtor(&stack); 
    return 0;
}

void interpret_command(struct Stack* stack, int code[]) // TODO найти ышяу 
{
    int ip = 0;
    int ddlx = 1;
    while(ddlx == 1)
    {

        dump_spu (stack, ip, code, 35); 

        switch (code[ip]) 
        {
            case PUSH: // push
                {
                    printf(">>> ip = %d, code[id] = %d, code[ip+1] = %d: I am going to push\n", ip, code[ip], code[ip + 1]);
                    stack_push (stack, code[ip + 1]);
                    printf("<<< stack->data[stack->size - 1] = %llu, size = %d\n\n", stack->data[stack->size - 1], stack->size);
                    ip += 2;
                }
                break;

            case ADD: // add
                {
                    Stack_Elem_t a = stack_pop(stack);
                    Stack_Elem_t b = stack_pop(stack);

                    printf(">>> ip = %d, code[id] = %d: I'm going to add: a = %llu | b = %llu\n", ip, code[ip], a, b);

                    stack_push(stack, a + b);

                    printf("<<< stack->data[stack->size - 1] = %llu, size = %d, a + b = %llu + %llu = %llu\n\n", stack->data[stack->size - 1], stack->size, a, b, a + b);

                    ip += 1;
                    printf("    NEXT: ip = %d\n", ip);
                    // printf("    add // push(%llu + %llu)\n", b, a);
                }
                break;

                case SUB: // sub
                {
                    Stack_Elem_t a = stack_pop(stack);
                    Stack_Elem_t b = stack_pop(stack);

                    printf(">>> ip = %d, code[id] = %d: I'm going to sub: a = %llu | b = %llu\n", ip, code[ip], a, b);

                    stack_push(stack, b - a);

                    printf("<<< stack->data[stack->size - 1] = %llu, size = %d, a - b = %llu - %llu = %llu\n\n", stack->data[stack->size - 1], stack->size, a, b, a - b);

                    ip +=1;
                    printf("ip = %d", ip);
                    printf("sub // push(%llu - %llu)\n", b, a);
                }
                break;

            case MULL: // mull
            {
                Stack_Elem_t a = stack_pop(stack);
                Stack_Elem_t b = stack_pop(stack);

                stack_push(stack, a * b);

                ip += 1;
            }
            break;

            case DIV: // div
            {
                Stack_Elem_t a = stack_pop(stack);
                Stack_Elem_t b = stack_pop(stack);

                if (a == 0)
                {
                    printf ("ERROR: Division by zero\n");
                    ddlx = 0;
                }

                else
                    stack_push (stack, b / a);

                ip +=1;
            }
            break;

            case SQRT: // sqrt
            {
                Stack_Elem_t a = stack_pop (stack);
                // if (a < 0) // [x] a - всегда беззнаковый
                // {
                //     printf ("ERROR: Square root of negative number\n");
                //     ddlx = 0;
                // }

                //else
                stack_push (stack, (Stack_Elem_t)sqrt(a));

                ip +=1;
            }
            break;

            case SIN: // sin
            {
                Stack_Elem_t a = stack_pop (stack);
                stack_push (stack, (Stack_Elem_t)sin(a));
                ip += 1;
            }
            break;

            case COS: // cos
            {
                Stack_Elem_t a = stack_pop (stack);
                stack_push (stack, (Stack_Elem_t)cos(a));
                ip +=1;
            }
            break;

            case OUTPUT: // output
                {
                    Stack_Elem_t result = stack_pop(stack);
                    printf("out %llu\n", result);
                    printf("%lg\n", (double)result);
                    ip += 1;
                }
                break;

            case HLT: // halt
                {
                    printf("hlt\n");
                    ddlx = 0;
                }
                break;

            default:
                printf("ERROR: Invalid command %d\n", code[ip]);
                ddlx = 0;
                break;
        }
    }    
}

void dump_spu (struct Stack* stack, int ip, int code[], size_t size)
{
    printf(RED"\n----------------------------------------------------------------------------------------------------------\n");

    printf(BLUE "code:");
    for (size_t i = 0; i < size; i++)
        printf(" %02llu", i);
    
    printf("\n     ");

    for (size_t i = 0; i < size; i++)
        printf(YELLOW " %02d", code[i]);

    printf("\n");

    for (int i = 0; i < ip; i++)
        printf("   ");

    printf(MAGENTA "      ^ ip = %d\n\n", ip);

    printf("stack: ");
    for (size_t i = 0; i < stack->size + 1; i++) // int i 
        printf(CYAN "[%llu]=" WHITE "%llu ", i, stack->data[i]);


    printf(RED "\n----------------------------------------------------------------------------------------------------------" RESET);

    getchar();
}

int stack_ctor(struct Stack *stack, int capacity) 
{
    stack->data = (Stack_Elem_t *) calloc (capacity, sizeof(Stack_Elem_t));
    if (!stack->data)
        return -1;

    stack->capacity = capacity;
    stack->size     = 0;

    return 0;
}

void stack_dtor(struct Stack *stack) 
{
    free(stack->data);
    stack->data     = NULL;
    stack->size     = 0;
    stack->capacity = 0;
}

int stack_push(struct Stack *stack, Stack_Elem_t value) 
{
    if (stack->size >= stack->capacity) 
        return -1; 
    
    stack->data[stack->size++] = value;
    return 0;
}

Stack_Elem_t stack_pop(struct Stack *stack) 
{
    if (stack_is_empty(stack)) 
    {
        return 0;  
    }
    return stack->data[--stack->size];
}

int stack_is_empty(struct Stack *stack) 
{
    return stack->size == 0;
}
