

#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#include<string.h>
#include "./get_command_cli.h"


// 任意の文字列に対して末尾に任意の文字列を付加する
char* add_any_words (char* target, const char* option_words) {
    // 第一引数に対して拡張するべきメモリサイズを取得する
    int first_length = strlen(target);
    int second_length = strlen(option_words);
    // 必要な総byte数は(第一引数のNULL文字を除く長さ + 第二引数のNULL文字を除く長さ + 連結後の文字列に付与するNULL文字分)
    int total_length = first_length + second_length + 1;

    // 連結後の
    char *temp = NULL;
    temp = (char *)realloc(target, total_length * sizeof(char));
    if (temp == NULL) {
        printf("reallocに失敗");
        exit(255);
    }
    if (temp != target) {
        target = temp;
    }
    strcat(target, option_words);
    //strcpy(target + first_length, option_words);
    return (target);
}

int main() {
    char *todays_comment = NULL;
    int length = 0; // 一行分の文章の char型要素数
    int total_length = 0; // 入力テキスト全char型要素数
    char *diary = NULL;
    char *temp = NULL;
    char filename[15];
    time_t t = time(NULL);
    strftime(filename, sizeof(filename), "%Y-%m-%d.dat", localtime(&t));
    printf("%s", filename);
    FILE *fp = NULL;
    if ((fp = fopen(filename, "a")) == NULL) {
        printf("%sのファイルオープンに失敗");
        exit(255);
    }
    printf("今日の出来事を入力してください.");
    int index = 0;
    for (;;) {
        todays_comment = get_string_from_cli();

        if (strcmp(todays_comment, "end") == 0) {
            printf("今日の出来事は次のような内容でよろしいですか?");
            // 現在入力されたbyte数マックスまでを出力
            int k = 0;
            for (k = 0; k <= index; k++) {
                printf("%c", diary[k]);
            }
            // 入力を終了する場合
            printf("put any key y or n");
            todays_comment = get_string_from_cli();
            if (strcmp(todays_comment, "y") == 0) {
                // 終了する場合は1byte分メモリを拡張して，NULLバイトを詰める
                temp = (char*)realloc(diary, (total_length + 1) * sizeof(char));
                if (temp == NULL) {
                    printf("reallocに失敗");
                    exit(255);
                }
                if (temp != diary) {
                    diary = temp;
                }
                // NULL文字を詰める
                diary[index] = 0;
                //diary = add_any_words(diary, "\n");
                break;
            } else {
                continue;
            }
        }
        // 一行入力したら入力した文字列に対して改行文字を付与する
        todays_comment = add_any_words(todays_comment, "\r\n");
        length = strlen(todays_comment);
        if (length == 0) {
            continue;
        }
        total_length = total_length + length;
        temp = (char *)realloc(diary, total_length * sizeof(char));
        if (temp == NULL) {
            printf("realloc失敗");
            exit(255);
        }
        if (temp != diary) {
            diary = temp;
        }
        // 入力した文字列の末尾のNULL文字が都合が悪いので
        // 切り詰める
        temp = realloc(todays_comment, length);
        if (temp == NULL) {
            printf("strcpy前処理に失敗しました.");
            exit(255);
        }
        todays_comment = NULL;
        // 現在のオフセット位置から新しい文章をコピーする
        strcpy(diary + index, temp);
        index = index + length;
        free(temp);
        free(todays_comment);
    }

    fwrite(diary, strlen(diary), 1, fp);
    fclose(fp);
    return 0;
}

