#include <stdio.h>
#include <stdlib.h>
void one_line_compress(char* line, ssize_t len)
{
    int one_compress_len = 0;
    char one_char_compress = ' ';
    for (int i = 0; i < len; i++)
    {
        if(i == 0)
        {
            one_char_compress = line[0];
            one_compress_len = 1;
        }
        else
        {
            if  (line[i] == one_char_compress)
                one_compress_len++;
            else
            {   
                fwrite(&one_compress_len, sizeof(one_compress_len), 1, stdout);
                printf("%c",one_char_compress);
                one_char_compress = line[i];
                one_compress_len = 1;
            }
        }
    }
    fwrite(&one_compress_len, sizeof(one_compress_len), 1, stdout);
    fwrite(&one_char_compress, sizeof(one_char_compress), 1, stdout);
}


void merge_file_compress()
{
    FILE* file = fopen("merge.txt", "r");
    if  (file == NULL)
    {
        printf("wzip: cannot open file\n");
        exit(1);
    }
    char* line = NULL;
    size_t len = 0;
    ssize_t read_nums = 0;
    while((read_nums = getline(&line, &len, file)) != -1)
        one_line_compress(line, read_nums);
    free(line);
    fclose(file);
}

void binary_compress()
{
    FILE* src = fopen("merge.txt", "r");
    char c1 = ' ';
    char c2 = ' ';
    int length = 0;
    int start = 0;
    while((fread(&c2, 1, 1, src)) == 1)
    {
        if  (start == 0)
        {
            c1 = c2;
            length = 1;
            start = 1;
        }
        else
        {
            if  (c1 == c2)
                length++;
            else
            {
                fwrite(&length, sizeof(length), 1, stdout);
                printf("%c",c1);
                c1 = c2;
                length = 1;
            }
        }
    }
    fwrite(&length, sizeof(length), 1, stdout);
    printf("%c",c1);
    fclose(src);
}

void multiple_file_merge(int files_num_begin, int files_num_end, char** filename)
{
    FILE* merge_file = fopen("merge.txt", "w");

    for(int i = files_num_begin;i < files_num_end;i++)
    {
        FILE* file = fopen(filename[i],"r");
        if  (file == NULL)
        {
            printf("wzip: cannot open file\n");
            exit(1);
        }
        char* line = NULL;
        size_t len = 0;
        ssize_t read_nums = 0;
        while((read_nums = getline(&line, &len, file)) != -1)
            fwrite(line, read_nums, 1, merge_file);
        fclose(file);
        free(line);
    }
    fclose(merge_file);
}

int main(int argc,char* argv[])
{
    if  (argc == 1)
    {
        printf("wzip: file1 [file2 ...]\n");
        exit(1);
    }
    multiple_file_merge(1, argc, argv);
    //merge_file_compress();
    binary_compress();
    //remove("merge.txt");
    return 0;
}