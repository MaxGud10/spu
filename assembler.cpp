#include <TXLib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#pragma GCC diagnostic ignored "-Wredundant-tags"

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
    DED_SMESHARIK = 16
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

struct FIXUP
{
    int pos_of_wrong_label_address;
    int label_number_with_address;
};

struct ASM
{
    int  machine_code [100];// TODO: считать размер файла и заказать память
    int  count;
    char arg          [52];
    //char cmd          [100];
    int  labels       [100];
    struct FIXUP fixup[50];  
    int  fixup_index;
};

int read_assembler_file  (struct ASM* asm_info);
int write_assembler_file (struct ASM* info_asm);

//int compile_arg (struct ASM* asm_info, FILE* file_asm, char* cmd);
int compile_arg (struct ASM* asm_info, FILE* file_asm);

int find_bracket_pluse (struct ASM* asm_info, char* cmd);

int ctor_labels       (struct ASM* asm_info);
int ctor_machine_code (struct ASM* asm_info);

int do_circle (struct ASM* asm_info, char* buf);

int do_fixup    (struct ASM* asm_info);

int dump_asm       (struct ASM* info_asm, size_t size);
int dump_mach_code (struct ASM* asm_info, int size);

int main (void)
{
    struct ASM asm_info = {};

    ctor_labels       (&asm_info);
    ctor_machine_code (&asm_info);

    read_assembler_file (&asm_info);

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
    while (!feof(file_asm))
    {
        char cmd[100] = ""; // TODO: константу

        //dump_asm(asm_info, 24);

        if (fscanf (file_asm, "%s", cmd) !=  1)
            break; 
        
        printf("\n>>> cmd = '%s'\n", cmd);

        if (strchr(cmd, ':') != NULL)
        {
            int number = 0;
            sscanf (cmd, "%d:", &number);
            asm_info->labels[number] = asm_info->count;
        }

        if (strcmp (cmd, "push") == 0) // TODO: структурами убрать копипаст
        {

            asm_info->machine_code[asm_info->count] = PUSH;
            asm_info->count++; 
            
            //fscanf (file_asm, "%s", asm_info.arg);

            //compile_arg (asm_info, file_asm, cmd);
            compile_arg (asm_info, file_asm);

        }

        else if (strcmp (cmd, "pop") == 0)
        {
            asm_info->machine_code[asm_info->count] = POP;
            asm_info->count++;

            //compile_arg (asm_info, file_asm, cmd);
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

            //compile_arg (asm_info, file_asm, cmd);
            compile_arg (asm_info, file_asm);
        }

        else if (strcmp (cmd, "pushr") == 0)
        {
            asm_info->machine_code[asm_info->count] = PUSHR;
            asm_info->count++;

            //compile_arg (asm_info, file_asm, cmd);
            compile_arg (asm_info, file_asm);
        }

        else if (strcmp(cmd, "nob") == 0) // пустая функция
        {   
            asm_info->machine_code[asm_info->count] = NOB;
            asm_info->count++;  
        }

        else if (strcmp(cmd, "jmp") == 0)
        {
            asm_info->machine_code[asm_info->count] = JMP;
            asm_info->count++;

            //compile_arg (asm_info, file_asm, cmd);
            compile_arg (asm_info, file_asm);
        }

        else if (strcmp(cmd, "ded_smesharik") == 0)
        {
            asm_info->machine_code[asm_info->count] = DED_SMESHARIK;
            asm_info->count++;

            compile_arg (asm_info, file_asm);
        }
    }

    do_fixup(asm_info);

    write_assembler_file (asm_info);

    fclose (file_asm);
    //fclose(i_machine);

    return 0;
}

int do_circle (struct ASM* asm_info, char buf[])
{
    int x = 0;
    asm_info->machine_code[asm_info->count++] ;
    int y = 0;
    if (x + y <= 100)
    {
        asm_info->machine_code[asm_info->count++] = '*';
    }

    else
    {
        asm_info->machine_code[asm_info->count++] = NOB; 
    }
    return 0;
}    

