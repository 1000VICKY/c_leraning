#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define S 128




char* get_command_line () {
    // 戻り値用文字列
    char *input = NULL;
    // 関数内でのメモリ確保用一時文字列
    char *temp = NULL;
    char c = 0;

    // 事前にchar型のメモリを128byte分確保しておく
    int size = S;
    input = (char *)malloc(size * sizeof(char));
    if (input == NULL) {
        printf("メモリ確保に失敗");
        return "";
    }

    int index = 0;
    while(c = getchar()) {
        // indexがメモリの範囲を超えるようであればreallco();
        if (index == size) {
            printf("<メモリ拡張>");
            size = size + S;
            printf("定数S<%d>", size);
            // 規定バイトである128byteずつ大きくしていく
            temp = (char *)realloc(input, size * sizeof(char));
            if (temp == NULL) {
                printf("メモリの拡充に失敗しました");
                exit(255);
            }
            if (temp != input ) {
                //ポインタ変数を入れ直す
                input = temp;
                temp = NULL;
            }
        }
        // 改行文字が来たら文字入力終了
        if (c == '\n'){
            input[index] = 0;
            break;
        }
        input[index] = c;
        // 書き込み対象を1byteずつシフト
        index++;
    }
    // 余分なメモリをカットする
    temp = (char *)realloc(input, (strlen(input) + 1) * sizeof(char));
    if (temp == NULL) {
        printf("メモリのダイエットに失敗");
        return input;
    }
    if (temp != input) {
        input = temp;
    }
    return (input);
}