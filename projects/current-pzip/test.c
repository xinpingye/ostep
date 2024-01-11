#include <unistd.h>
#include <stdio.h>

int main()
{
    char name[] = "nameisyxp";
    write(1, name, 0);
    return 0;
}