int ctor_labels (struct ASM* asm_info) // заполнение -1 весь массив labels 
{
    int size = sizeof(asm_info->labels) / sizeof (asm_info->labels[0]);
    for (int i = 0; i < size; i++)
            asm_info->labels[i] = -1;

    return 0;
}

int ctor_machine_code (struct ASM* asm_info)
{
    int size = sizeof(asm_info->machine_code) / sizeof (asm_info->machine_code[0]);
    for (int i = 0; i < size; i++)
        asm_info->machine_code[i] = -1;

    return 0;
}

int dump_mach_code (struct ASM* asm_info, int size) // для отладки // только машинные код 
{
    printf("machine_code: ");
    for (int i = 0; i < size; i++)
        printf("%02d ", asm_info->machine_code[i]);

    printf("count = %d\n", asm_info->count);

    return 0;
}

int compile_arg (struct ASM* asm_info, FILE* file_asm) 
{
    fscanf (file_asm, " %[^\n]", asm_info->arg);

    int number = 0;

    printf("\n\nNow I will search for + and [ to add types\n");
    int find_bracket = (strchr (asm_info->arg, '[') != NULL);
    int find_plus    = (strchr (asm_info->arg, '+') != NULL);
    char buf [3] = "";
    printf("find_bracket = %d, find_plus = %d\n", find_bracket, find_plus);

    int command_type = 0;
    if (find_bracket && find_plus)
    {
        printf("The first comparison worked\n");
        sscanf (asm_info->arg, " [%1[a-d]x + %d]", buf, &number);
        printf("    arg = '%s', buf = '%s', number = %d\n", asm_info->arg, buf, number);

        command_type = 7;
        asm_info->machine_code[asm_info->count++] = command_type;

        printf("\nFIND_BRACKET && FIND_PLUS COMPLETED\n");
        dump_mach_code (asm_info, 24);
    }

    if (find_bracket && !find_plus)
    {
        printf("The second comparison worked\n");
        int res_1 = sscanf (asm_info->arg, "[%1[a-d]x]", buf);
        int res_2 = sscanf (asm_info->arg, "[%d]", &number);
        printf("    arg = '%s', buf = '%s', number = %d\n", asm_info->arg, buf, number);

        if (res_1 == 1)
        {
            command_type = 6;
        }

        if (res_2 == 1)
        {
            command_type = 5;
            asm_info->machine_code [asm_info->count++] = command_type;
        }

        printf ("\nFIND_BRACKET && !FIND_PLUSE COMPLETED\n");
        dump_mach_code (asm_info, 24);
    }

    if (!find_bracket && find_plus)
    {
        printf("The third comparison worked\n");
        int res = sscanf (asm_info->arg, "%1[a-d]x + %d", buf, &number);
        //char ddlx = buf[0] + 'x';
        printf("    arg = '%s', buf = '%s', number = %d\n", asm_info->arg, buf, number);

        command_type = 3;
        asm_info->machine_code[asm_info->count++] = command_type;
        asm_info->machine_code[asm_info->count++] = number;

        printf("\n!FIND_BRACKET && FIND_PLUS COMPLETED\n");    
        dump_mach_code (asm_info, 24);
    }

    if (!find_bracket && !find_plus)
    {
        printf("The fourth comparison worked\n");
        int res_1 = sscanf(asm_info->arg, "%1[a-d]x ", buf);
        int res_2 = sscanf(asm_info->arg, "%d", &number);
        printf("    arg = '%s', buf = '%s', number = %d\n", asm_info->arg, buf, number);

        if (res_1 == 1) 
        {
            command_type = 2; // 2 // 0b00000010
            asm_info->machine_code[asm_info->count++] = command_type;
        }

        if (res_2 == 1) 
        {
            command_type = 1; // 1 // 0b00000001
            asm_info->machine_code[asm_info->count++] = command_type;
        }    

        printf("\n!FIND_BRACKET && !FIND_PLUSE COMPLETED\n");
        dump_mach_code (asm_info, 24);
    }

    
    //find_bracket_pluse (asm_info, cmd);
    printf("\n\n%s(): Now I'm going to check the arguments for registers...\n", __func__);
 
    //if (strcmp (asm_info->arg, "ax") == 0)
    if (strcmp (buf, "a") == 0)
    {
        asm_info->machine_code[asm_info->count] = AX;
        printf("%s(): I found the AX register (arg = %s, count = %d)\n", __func__, asm_info->arg, asm_info->count);

        printf("\nadded the AX register\n");
        dump_mach_code (asm_info, 24);
    }

    //else if (strcmp (asm_info->arg, "bx") == 0)
    else if (strcmp (buf, "b") == 0)
    {
        asm_info->machine_code[asm_info->count] = BX;
        printf("%s(): I found the BX register (arg = %s, count = %d)\n", __func__, asm_info->arg, asm_info->count);

        printf("\nadded the BX register\n");
        dump_mach_code (asm_info, 24);
    }

    //else if (strcmp (asm_info->arg, "cx") == 0)
    else if (strcmp (buf, "c") == 0)
    {
        asm_info->machine_code[asm_info->count] = CX;
        printf("%s(): I found the CX register (arg = %s, count = %d) \n", __func__, asm_info->arg, asm_info->count);

        printf("\nadded the CX register\n");
        dump_mach_code (asm_info, 24);
    }

    //else if (strcmp(asm_info->arg, "dx") == 0)
    else if (strcmp (buf, "d") == 0)
    {
        asm_info->machine_code[asm_info->count] = DX;
        printf("%s():I found the AD register (arg = %s, count = %d)\n", __func__, asm_info->arg, asm_info->count);

        printf("\nadded the DX register\n");
        dump_mach_code (asm_info, 24);
    }
    printf("I've finished searching for registers\n");
    // else
    // {
    //     printf()
    // }

    printf("\n\nNow I'm going to look for Value\n");
    if (sscanf (asm_info->arg, "%d", &number) == 1)
    {
        printf("    number = %d !!!\n", number);
        asm_info->machine_code[asm_info->count] = number; 

        printf("value:");
        dump_mach_code (asm_info, 24);
    }

    printf("\n\nI will look for metki\n");
    if (strchr(asm_info->arg, ':') != NULL)
    {
        
        sscanf (asm_info->arg, "%d:", &number);
        printf("%s():    number = %d\n", __func__, number);
        asm_info->machine_code[asm_info->count] = asm_info->labels[number];
        printf("%s():    machine_code = %d\n", __func__, asm_info->machine_code[asm_info->count]);

        if (asm_info->labels[number] == -1)
        {   
            asm_info->fixup[asm_info->fixup_index].pos_of_wrong_label_address = asm_info->count; 
            asm_info->fixup[asm_info->fixup_index].label_number_with_address  = number;
            printf("%s(): asm_info->labels[number]   = %d\n", __func__, asm_info->labels[number]);
            printf("%s(): pos_of_wrong_label_address = %d\n", __func__, asm_info->fixup[asm_info->fixup_index].pos_of_wrong_label_address);
            printf("%s(): label_number_with_address  = %d\n", __func__, asm_info->fixup[asm_info->fixup_index].label_number_with_address);

            asm_info->fixup[asm_info->fixup_index++]; // переделать 
        }
    }

    do_circle (asm_info, buf);

    //dump_asm(asm_info, 24);
    asm_info->count++;

    printf("\n-----------------------------------------------------------------------------------------\n");

    return 0;
}

