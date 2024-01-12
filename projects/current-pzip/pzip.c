#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

char filename1[] = "file1.txt";
char filename2[] = "file2.txt";
char filename3[] = "file3.txt";
char filename4[] = "file4.txt";

char* all_file_name[] = {filename1, filename2, filename3, filename4};

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

void binary_compress3(char* src, long len, int file_to_write)
{
    FILE* dst = fopen(all_file_name[file_to_write], "w");
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
                fwrite(&length, sizeof(length), 1, dst);
                fwrite(&c1, sizeof(c1), 1, dst);
                //printf("%c",c1);
                c1 = c2;
                length = 1;
            }
        }
    }
    fwrite(&length, sizeof(length), 1, dst);
    fwrite(&c1, sizeof(c1), 1, dst);
    //printf("%c",c1);
    fclose(dst);
}

int binary_compress4(char* src, long len, int file_to_write)
{
    FILE* dst = fopen(all_file_name[file_to_write], "w");
    char c1 = ' ';
    char c2 = ' ';
    int length = 0;
    int start = 0;
    int cur_len = 0;
    int nums = 0;
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
                fwrite(&length, sizeof(length), 1, dst);
                fwrite(&c1, sizeof(c1), 1, dst);
                nums++;
                //printf("%c",c1);
                c1 = c2;
                length = 1;
            }
        }
    }
    fwrite(&length, sizeof(length), 1, dst);
    fwrite(&c1, sizeof(c1), 1, dst);
    nums++;
    //printf("%c",c1);
    fclose(dst);
    return nums;
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

struct args
{
    char* srcp;
    long filesize;
    int file_num;
}; 

int rc_array[4];

void* worker(void* arg)
{
    struct args *tmp = (struct args *)(arg);
    char* srcp = tmp->srcp;
    long filesize = tmp->filesize;
    int file_num = tmp->file_num;
    rc_array[file_num] = binary_compress4(srcp, filesize, file_num);
    return NULL;
}

struct data_item
{
    char item_char;
    int item_int;
};

struct data_item inter_merge(struct data_item data_item1, struct data_item data_item2, int nums, char* srcp)
{
    struct data_item rc_data_item;
    if (nums != 1)
    {
        if (data_item1.item_char == data_item2.item_char)
        {
            int length = data_item1.item_int + data_item2.item_int;
            fwrite(&length, sizeof(length), 1, stdout);
            fwrite(&data_item1.item_char, sizeof(data_item1.item_char), 1, stdout);
        }
        else
        {
            fwrite(&data_item1.item_int, sizeof(data_item1.item_int), 1, stdout);
            fwrite(&data_item1.item_char, sizeof(data_item1.item_char), 1, stdout);
            fwrite(&data_item2.item_int, sizeof(data_item2.item_int), 1, stdout);
            fwrite(&data_item2.item_char, sizeof(data_item2.item_char), 1, stdout);
        }
        fwrite(srcp + 5, sizeof(char), (nums - 2) * 5, stdout);
        rc_data_item.item_int = *(int*)(srcp + (nums -1) * 5);
        rc_data_item.item_char = *(char*)(srcp + (nums - 1) * 5 + 4);
    }
    else if (nums == 1)
    {   
        if (data_item1.item_char == data_item2.item_char)
        {
            rc_data_item.item_char = data_item1.item_char;
            rc_data_item.item_int = data_item1.item_int + data_item2.item_int;
        }
        else
        {
            fwrite(&data_item1.item_int, sizeof(data_item1.item_int), 1, stdout);
            fwrite(&data_item1.item_char, sizeof(data_item1.item_char), 1, stdout);
            rc_data_item.item_int = data_item2.item_int;
            rc_data_item.item_char = data_item2.item_char;
        }
    }
    return rc_data_item;
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

