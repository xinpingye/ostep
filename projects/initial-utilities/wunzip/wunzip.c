#include <stdio.h>
#include <stdlib.h>

void one_file_uncompress(char* filename)
{
    FILE* file = fopen(filename,"r");
    int nums = 0;
    char c = ' ';
    while(fread(&nums, sizeof(nums), 1, file) == 1 && fread(&c, sizeof(c), 1, file) == 1)
    {
        for (int i = 0;i < nums;i++)
            printf("%c",c);
    }
    fclose(file);
}

int main(int argc,char* argv[])
{
    if  (argc == 1)
    {
        printf("wunzip: file1 [file2 ...]\n");
        exit(1);
    }
    for (int i = 1;i < argc; i++)
        one_file_uncompress(argv[i]);
    return 0;
}