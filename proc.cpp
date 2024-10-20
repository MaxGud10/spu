#include <TXLib.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#pragma GCC diagnostic ignored "-Wredundant-tags"

#include "color.h"

typedef unsigned long long Stack_Elem_t;

enum Commands // TODO сделать верификатор и палачей 
{
    PUSH    = 1,
    POP     = 2,
    ADD     = 3,
    SUB     = 4,
    MULL    = 5,
    DIV     = 6,
    SQRT    = 7, 
    SIN     = 8,
    COS     = 9,
    OUTPUT  = 10,
    HLT     = 11,
    PUSHR   = 12,
    JB      = 13, // JUMP
    NOB     = 14,
    JMP     = 15
};

enum Registers
{
    REG_STEP = 10,
    REG_BASE = 70,
    AX       = 70,
    BX       = 80,
    CX       = 90,
    DX       = 100
};

struct Stack // TODO добавить уже написанный стек. Подключать хедер
{
    int size;
    int capacity;
    Stack_Elem_t *data; 
};

struct Spu // [x] - не лучше ли написать CPU
{
    int* code; // [40]
    int ip;
    int* registers;
};


void interpret_command (struct Stack* stack, struct Spu* processor);
void dump_spu          (struct Stack* stack, struct Spu* processor, size_t size);
//void dump_spu          (struct Stack* stack, int ip, int code[], size_t size);

int filling_the_machine_code (struct Spu* processor);

int stack_ctor (struct Stack *stack, int capacity);
void stack_dtor(struct Stack *stack);
int stack_push (struct Stack *stack, Stack_Elem_t value);
Stack_Elem_t stack_pop (struct Stack *stack);
int stack_is_empty (struct Stack *stack);

int main(void) 
{
    struct Stack stack = {};
    
    struct Spu processor = {};

    processor.ip = 0;

    processor.code = (int*)calloc (40, sizeof(int)); // TODO: заказывать размер файла

    filling_the_machine_code (&processor);

    processor.registers = (int*) calloc (8, sizeof(int));

    stack_ctor (&stack, 15); // TODO: числа голыми быть не должны

    dump_spu (&stack, &processor, 35);
    //return 0;


    // for (int i = 0; i < 40; i++)
    // {
    //     printf("code[%d] = %d\n", i, code[i]);
    // }

    interpret_command(&stack, &processor);

    dump_spu (&stack, &processor, 35);


    stack_dtor(&stack); 
    return 0;
}

