#include <TXLib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

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

int read_assembler_file(char machine_code[]);
int write_assembler_file(char machine_code[], int count);

int main (void)
{
    char machine_code[60] = {};

    read_assembler_file(machine_code);

    return 0;
}

int read_assembler_file(char machine_code[])
{
    FILE*  file_asm = fopen("code.asm", "r");
    if (file_asm == NULL)
    {
        printf("File (code.asm) opening error");
        return -1;
    }

    int count = 0;
    while (!feof(file_asm))
    {
        char cmd[15] ="";

        if (fscanf (file_asm, "%s ", cmd) !=  1)
            break;
        
        if (strcmp (cmd, "push") == 0)
        {
            machine_code[count] = PUSH;
            count++;
        }

        else if (strcmp (cmd, "add") == 0)
        {
            machine_code[count] = ADD;
            count++;
        }

         else if (strcmp (cmd, "sub") == 0)
        {
            machine_code[count] = SUB;
            count++;
        }

        else if (strcmp (cmd, "mull") == 0)
        {
            machine_code[count] = MULL;
            count++;
        }

        else if (strcmp (cmd, "div") == 0)
        {
            machine_code[count] = DIV;
            count++;
        }

        else if (strcmp (cmd, "sqrt") == 0)
        {
            machine_code[count] = SQRT;
            count++;
        }

        else if (strcmp (cmd, "sin") == 0)
        {
            machine_code[count] = SIN;
            count++;
        }

        else if (strcmp (cmd, "cos") == 0)
        {
            machine_code[count] = COS;
            count++;
        }

        else if (strcmp (cmd, "out") == 0)
        {
            machine_code[count] = OUTPUT;
            count++;
        }

        else if (strcmp (cmd, "hlt") == 0)
        {
            machine_code[count] = HLT;
            count++;
        }

        if (strcmp(cmd, "push") == 0)
        {
            int c = 0;

            fscanf(file_asm, "%d", &c);

            machine_code[count] = c;

            count++;
        }
    }

    write_assembler_file (machine_code, count);

    // FILE* i_machine = fopen("code_machine.txt", "w");
    // if (i_machine == NULL)
    // {
    //     printf("File (code_machine.txt) opening error");
    //     return -1;
    // }

    // for (int i = 0; i < count; i++)
    // {
    //     fprintf(i_machine, "%d ", machine_code[i]);
    // }

    fclose (file_asm);
    //fclose(i_machine);

    return 0;
}

int write_assembler_file(char machine_code[], int count)
{
    FILE* i_machine = fopen("code_machine.txt", "w");
    if (i_machine == NULL)
    {
        printf ("File (code_machine.txt) opening error");
        return -1;
    }

    for (int i = 0; i < count; i++)
        fprintf (i_machine, "%d ", machine_code[i]);

    fclose (i_machine);

    return 0;
}
