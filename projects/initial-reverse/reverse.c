#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
struct list_element
{
    struct list_element* next;
    char* string;
    ssize_t length;
};

struct list
{
    struct list_element* first;
    struct list_element* last;
};

static struct list data_list; 

void store_data(char* line, ssize_t length)
{
    if  (data_list.first == NULL)
    {
        struct list_element* new_element = (struct list_element*)malloc(sizeof(struct list_element));
        if  (new_element == NULL)
        {
            fprintf(stderr, "malloc failed\n");
            exit(1);
        }
        new_element->next = NULL;
        new_element->string = line;
        new_element->length = length;
        data_list.first = new_element;
        data_list.last = new_element;
    }
    else
    {
        struct list_element* new_element = (struct list_element*)malloc(sizeof(struct list_element));
        if  (new_element == NULL)
        {
            fprintf(stderr, "malloc failed\n");
            exit(1);
        }
        new_element->next = data_list.first;
        new_element->string = line;
        new_element->length = length;
        data_list.first = new_element;
    }
}

void reverse_print(FILE* dst)
{
    struct list_element* cur = data_list.first;
    while(cur != NULL)
    {
        fprintf(dst, "%s", cur->string);
        cur = cur->next;
    }
}

void release_data()
{
    struct list_element* cur = data_list.first;
    while(cur != NULL)
    {
        struct list_element* next = cur->next;
        free(cur->string);
        cur = next;
    }
}

void no_argument_handler()
{
    char* line = NULL;
    size_t len = 0;
    ssize_t read_nums = 0;
    while((read_nums = getline(&line, &len, stdin)) != -1)
    {
        char* str_cpy = (char*)malloc(read_nums);
        if  (str_cpy == NULL)
        {
            fprintf(stderr, "malloc failed\n");
            exit(1);
        }
        memcpy(str_cpy,line,read_nums);
        store_data(str_cpy, read_nums);
    }
    reverse_print(stdout);
    release_data();
}

void one_argument_handler(char* filename)
{
    FILE* file = fopen(filename, "r");
    if  (file == NULL)
    {
        fprintf(stderr, "reverse: cannot open file '/no/such/file.txt'\n");
        exit(1);
    }
    char* line = NULL;
    size_t len = 0;
    ssize_t read_nums = 0;
    while((read_nums = getline(&line, &len, file)) != -1)
    {
        char* str_cpy = (char*)malloc(read_nums);
        if  (str_cpy == NULL)
        {
            fprintf(stderr, "malloc failed\n");
            exit(1);
        }
        memcpy(str_cpy,line,read_nums);
        store_data(str_cpy, read_nums);
    }
    reverse_print(stdout);
    release_data();
    fclose(file);
}

void two_argument_handler(char* filename1,char* filename2)
{
    FILE* src = fopen(filename1, "r");
    if  (src == NULL)
    {
        fprintf(stderr, "reverse: cannot open file '/no/such/file.txt'\n");
        exit(1);
    }
    FILE* dst = fopen(filename2, "w");
    if  (dst == NULL)
    {
        fprintf(stderr, "reverse: cannot open file '/no/such/file.txt'\n");
        exit(1);
    }
    char* line = NULL;
    size_t len = 0;
    ssize_t read_nums = 0;
    while((read_nums = getline(&line, &len, src)) != -1)
    {
        char* str_cpy = (char*)malloc(read_nums);
        if  (str_cpy == NULL)
        {
            fprintf(stderr, "malloc failed\n");
            exit(1);
        }
        memcpy(str_cpy,line,read_nums);
        store_data(str_cpy, read_nums);
    }    
    reverse_print(dst);
    release_data();
    fclose(src);
    fclose(dst);
}

int main(int argc,char* argv[])
{
    data_list.first = NULL;
    data_list.last = NULL;
    if  (argc == 1)
        no_argument_handler();
    else if(argc == 2)
        one_argument_handler(argv[1]);
    else if(argc == 3)
    {
        struct stat stat1, stat2;
        int rec1 = stat(argv[1], &stat1);
        if  (rec1 == -1)
        {
            fprintf(stderr, "reverse: cannot open file '/no/such/file.txt'\n");
            exit(1);
        }
        int rec2 = stat(argv[2], &stat2);
        if  (rec1 == 0 && rec2 == 0 && stat1.st_ino == stat2.st_ino)
        {
            fprintf(stderr, "reverse: input and output file must differ\n");
            exit(1);
        }
        two_argument_handler(argv[1], argv[2]);
    }
    else
    {
        fprintf(stderr, "usage: reverse <input> <output>\n");
        exit(1);
    }
    return 0;
}