    struct args all_args[4];
    all_args[0].srcp = srcp;
    all_args[0].filesize = filesize / 4;
    all_args[0].file_num = 0;
    all_args[1].srcp = srcp + filesize / 4;
    all_args[1].filesize = filesize / 4;
    all_args[1].file_num = 1;
    all_args[2].srcp = srcp + filesize / 4 * 2;
    all_args[2].filesize = filesize / 4;
    all_args[2].file_num = 2;
    all_args[3].srcp =srcp + filesize / 4 * 3;
    all_args[3].filesize = filesize - filesize / 4 * 3;
    all_args[3].file_num = 3;

    pthread_t threads[4];
    for (int i = 0;i < 4;i++)
    {
        pthread_create(&threads[i], NULL, worker, &all_args[i]);
    }

    for (int i = 0;i < 4;i++)
    {
        pthread_join(threads[i], NULL);
    }

    int nums1 = rc_array[0];
    int nums2 = rc_array[1];
    int nums3 = rc_array[2];
    int nums4 = rc_array[3];
    /*
     * just to split to fuor part
    //merge_file_compress();
    int nums = binary_compress4(srcp, filesize, 0);
    //printf("nums:%d\n", nums);
    */
    /*
    
    int nums1 = binary_compress4(srcp, filesize / 4, 0);
    int nums2 = binary_compress4(srcp + filesize / 4, filesize / 4, 1);
    int nums3 = binary_compress4(srcp + filesize / 4 * 2, filesize / 4, 2);
    int nums4 = binary_compress4(srcp + filesize / 4 * 3, filesize - filesize / 4 * 3, 3);
    */

    //printf("nums1:%d nums2:%d nums3:%d nums4:%d\n", nums1, nums2, nums3, nums4);

    /*
     * just to merge four part to one 
    */

    /*
     * first part mmap
    */
    struct stat part1_sbuf;
    stat(all_file_name[0], &part1_sbuf);
    long file1_size = part1_sbuf.st_size;

    int src1_fd = open(all_file_name[0], O_RDONLY, 0);
    char* src1_p = mmap(0, file1_size, PROT_READ, MAP_PRIVATE, src1_fd, 0);
    close(src1_fd);

    /*
     * second part mmap
    */
    struct stat part2_sbuf;
    stat(all_file_name[1], &part2_sbuf);
    long file2_size = part2_sbuf.st_size;

    int src2_fd = open(all_file_name[1], O_RDONLY, 0);
    char* src2_p = mmap(0, file2_size, PROT_READ, MAP_PRIVATE, src2_fd, 0);
    close(src2_fd);

    /*
     * third part mmap
    */
    struct stat part3_sbuf;
    stat(all_file_name[2], &part3_sbuf);
    long file3_size = part3_sbuf.st_size;

    int src3_fd = open(all_file_name[2], O_RDONLY, 0);
    char* src3_p = mmap(0, file3_size, PROT_READ, MAP_PRIVATE, src3_fd, 0);
    close(src3_fd);

    /*
     * fourth part mmap
    */
    struct stat part4_sbuf;
    stat(all_file_name[3], &part4_sbuf);
    long file4_size = part4_sbuf.st_size;

    int src4_fd = open(all_file_name[3], O_RDONLY, 0);
    char* src4_p = mmap(0, file4_size, PROT_READ, MAP_PRIVATE, src4_fd, 0);
    close(src4_fd);

    /*
     * begin output and handle inter case
    */

    //write(1, src1_p, (nums1 - 1) * 5);
    fwrite(src1_p, sizeof(char), (nums1 - 1) * 5, stdout);

    int  part1_end_len = *(int*)(src1_p + (nums1 -1) * 5);
    char part1_end_char = *(char*)(src1_p + (nums1 - 1) * 5 + 4);

    int  part2_front_len = *(int*)(src2_p);
    char part2_front_char = *(char*)(src2_p + 4);

    struct data_item rc_data_item;
    struct data_item data_item1; 
    struct data_item data_item2;

