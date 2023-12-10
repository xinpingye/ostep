#include <stdio.h>

int main()
{
    int i = 0;
    int* p = &i;
    p = NULL;
    int tmp = *p;
    printf("tmp:%d\n",tmp);
    return 0;
}