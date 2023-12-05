#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
    char str[] = "2,张三,89,99,66";
    // char str[] = ",,2,张三,89,99,66";
    // str是一个指针常量，而strsep的第一个参数需要一个指向指针的指针，所以不能对str做取地址操作，这里再定义一个指针变量就可以取地址操作了。否则会出现段错误。
    char *strv = str;
    char *token = strsep(&strv, ",");
    while(token != NULL){
        printf("%s\t", token);
        token = strsep(&strv, ",");
    }
    printf("\n");
    return 0;
}