    data_item1.item_int = part1_end_len;
    data_item1.item_char = part1_end_char;
    data_item2.item_int = part2_front_len;
    data_item2.item_char = part2_front_char;
    rc_data_item = inter_merge(data_item1, data_item2, nums2, src2_p);


    /*
    char merge1_char;
    int merge1_length;

    if (nums2 != 1)
    {
        //printf("--------");
        if (part1_end_char == part2_front_char)
        {
            int length = part1_end_len + part2_front_len;
            fwrite(&length, sizeof(length), 1, stdout);
            //printf("%d", length);
            //printf("%c",part1_end_char); 
            fwrite(&part1_end_char, sizeof(part1_end_char), 1, stdout);
        }
        else
        {
            fwrite(&part1_end_len, sizeof(part1_end_len), 1, stdout);
            //printf("%d", part1_end_len);
            //printf("%c",part1_end_char); 
            fwrite(&part1_end_char, sizeof(part1_end_char), 1, stdout);
            fwrite(&part2_front_len, sizeof(part2_front_len), 1, stdout);
            //printf("%d", part2_front_len);
            //printf("%c",part2_front_char); 
            fwrite(&part2_front_char, sizeof(part2_front_char), 1, stdout);
        }
        //write(1, src2_p + 5, (nums2 - 2) * 5);
        fwrite(src2_p + 5, sizeof(char), (nums2 - 2) * 5, stdout);
        merge1_length = *(int*)(src2_p + (nums2 -1) * 5);
        merge1_char = *(char*)(src2_p + (nums2 - 1) * 5 + 4);
    }
    else if (nums2 == 1)
    {   
        if (part1_end_char == part2_front_char)
        {
            merge1_char = part1_end_char;
            merge1_length = part1_end_len + part2_front_len;
            //不能直接输出 得考虑三个merge的情况
        }
        else
        {
            fwrite(&part1_end_len, sizeof(part1_end_len), 1, stdout);
            //printf("%d", part1_end_len);
            //printf("%c",part1_end_char); 
            fwrite(&part1_end_char, sizeof(part1_end_char), 1, stdout);
            
            //不能直接输出第二个 因为可能和第三个merge
            
            merge1_char = part2_front_char;
            merge1_length = part2_front_len;
        }
    }
    */


    int  part3_front_len = *(int*)(src3_p);
    char part3_front_char = *(char*)(src3_p + 4);

    data_item1.item_int = rc_data_item.item_int;
    data_item1.item_char = rc_data_item.item_char;
    data_item2.item_int = part3_front_len;
    data_item2.item_char = part3_front_char;
    rc_data_item = inter_merge(data_item1, data_item2, nums3, src3_p);

    /*
    char merge2_char;
    int merge2_length;  

    if (nums3 != 1)
    {
        if (merge1_char == part3_front_char)
        {
            int length = merge1_length + part3_front_len;
            fwrite(&length, sizeof(length), 1, stdout);
            //printf("%d",length);
            //printf("%c",merge1_char);
            fwrite(&merge1_char, sizeof(merge1_char), 1, stdout); 
        }
        else
        {
            fwrite(&merge1_length, sizeof(merge1_length), 1, stdout);
            //printf("%d", merge1_length);
            //printf("%c",merge1_char); 
            fwrite(&merge1_char, sizeof(merge1_char), 1, stdout);
            fwrite(&part3_front_len, sizeof(part3_front_len), 1, stdout);
            //printf("%d", part3_front_len);
            //printf("%c",part3_front_char); 
            fwrite(&part3_front_char, sizeof(part3_front_char), 1, stdout);
        }
        //write(1, src3_p + 5, (nums3 - 2) * 5);
        fwrite(src3_p + 5, sizeof(char), (nums3 - 2) * 5, stdout);
        merge2_length = *(int*)(src3_p + (nums3 -1) * 5);
        merge2_char = *(char*)(src3_p + (nums3 - 1) * 5 + 4);
    }
    else if(nums3 == 1)
    {
        if (merge1_char == part3_front_char)
        {
            merge2_char = merge1_char;
            merge2_length = merge1_length + part3_front_len;
            //不能直接输出 得考虑三个merge的情况
        }
        else
        {
            fwrite(&merge1_length, sizeof(merge1_length), 1, stdout);
            //printf("%d", merge1_length);
            //printf("%c",merge1_char);
            fwrite(&merge1_char, sizeof(merge1_char), 1, stdout); 
            //不能直接输出第二个 因为可能和第三个merge
            merge2_char = part3_front_char;
            merge2_length = part3_front_len;
        }
    }
    */

