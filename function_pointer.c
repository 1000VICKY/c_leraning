
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// 関数ポインタの型を定義する
typedef char* (*ConnectedString)(char*, char*);
char *connected_string(char*, char*);


int main()
{
    // 事前宣言したポインタ関数の変数を宣言
    ConnectedString cdptr = NULL;
    cdptr = connected_string;
    if (cdptr != NULL) {
        char* res = cdptr("文字列の", "連結をこの関数を用いて行います.");
        printf("あなたが指定した文字列の連結結果は => %s", res);
        free(res);
        res = cdptr("別の文字列を今度は連結します!", "いかがでしょうか?");
        printf("あなたが指定した文字列の連結結果は => %s", res);
        free(res);
    } else {
        printf("NULL Pointer Exception!");
        exit(255);
    }
}

char* connected_string(char* a_string, char* b_string) {
    //引数に渡された文字列分のメモリを確保する
    int total_length = strlen(a_string) + strlen(b_string) + 1;
    //引数2つ分のバイト数を確保
    char *connected_string = (char *)malloc(sizeof(char) * total_length);
    strcpy(connected_string, a_string);
    char *temp = &connected_string[strlen(a_string)];
    strcpy(temp, b_string);

    return (connected_string);
}
