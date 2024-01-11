#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

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

void binary_compress2(char* src, long len)
{
    char c1 = ' ';
    char c2 = ' ';
    int length = 0;
    int start = 0;
    int cur_len = 0;
    while(cur_len < len)
    {
        c2 = *src;
        src++;
        cur_len++;
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

    char filename[] = "merge.txt";

    struct stat sbuf;
    stat(filename, &sbuf);
    long filesize = sbuf.st_size;

    int srcfd = open(filename, O_RDONLY, 0);
    char* srcp = mmap(0, filesize, PROT_READ, MAP_PRIVATE, srcfd, 0);
    close(srcfd);

    //merge_file_compress();
    binary_compress2(srcp, filesize);
    //remove("merge.txt");
    return 0;
}