void interpret_command (struct Stack* stack, struct Spu* processor) // TODO найти size 
{
    int ip = 0;
    int ddlx = 1;
    while(ddlx == 1)
    {

        dump_spu (stack, processor, 35); 

        switch (processor->code[processor->ip]) 
        {
            case PUSH: // push
                {
                    printf(">>> ip = %d, code[id] = %d, code[ip+1] = %d: I am going to push\n", ip, processor->code[processor->ip], processor->code[processor->ip + 1]);
                    stack_push (stack, processor->code[processor->ip + 1]); // [x] - сейчас я могу изменить внутреннюю функцию push а что делать, когда я буду подключать свой стек. изменять там push или как ?
                    printf("<<< stack->data[stack->size - 1] = %llu, size = %d\n\n", stack->data[stack->size - 1], stack->size);
                    processor->ip += 2;
                }
                break;

            case POP:
                {
                    Stack_Elem_t a = stack_pop(stack);

                    
                    // [0]
                    int reg_num = (processor->code[processor->ip + 1] - REG_BASE) / REG_STEP;
                    processor->registers[reg_num]= (int) a; // брать номер регистра из машинного кода - 70 и делить на 10

                    processor->ip += 2;
                }
                break;

            case ADD: // add
                {
                    Stack_Elem_t a = stack_pop(stack);
                    Stack_Elem_t b = stack_pop(stack);

                    printf(">>> ip = %d, code[id] = %d: I'm going to add: a = %llu | b = %llu\n", ip, processor->code[processor->ip], a, b);

                    stack_push(stack, a + b);

                    printf("<<< stack->data[stack->size - 1] = %llu, size = %d, a + b = %llu + %llu = %llu\n\n", stack->data[stack->size - 1], stack->size, a, b, a + b);

                    processor->ip += 1;
                    printf("    NEXT: ip = %d\n", ip);
                    // printf("    add // push(%llu + %llu)\n", b, a);
                }
                break;

                case SUB: // sub
                {
                    Stack_Elem_t a = stack_pop(stack);
                    Stack_Elem_t b = stack_pop(stack);

                    printf(">>> ip = %d, code[id] = %d: I'm going to sub: a = %llu | b = %llu\n", ip, processor->code[processor->ip], a, b);

                    stack_push(stack, b - a);

                    printf("<<< stack->data[stack->size - 1] = %llu, size = %d, a - b = %llu - %llu = %llu\n\n", stack->data[stack->size - 1], stack->size, a, b, a - b);

                    processor->ip +=1;
                    printf("ip = %d", ip);
                    printf("sub // push(%llu - %llu)\n", b, a);
                }
                break;

            case MULL: // mull
            {
                Stack_Elem_t a = stack_pop(stack);
                Stack_Elem_t b = stack_pop(stack);

                stack_push(stack, a * b);

                processor->ip += 1;
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

                processor->ip +=1;
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

                processor->ip +=1;
            }
            break;

            case SIN: // sin
            {
                Stack_Elem_t a = stack_pop (stack);
                stack_push (stack, (Stack_Elem_t)sin(a));
                processor->ip += 1;
            }
            break;

            case COS: // cos
            {
                Stack_Elem_t a = stack_pop (stack);
                stack_push (stack, (Stack_Elem_t)cos(a));
                processor->ip +=1;
            }
            break;

            case OUTPUT: // output
                {
                    $(stack->size);
                    $(stack->data[0]);
                    
                    Stack_Elem_t result = stack_pop(stack);
                    printf("out %llu\n", result);
                    printf("%lg\n", (double)result);
                    $(result);
                    processor->ip += 1;
                }
                break;

            case HLT: // halt
                {
                    printf("hlt\n");
                    ddlx = 0;
                }
                break;

            case JB: // [x] - правильно ли ?
                {                                                                                                                                               for (int i = 0; i < stack->size; i++) printf ("piss ");
                    printf("size= %d\n", stack->size);
                    Stack_Elem_t a = stack_pop(stack);
                    Stack_Elem_t b = stack_pop(stack);

                    printf("a = %llu\n", a);
                    printf("b = %llu\n", b);

                    if (a < b) // > 
                        processor->ip = processor->code[processor->ip + 1];
                    
                    else
                        processor->ip += 2;
                }
                break;
            
            case PUSHR:
                {
                    int reg_num = (processor->code[processor->ip + 1] - REG_BASE) / REG_STEP;
                    stack_push(stack, processor->registers[reg_num]);

                    processor->ip += 2;
                }
                break;

            case JMP:
                {
                    // Stack_Elem_t a = stack_pop(stack);
                    // Stack_Elem_t b = stack_pop(stack); 

                    processor->ip = processor->code[processor->ip + 1];
                }
                break;

            default:
                printf("ERROR: Invalid command %d\n", processor->code[processor->ip]);
                ddlx = 0;
                break;
        }                                                                                              
    } 
}

int filling_the_machine_code (struct Spu* processor)
{
    FILE* file = fopen ("code_machine.txt", "r");
    assert (file); // TODO сделать через if


    for (int i = 0; i < 40; i++)
        fscanf (file, "%d", &processor->code[i]);

    fclose (file);

    return 0;
}

void dump_spu (struct Stack* stack, struct Spu* processor, size_t size)
{
    printf(RED"\n----------------------------------------------------------------------------------------------------------\n");

    printf(BLUE "code:");
    for (size_t i = 0; i < size; i++)
        printf(" %02llu", i);
    
    printf("\n     ");

    for (size_t i = 0; i < size; i++)
        printf(YELLOW " %02d", processor->code[i]);

    printf("\n");

    for (int i = 0; i < processor->ip; i++)
        printf("   ");

    printf(MAGENTA "      ^ ip = %d\n\n", processor->ip);

    printf("registers: ");
    for (size_t i = 0; i < 8; i++) // 4
        printf(" %02d", processor->registers[i]);

    printf("\n");

    printf("stack (size %d): ", stack->size);
    for (int i = 0; i < stack->size; i++) // int i 
        printf(CYAN "[%d]=" WHITE "%llu ", i, stack->data[i]);


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