int find_bracket_pluse (struct ASM* asm_info, char* cmd) // TODO переименовать
{
    int num = 0;
    int find_bracket = (strchr (asm_info->arg, '[') != NULL);
    int find_plus    = (strchr (asm_info->arg, '+') != NULL);
    printf("%s(): find_bracket = %d and find_plus = %d", __func__, find_bracket, find_plus); 

    int command_type = 0;
    if (find_bracket && find_plus)
    {
        int res = sscanf(cmd, " [%1[a-d]x + %d]", asm_info->arg, &num);
        //assert (res == 2);

        command_type = 7; // 7 // 0b00000111
        asm_info->machine_code [asm_info->count++] = command_type;
        //asm_info->machine_code [asm_info->count++] = asm_info->arg [0] - 'a' + 1;

        return 0;
    }

    if (find_bracket && !find_plus)
    {
        int res_1 = sscanf (cmd, "[%1[a-d]x]", asm_info->arg);
        int res_2 = sscanf (cmd, "[%d]", &num);

        if (res_1 == 1)
        {
            command_type = 6; // 6 // 0b00000110
            //asm_info->machine_code[asm_info->count++] = asm_info->arg[0] - 'a' + 1;
        }

        if (res_2 == 1)
        {
            command_type = 5; // 5 // 0b00000101
            asm_info->machine_code [asm_info->count++] = command_type;
            //asm_info->machine_code [asm_info->count++] = num;
        }

        return 0;
    }

    if (!find_bracket && find_plus)
    {
        int res = sscanf(cmd, "%1[a-d]x + %d", asm_info->arg, &num);
        assert(res == 2);

        command_type = 3; // 3 // 0b00000011
        asm_info->machine_code[asm_info->count++] = command_type;
        asm_info->machine_code[asm_info->count++] = num;
        //asm_info->machine_code[asm_info->count++] = asm_info->arg[0] - 'a' + 1;

        return 0;
    }

    if (!find_bracket && !find_plus)
    {
        int res_1 = sscanf(cmd, "%1[a-d]x", asm_info->arg);
        int res_2 = sscanf(cmd, "%d", &num);

        if (res_1 == 1) 
        {
            command_type = 2; // 2 // 0b00000010
            asm_info->machine_code[asm_info->count++] = command_type;
            //asm_info->machine_code[(asm_info->count)++] = asm_info->arg[0] - 'a' + 1;
        }

        if (res_2 == 1) 
        {
            command_type = 1; // 1 // 0b00000001
            asm_info->machine_code[asm_info->count++] = command_type;
            //asm_info->machine_code[(asm_info->count)++] = num;
        }
    }
    printf("\n-----------------------------------------------------------------------------------------\n");

    return 0; // -1
}

