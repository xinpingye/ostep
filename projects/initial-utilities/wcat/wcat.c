#include <stdio.h>
#include <stdlib.h>
#define MAX_CHAR_NUMS_EVERY_LINE 150

void cat_one_file(char* filename)
{
    FILE* file = fopen(filename,"r");
    if (file == NULL)
    {
        printf("wcat: cannot open file\n");
        exit(1);
    }
    char buffer[MAX_CHAR_NUMS_EVERY_LINE + 1];
    while (fgets(buffer,MAX_CHAR_NUMS_EVERY_LINE,file) != NULL)
        printf("%s", buffer);
    fclose(file);
}


int main(int argc,char* argv[])
{
    if (argc == 1)
        return 0;
    for (int i = 1;i < argc; i++)
        cat_one_file(argv[i]);
    return 0;

}
