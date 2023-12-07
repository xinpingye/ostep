#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    if  (argc == 1)
    {
        printf("need one argumen\n");
        exit(1);
    }
    int nums = atoi(argv[1]);
    printf("nums:%d\n",nums);
    int * addr = (int*)malloc(nums * 1024 * 1024);
    printf("addr:%p\n",addr);
    printf("&nums:%p\n",&nums);
    while(1)
    {
        int result = 0;
        for (int i = 0; i < nums * 1024 * 1024 / 4; i++)
        {
            //printf("%d\n",addr[i]);
            result += addr[i] + 1; 
        }
    }
    return 0;
}