    int  part4_front_len = *(int*)(src4_p);
    char part4_front_char = *(char*)(src4_p + 4);

    data_item1.item_int = rc_data_item.item_int;
    data_item1.item_char = rc_data_item.item_char;
    data_item2.item_int = part4_front_len;
    data_item2.item_char = part4_front_char;
    rc_data_item = inter_merge(data_item1, data_item2, nums4, src4_p);

    /*
    char merge3_char;
    int merge3_length;  

    if (nums4 != 1)
    {
        if (merge2_char == part4_front_char)
        {
            int length = merge2_length + part4_front_len;
            fwrite(&length, sizeof(length), 1, stdout);
            //printf("%d",length);
            //printf("%c",merge2_char); 
            fwrite(&merge2_char, sizeof(merge2_char), 1, stdout);
        }
        else
        {
            fwrite(&merge2_length, sizeof(merge2_length), 1, stdout);
            //printf("%d",merge2_length);
            //printf("%c",merge2_char);
            fwrite(&merge2_char, sizeof(merge2_char), 1, stdout); 
            fwrite(&part4_front_len, sizeof(part4_front_len), 1, stdout);
            //printf("%d",part4_front_len);
            //printf("%c",part4_front_char);
            fwrite(&part4_front_char, sizeof(part4_front_char), 1, stdout); 
        }
        //write(1, src4_p + 5, (nums4 - 2) * 5);
        fwrite(src4_p + 5, sizeof(char), (nums4 - 2) * 5, stdout);
        merge3_length = *(int*)(src4_p + (nums4 -1) * 5);
        merge3_char = *(char*)(src4_p + (nums4 - 1) * 5 + 4);
    }
    else if (nums4 == 1)
    {
        if (merge2_char == part4_front_char)
        {
            merge3_char = merge2_char;
            merge3_length = merge2_length + part4_front_len;
            //不直接输出 后面一起输出
        }
        else
        {
            fwrite(&merge2_length, sizeof(merge2_length), 1, stdout);
            //printf("%d",merge2_length);
            //printf("%c",merge2_char);
            fwrite(&merge2_char, sizeof(merge2_char), 1, stdout); 
            //不直接输出 后面一起输出
            merge3_char = part4_front_char;
            merge3_length = part4_front_len;
        }
    }
    */

    fwrite(&rc_data_item.item_int, sizeof(rc_data_item.item_int), 1, stdout);
    fwrite(&rc_data_item.item_char, sizeof(rc_data_item.item_char), 1, stdout); 

    /*
    // now ,output all remain
    fwrite(&merge3_length, sizeof(merge3_length), 1, stdout);
    //printf("%d",merge3_length);
    //printf("%c",merge3_char);
    fwrite(&merge3_char, sizeof(char), 1, stdout); 
    */

    /*
    struct stat out_sbuf;
    stat(all_file_name[0], &out_sbuf);
    long out_filesize = out_sbuf.st_size;

    int out_fd = open(all_file_name[0], O_RDONLY, 0);
    char* outp = mmap(0, out_filesize, PROT_READ, MAP_PRIVATE, out_fd, 0);
    close(out_fd);

    write(1, outp, out_filesize);
    */
    //remove("merge.txt");
    return 0;
}
