//#define DEBUG

#include <TXLib.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#ifdef DEBUG
    #define DBG  if (1)
#else
    #define DBG  if (0) 
#endif

#pragma GCC diagnostic ignored "-Wredundant-tags"

#include "color.h"

typedef int Stack_Elem_t; // typedef unsigned long long Stack_Elem_t;
const int DEL_LINES = 10;


enum Commands // TODO сделать верификатор и палачей 
{
    PUSH          = 1,
    POP           = 2,
    ADD           = 3,
    SUB           = 4,
    MULL          = 5,
    DIV           = 6,
    SQRT          = 7, 
    SIN           = 8,
    COS           = 9,
    OUTPUT        = 10,
    HLT           = 11,
    PUSHR         = 12,
    JB            = 13, // JUMP
    NOB           = 14,
    JMP           = 15,
    DED_SMESHARIK = 16,
    JA            = 17,
    CALL          = 18,
    RET           = 19,
    CMD_IN        = 20,
    JNE           = 21,
    JE            = 22,
    JBE           = 23,
    JAE           = 24
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

struct Spu 
{
    int* code; // [40]
    int ip;
    int* registers;
    int* ram;
    struct Stack stack; 
    struct Stack ret_addr_stk;
};


void interpret_command (struct Spu* processor) ;
void dump_spu (struct Spu* processor, size_t size);
//void dump_spu          (struct Stack* stack, int ip, int code[], size_t size);

int filling_the_machine_code (struct Spu* processor);

int* get_arg (struct Spu* processor);

int stack_ctor  (struct Stack *stack, int capacity);
void stack_dtor (struct Stack *stack);
int stack_push  (struct Stack *stack, Stack_Elem_t value);
Stack_Elem_t stack_pop (struct Stack *stack);
int stack_is_empty (struct Stack *stack);

int main(void) 
{
    // struct Stack stack        = {};

    // struct Stack ret_addr_stk = {};
    
    struct Spu processor      = {};

    processor.ip = 0;

    processor.code = (int*) calloc (1000, sizeof(int)); // TODO: заказывать размер файла

    filling_the_machine_code (&processor);

    processor.registers = (int*) calloc (300, sizeof(int));
    // processor.registers[1] = 10; // для примера 
    // processor.registers[2] = 20;
    // processor.registers[3] = 30;
    // processor.registers[4] = 40;


    stack_ctor (&processor.stack, 300); // TODO: числа голыми быть не должны
    stack_ctor (&processor.ret_addr_stk, 300);

    dump_spu (&processor, 200);

    $(processor.ram) = (int*) calloc (DEL_LINES * DEL_LINES, sizeof(int)); 

    interpret_command(&processor);
// 

    dump_spu (&processor, 200);


    stack_dtor(&processor.stack); 
    return 0;
}

void interpret_command (struct Spu* processor) 
{
    int ip   = 0;
    int ddlx = 1;
    while(ddlx == 1)
    {
        //dump_spu (processor, 50); 

        switch (processor->code[processor->ip]) 
        {
            case PUSH: // push
                {
                    DBG printf(">>> ip = %d, code[id] = %d, code[ip+1] = %d: I am going to push\n", ip, processor->code[processor->ip], processor->code[processor->ip + 1]);
                    //int ded_loh = get_arg_pop_new (processor);
                    stack_push (&processor->stack, *get_arg (processor)); // processor->code[processor->ip + 1]
                    DBG printf("<<< stack->data[stack->size - 1] = %llu, size = %d\n\n", processor->stack.data[processor->stack.size - 1], processor->stack.size);
                    //processor->ip += 2;
                }
                break; 

            case POP:
                {
                    DBG printf("POP is done and   PROCESSOR->CODE[PROCESSOR->IP] = %d\n",  processor->code[processor->ip]);
                    Stack_Elem_t a = stack_pop(&processor->stack);

                    // [0]
                    //int reg_num = (processor->code[processor->ip + 2] - REG_BASE) / REG_STEP;
                    int* addr = get_arg (processor);
                    DBG printf("maybe a prog is falling here: addr = %p \n", addr);
                    //int lower_bound_register_array = sizeof(processor->registers) / sizeof (processor->registers[0]); // [x] как сделать правую и левую границу массива регистров  
                    *addr = (int) a;
                    DBG printf("a = %llu\n", a); // 

                    // printf("<<< processor->registers[processor->code[processor->ip]] = %d\n", processor->registers[processor->code[processor->ip]]);
                    // processor->registers[processor->code[processor->ip]] = (int) a;
                    // printf(">>> processor->registers[processor->code[processor->ip]] = %d\n", processor->registers[processor->code[processor->ip]]);
                    
                    //processor->registers[]= (int) a; 
                    //processor->ip += 1;
                }
                break;

            case ADD: // add
                {
                    Stack_Elem_t a = stack_pop(&processor->stack);
                    Stack_Elem_t b = stack_pop(&processor->stack);

                    DBG printf(">>> ip = %d, code[id] = %d: I'm going to add: a = %llu | b = %llu\n", ip, processor->code[processor->ip], a, b);

                    stack_push(&processor->stack, a + b);

                    processor->ip += 1;
                    DBG printf("    NEXT: ip = %d\n", ip);
                    // printf("    add // push(%llu + %llu)\n", b, a);
                }
                break;

                case SUB: // sub
                {
                    Stack_Elem_t a = stack_pop(&processor->stack);
                    Stack_Elem_t b = stack_pop(&processor->stack);

                    DBG printf(">>> ip = %d, code[id] = %d: I'm going to sub: a = %llu | b = %llu\n", ip, processor->code[processor->ip], a, b);

                    stack_push(&processor->stack, b - a);

                    DBG printf("<<< stack->data[stack->size - 1] = %llu, size = %d, a - b = %llu - %llu = %llu\n\n", processor->stack.data[processor->stack.size - 1], processor->stack.size, a, b, a - b);

                    processor->ip +=1;
                    DBG printf("ip = %d", ip);
                    DBG printf("sub // push(%llu - %llu)\n", b, a);
                }
                break;

            case MULL: // mull
            {
                Stack_Elem_t a = stack_pop(&processor->stack);
                Stack_Elem_t b = stack_pop(&processor->stack);

                stack_push(&processor->stack, a * b);

                processor->ip += 1;
            }
            break;

            case DIV: // div
            {
                Stack_Elem_t a = stack_pop(&processor->stack);
                Stack_Elem_t b = stack_pop(&processor->stack);

                if (a == 0)
                {
                    DBG printf ("ERROR: Division by zero\n");
                    ddlx = 0;
                }

                else
                    stack_push (&processor->stack, b / a);

                processor->ip +=1;
            }
            break;

            case SQRT: // sqrt
            {
                Stack_Elem_t a = stack_pop (&processor->stack);
                // if (a < 0) // [x] a - всегда беззнаковый
                // {
                //     printf ("ERROR: Square root of negative number\n");
                //     ddlx = 0;
                // }

                //else
                stack_push (&processor->stack, (Stack_Elem_t)sqrt(a));

                processor->ip +=1;
            }
            break;

            case SIN: // sin
            {
                Stack_Elem_t a = stack_pop (&processor->stack);
                stack_push (&processor->stack, (Stack_Elem_t)sin(a));
                processor->ip += 1;
            }
            break;

            case COS: // cos
            {
                Stack_Elem_t a = stack_pop (&processor->stack);
                stack_push (&processor->stack, (Stack_Elem_t)cos(a));
                processor->ip +=1;
            }
            break;

            case OUTPUT: // output
                {
                    $(&processor->stack.size);
                    $(&processor->stack.data[0]);
                    
                    Stack_Elem_t result = stack_pop(&processor->stack);
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
                } // 
                break;

            case JB: // [x] - правильно ли ?
                {                                                                                                                                              
                    DBG printf("size = %d\n", processor->stack.size);
                    Stack_Elem_t a = stack_pop(&processor->stack);
                    Stack_Elem_t b = stack_pop(&processor->stack);

                    DBG printf("a = %llu\n", a);
                    DBG printf("b = %llu\n", b);

                    if (a < b) // > 
                        processor->ip = processor->code[processor->ip + 1];
                    
                    else
                        processor->ip += 2;
                }
                break;
            
            case PUSHR:
                {
                    int reg_num = (processor->code[processor->ip + 1] - REG_BASE) / REG_STEP;
                    stack_push (&processor->stack, processor->registers[reg_num]);

                    processor->ip += 2;
                }
                break;

            case JMP:
                {
                    processor->ip = processor->code[processor->ip + 1];
                }
                break;

            case DED_SMESHARIK: // draw 
                {

                    //printf("");
                    for (int i = 0; i < (DEL_LINES * DEL_LINES); i++)
                    {
                        if (i % DEL_LINES == 0) printf("\n");

                        printf("%c", processor->ram[i] ? processor->ram[i] : '.');
                    }

                    printf("\n");

                    processor->ip += 1;
                }
                break;

            case JA:
                {                                                                                                                                              
                    DBG printf("size = %d\n", processor->stack.size);
                    Stack_Elem_t a = stack_pop(&processor->stack);
                    Stack_Elem_t b = stack_pop(&processor->stack);

                    DBG printf("a = %llu\n", a);
                    DBG printf("b = %llu\n", b);

                    if (a > b) 
                        processor->ip = processor->code[processor->ip + 1];
                    
                    else
                        processor->ip += 2;
                }
                break;

            case CALL:
            {
                DBG printf("\n>>> processor->ret_addr_stk.size = [%d]\n",processor->ret_addr_stk.size);
                stack_push (&processor->ret_addr_stk, processor->ip + 2);
                DBG printf("\n<<< processor->ret_addr_stk.size = [%d], \n",processor->ret_addr_stk.size);

                processor->ip = processor->code[processor->ip + 1];
            }
            break;

            case RET:
            {
                Stack_Elem_t addr = stack_pop (&processor->ret_addr_stk);
                //stack_pop (ret_addr_stack);

                processor->ip = (int) addr;
            }
            break;

            case CMD_IN:
            {
                int num  = 0;
                scanf("%d", &num);

                stack_push (&processor->stack, num);

                processor->ip += 1; 
            }
            break;

            case JNE:
            {
                DBG printf("size = %d\n", processor->stack.size);
                Stack_Elem_t a = stack_pop(&processor->stack);
                Stack_Elem_t b = stack_pop(&processor->stack);

                DBG printf("a = %llu\n", a);
                DBG printf("b = %llu\n", b);

                if (a != b)
                    processor->ip = processor->code[processor->ip + 1];
                else
                    processor->ip += 2;
            }
            break;


            case JE:
            {
                 DBG printf("size = %d\n", processor->stack.size);
                Stack_Elem_t a = stack_pop(&processor->stack);
                Stack_Elem_t b = stack_pop(&processor->stack);

                DBG printf("a = %llu\n", a);
                DBG printf("b = %llu\n", b);

                if (a == b)
                    processor->ip = processor->code[processor->ip + 1];
                else
                    processor->ip += 2;
            }
            break;

            
            case JBE:
            {
                 DBG printf("size = %d\n", processor->stack.size);
                Stack_Elem_t a = stack_pop(&processor->stack);
                Stack_Elem_t b = stack_pop(&processor->stack);

                DBG printf("a = %llu\n", a);
                DBG printf("b = %llu\n", b);

                if (a <= b)
                    processor->ip = processor->code[processor->ip + 1];
                else
                    processor->ip += 2;
            }
            break;

            
            case JAE:
            {
                 DBG printf("size = %d\n", processor->stack.size);
                Stack_Elem_t a = stack_pop(&processor->stack);
                Stack_Elem_t b = stack_pop(&processor->stack);

                DBG printf("a = %llu\n", a);
                DBG printf("b = %llu\n", b);

                if (a >= b)
                    processor->ip = processor->code[processor->ip + 1];
                else
                    processor->ip += 2;
            }
            break;

            default:
                printf("ERROR: Invalid command [%d]\n", processor->code[processor->ip]);
                ddlx = 0;
                break;
        }                                                                                              
    } 
}

int filling_the_machine_code (struct Spu* processor)
{
    FILE* file = fopen ("code_machine.txt", "r");
    if (file == NULL)
    {
        printf("\nFile with code_machine is correct\n");
    }

    for (int i = 0; i < 1000; i++)
        fscanf (file, "%d", &processor->code[i]);

    fclose (file);

    return 0;
}

int* get_arg (struct Spu* processor)
{
    DBG printf("I go into the get_arg function\n");
    DBG printf("<<< ip = %d\n", processor->ip);
    int  op_code   = processor->code[processor->ip++];
    int  arg_type  = processor->code[processor->ip++];
    int  arg_value = 0;
    int* arg_addr  = NULL;

    DBG printf ("arg_type = %04X (%d), PROCESSOR->CODE[PROCESSOR->IP] = %d\n", arg_type, arg_type, processor->code[processor->ip]);

    if (arg_type & 1) { arg_addr  = &processor->code[processor->ip]; 
                        arg_value =  processor->code[processor->ip++]; 
                        DBG printf("arg_type and 1:\n arg_addr  = %p = &processor->code[processor->ip = %p]\n"
                                                 "arg_value = %d = processor->code[processor->ip++ = %d]\n", 
                                                  arg_addr, &processor->code[processor->ip], 
                                                  arg_value, processor->code[processor->ip]) ;
                        //processor->ip++;
                      }

    if (arg_type & 2) { DBG printf("-----PROCESSOR->CODE[PROCESSOR->IP] = %d, processor->registers[processor->code[processor->ip]] = %d----------\n", processor->code[processor->ip], processor->registers[processor->code[processor->ip]]);
                        arg_addr   = &processor->registers[processor->code[processor->ip]];
                        arg_value +=  processor->registers[processor->code[processor->ip++]];
                        
                        DBG printf("arg_type and 2:\n arg_addr = %p   = &processor->registers[processor->code[processor->ip = %p]]\n"
                                                 "arg_value (%d) +=  processor->registers[processor->code[processor->ip++ = %d]]\n", 
                                                  arg_addr, &processor->registers[processor->code[processor->ip]], 
                                                  arg_value, processor->registers[processor->code[processor->ip]]);
                        //processor->ip++;                          
                      }

    if (arg_type & 4) { assert( 0 <= arg_value and arg_value <= DEL_LINES * DEL_LINES );
                        arg_addr = &processor->ram[arg_value]; // TODO регистр, arg_addr, 
                        DBG printf("arg_type and 4:\n arg_addr = %p = &processor->ram [arg_value = %p] \n", 
                                                  arg_addr, &processor->ram [arg_value] );
                      } // pop ax
                                                                                           // pop [ax + 2] 
    DBG printf(">>> ip = %d\n", processor->ip);

    (void)op_code; // наплевали, что переменная не используется 
    DBG printf("I'm exiting the get_arg function\n");

    return arg_addr;  
} 

void dump_spu (struct Spu* processor, size_t size)
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
    $(processor->ram);

    printf(RED"RAM: ");
    if (processor->ram)
    {
        for (int i = 0; i < DEL_LINES * DEL_LINES; i++)
        printf("%c "WHITE, processor->ram[i]); 
    }
    else
        printf("NULL ram");

    printf("\n\nstack (size %d): ", processor->stack.size);
    for (int i = 0; i < processor->stack.size; i++) // int i 
        printf(CYAN "[%d]=" WHITE "%llu ", i, processor->stack.data[i]);

    printf("\n\nret_addr_stk (sizw %d): ", processor->ret_addr_stk.size);
    for (int i = 0; i < processor->ret_addr_stk.size; i++)
        printf(MAGENTA"[%d]="WHITE "%llu ", i, processor->ret_addr_stk.data[i]);

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
