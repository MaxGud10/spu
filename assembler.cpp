#include <TXLib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

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
            machine_code[count] = 1;
            count++;
        }

        else if (strcmp (cmd, "add") == 0)
        {
            machine_code[count] = 2;
            count++;
        }

        else if (strcmp (cmd, "out") == 0)
        {
            machine_code[count] = 3;
            count++;
        }

        else if (strcmp (cmd, "hlt") == 0)
        {
            machine_code[count] = 4;
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
