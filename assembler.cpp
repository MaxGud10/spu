#include <TXLib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#pragma GCC diagnostic ignored "-Wredundant-tags"

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
    JB      = 13 // JUMP
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

struct ASM
{
    int machine_code [100];// TODO: считать размер файла и заказать память
    int count;
    char str[52];
};

int read_assembler_file  (struct ASM* asm_info);
int write_assembler_file (struct ASM* info_asm);

int compile_arg (struct ASM* asm_info, FILE* file_asm); 

int dump_asm (struct ASM* info_asm, size_t size);

int main (void)
{
    struct ASM asm_info = {};

    read_assembler_file(&asm_info);

    return 0;
}

int read_assembler_file (struct ASM* asm_info)
{
    FILE* file_asm = fopen("code.asm", "r");
    if (file_asm == NULL)
    {
        printf("File (code.asm) opening error");
        return -1;
    }
  
    asm_info->count = 0;
    while (!feof(file_asm)) // TODO: лучше считать в буфер
    {
        char cmd[100] = ""; // TODO: константу

        //dump_asm(asm_info, 24);

        if (fscanf (file_asm, "%s", cmd) !=  1)
            break; 
        
        printf("\n>>> cmd = '%s'\n", cmd);

        if (strcmp (cmd, "push") == 0) // TODO: структурами убрать копипаст
        {

            asm_info->machine_code[asm_info->count] = PUSH;
            asm_info->count++; 
            
            //fscanf (file_asm, "%s", asm_info.str);

            compile_arg (asm_info, file_asm);

        }

        else if (strcmp (cmd, "pop") == 0)
        {
            asm_info->machine_code[asm_info->count] = POP;
            asm_info->count++;

            compile_arg (asm_info, file_asm);
        }

        else if (strcmp (cmd, "add") == 0)
        {
            asm_info->machine_code[asm_info->count] = ADD;
            asm_info->count++;
        }

        else if (strcmp (cmd, "sub") == 0)
        {
            asm_info->machine_code[asm_info->count] = SUB;
            asm_info->count++;
        }

        else if (strcmp (cmd, "mull") == 0)
        {
            asm_info->machine_code[asm_info->count] = MULL;
            asm_info->count++;
        }

        else if (strcmp (cmd, "div") == 0)
        {
            asm_info->machine_code[asm_info->count] = DIV;
            asm_info->count++;
        }

        else if (strcmp (cmd, "sqrt") == 0)
        {
            asm_info->machine_code[asm_info->count] = SQRT;
            asm_info->count++;
        }

        else if (strcmp (cmd, "sin") == 0)
        {
            asm_info->machine_code[asm_info->count] = SIN;
            asm_info->count++;
        }

        else if (strcmp (cmd, "cos") == 0)
        {
            asm_info->machine_code[asm_info->count] = COS;
            asm_info->count++;
        }

        else if (strcmp (cmd, "out") == 0)
        {
            asm_info->machine_code[asm_info->count] = OUTPUT;
            asm_info->count++;
        }

        else if (strcmp (cmd, "hlt") == 0)
        {
            asm_info->machine_code[asm_info->count] = HLT;
            asm_info->count++;
        }

        else if (strcmp (cmd, "jb") == 0)
        {
            asm_info->machine_code[asm_info->count] = JB;
            asm_info->count++;



            compile_arg (asm_info, file_asm);
        }

        else if (strcmp (cmd, "pushr") == 0)
        {
            asm_info->machine_code[asm_info->count] = PUSHR;
            asm_info->count++;

            compile_arg (asm_info, file_asm);
        }
    }

    write_assembler_file (asm_info);

    fclose (file_asm);
    //fclose(i_machine);

    return 0;
}

int compile_arg (struct ASM* asm_info, FILE* file_asm) 
{
    fscanf (file_asm, "%s", asm_info->str);

    printf("\n<<< str = %s\n", asm_info->str);   
    if (strcmp (asm_info->str, "ax") == 0)
        asm_info->machine_code[asm_info->count] = AX;

    else if (strcmp (asm_info->str, "bx") == 0)
        asm_info->machine_code[asm_info->count] = BX;

    else if (strcmp (asm_info->str, "cx") == 0)
        asm_info->machine_code[asm_info->count] = CX;

    else if (strcmp(asm_info->str, "dx") == 0)
        asm_info->machine_code[asm_info->count] = DX;

    printf(">>> str = '%s'\n", asm_info->str); 

    int number = 0;
    if (sscanf (asm_info->str, "%d", &number) == 1)
    {
        printf("    number = %d !!!\n", number);
        asm_info->machine_code[asm_info->count] = number; 
    }

    dump_asm(asm_info, 24);
    asm_info->count++;

    return 0;
}

int dump_asm (struct ASM* info_asm, size_t size)
{
    printf("\n-----------------------------------------------------------------------------------------------------\n");
    for (size_t i = 0; i < size; i++)
        printf(" %02llu", i);

    printf("\n ");

    for (size_t i = 0; i < size; i++)
        printf("%02d ", info_asm->machine_code[i]);
    
    printf("\n");

    for (int i = 0; i <  info_asm->count; i++)
        printf("   ");

    printf(" ^ count = %d\n\n", info_asm->count);

    printf("\n-----------------------------------------------------------------------------------------------------\n");

    getchar();

    return 0;
}

int write_assembler_file (struct ASM* info_asm)
{
    FILE* i_machine = fopen("code_machine.txt", "w");
    if (i_machine == NULL)
    {
        printf ("File (code_machine.txt) opening error");
        return -1;
    }

    for (int i = 0; i < info_asm->count; i++)
        fprintf (i_machine, "%02d ", info_asm->machine_code[i]);

    fclose (i_machine);

    return 0;
}
/*
push 0
push 0
pop ax
pop bx
pushr ax 
pushr ax
mul 
pushr bx
add
pop bx
pushr ax 
push 1
add
pop ax
pushr bx
out 
push 6
pushr ax
jb 8
out 
hlt 



*/