#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

bool line_find_search_term(char* search_term, char* line)
{
    if (strstr(line, search_term) == NULL)
        return false;
    else
        return true;
}

void stdin_handler(char* search_term)
{
    char* line = NULL;
    size_t len = 0;
    ssize_t read_nums = 0;
    while((read_nums = getline(&line, &len, stdin)) != -1)
    {
        if  (line_find_search_term(search_term, line))
            printf("%s",line);
    }
    free(line);
}

void file_handler(char* search_term, char* filename)
{
    FILE* file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("wgrep: cannot open file\n");
        exit(1);
    }
    char* line = NULL;
    size_t len = 0;
    ssize_t read_nums = 0;
    while((read_nums = getline(&line, &len, file)) != -1)
    {
        if  (line_find_search_term(search_term, line))
            printf("%s",line);
    }
    free(line);
    fclose(file);
}

int main(int argc,char* argv[])
{
    if (argc == 1)
    {
        printf("wgrep: searchterm [file ...]\n");
        exit(1);
    }
    else  if(argc == 2)
        stdin_handler(argv[1]);
    else
    {
        for(int i = 2;i < argc;i++)
            file_handler(argv[1], argv[i]);
    }
    return 0;
}