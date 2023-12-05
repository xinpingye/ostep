#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

//path to find bin for the command
static char* path = "/bin";

struct list_element
{
    char* string;
    struct list_element* next;
};

struct list
{
    struct list_element* first;
    struct list_element* last;
};

void exec_command(char pathname[], char* argv[])
{
    int rc = fork();
    if (rc == 0)
    {
        execv(pathname, argv);
        fprintf(stderr, "An error has occurred\n");
        exit(1);
    }
    else if (rc > 0)
    {
        wait(NULL);
        printf("wish> ");
        return;
    }
}



void parse_command_line(char* command_line)
{
    char *token =NULL;
    struct list arguments;
    arguments.first = NULL;
    arguments.last = NULL;
    int nums = 0;
    do{
        token = strsep(&command_line, " ");
        if (token != NULL)
        {
            struct list_element* new_element = (struct list_element*)malloc(sizeof(struct list_element));
            new_element->string = token;
            new_element->next = NULL;
            if  (new_element == NULL)
            {
                fprintf(stderr, "malloc failed\n");
                exit(1);
            }
            if (arguments.first == NULL)
            {
                arguments.first = new_element;
                arguments.last = new_element;
            }
            else
            {
                arguments.last->next = new_element;
                arguments.last = new_element;
            }
            nums++;
        }  
    }while(token != NULL);
    char* argv[nums + 1];
    struct list_element* cur = arguments.first;
    int idx_cur = 0;
    while(cur != NULL)
    {
        argv[idx_cur++] = cur->string;
        struct list_element* tmp = cur;
        cur = cur -> next;
        free(tmp);
    }
    argv[nums] = NULL;
    if (!strcmp(argv[0], "path"))
    {
        if (nums == 2)
        {
            char* tmp = strdup(argv[1]);
            tmp[strlen(tmp) - 1] = '\0';
            char* tmp_dup = (char*)malloc(strlen(tmp) + 1);
            strcpy(tmp_dup, tmp);
            path = tmp_dup;
            printf("wish> ");
            return;
        }
    }
    int command_len = -1;
    if  (nums != 1)
        command_len = strlen(argv[0]);
    else
        command_len = strlen(argv[0]) - 1;
    char pathname[strlen(path) + command_len + 1 + 1];
    for(int i = 0; i < strlen(path); i++)
        pathname[i] = path[i];
    pathname[strlen(path)] = '/';
    for(int i = 0; i < command_len; i++)
        pathname[strlen(path) + 1 + i] = argv[0][i];
    pathname[strlen(path) + command_len + 1] = '\0';
    argv[nums - 1][strlen(argv[nums - 1]) - 1] = '\0';
    if  (!strcmp(argv[0], "exit"))
        exit(0);
    else if(!strcmp(argv[0], "cd"))
    {
        if (nums != 2  || chdir(argv[1]) == -1)
        {
            fprintf(stderr, " An error has occurred\n");
            exit(1);
        }
        else
        {
            printf("wish> ");
            return;
        }
    }
    if (access((char*)pathname, X_OK) == -1)
    {
        fprintf(stderr, " An error has occurred\n");
        exit(1);
    }
    argv[nums - 1][strlen(argv[nums - 1]) - 1] = '\0';
    //access path + bin_name
    exec_command(pathname, argv);
}


void no_argument_handler()
{
    char* line = NULL;
    size_t len = 0;
    ssize_t read_nums = 0;
    printf("wish> ");
    while((read_nums = getline(&line, &len, stdin)) != -1)
        parse_command_line(line);
}

void one_argument_handler(char* filename)
{
    //fopen && getline && File*
}

int main(int argc, char* argv[])
{
    if (argc == 1)
        no_argument_handler();
    else if (argc == 2)
        one_argument_handler(argv[1]);
    else
    {

    }
    return 0;
}