int do_fixup (struct ASM* asm_info)
{
    for (int i = 0; i < asm_info->fixup_index; i++)
    {
        int code_to_fixup     = asm_info->fixup[i].pos_of_wrong_label_address;
        int address_on_change = asm_info->labels[asm_info->fixup[i].label_number_with_address];
        printf ("code_to_fixup     = %d\n", code_to_fixup);
        printf ("address_on_change = %d\n", address_on_change);
        asm_info->machine_code[code_to_fixup] = address_on_change;
    }

    dump_asm(asm_info, 30);

    return 0;
}

int dump_asm (struct ASM* asm_info, size_t size)
{
    printf("\n-----------------------------------------------------------------------------------------------------\n");
    printf("              ");
    for (size_t i = 0; i < size; i++)
        printf(" %02llu", i);

    printf("\n ");

    printf("machine_code: ");
    for (size_t i = 0; i < size; i++)
        printf("%02d ", asm_info->machine_code[i]);
    
    printf("\n");

    for (int i = 0; i <  asm_info->count; i++)
        printf("               ");

    printf(" ^ count = %d\n\n", asm_info->count);

    printf("              ");
    for (size_t i = 0; i < size; i++)
        printf(" %02llu", i);

    printf("\n");

    printf("       ");
    printf("labels: ");
    for (size_t i = 0; i < size; i++)
        printf("%02d ", asm_info->labels[i]);

    printf("\n\n");

    printf("        ");
    printf("FIXUP: ");
    for (int i = 0; i < asm_info->fixup_index; i++)
    {
        printf("{%02d | %02d} ", asm_info->fixup[i].pos_of_wrong_label_address, 
                                 asm_info->fixup[i].label_number_with_address);
    }

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

19:
pushr ax 
pushr ax
mull 
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
jb 19:

pushr bx
out 

hlt 



*/