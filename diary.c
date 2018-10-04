#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#include<string.h>
#include "./get_command_cli.h"


/**
 * 任意の文字列に対して末尾に任意の文字列を付加する。失敗時は第一引数をそのまま返却
 * @param char* target
 * @param char* option_words
 * @return char*
 */
char* add_any_words (char* target, const char* option_words) {
    // 第一引数に対して拡張するべきメモリサイズを取得する
    int first_length = strlen(target);
    int second_length = strlen(option_words);
    // 付与対象の文字列サイズが<0byte>の場合はそのまま第一引数を返却する
    if (second_length == 0) {
        return target;
    }
    // 必要な総byte数は(第一引数のNULL文字を除く長さ + 第二引数のNULL文字を除く長さ + 連結後の文字列に付与するNULL文字分)
    int total_length = first_length + second_length + 1;

    // 連結後の一時ポインタ
    char *temp = NULL;
    printf("<文字の拡張>");
    temp = (char *)realloc(target, total_length * sizeof(char));
    printf("</文字の拡張>");
    if (temp == NULL) {
        printf("reallocに失敗");
        return target;
    }
    if (temp != target) {
        target = temp;
    }
    target = strcat(target, option_words);
    return target;
}

int main() {
    char *todays_comment = NULL;
    int length = 0; // 一行分の文章の char型要素数
    int total_length = 0; // 入力テキスト全char型要素数
    char *diary = NULL;
    diary = (char *)malloc(1 * sizeof(char));
    if (diary == NULL) {
        exit(255);
    }
    *diary = 0;
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
    int k = 0;
    for (;;) {
        todays_comment = get_command_line();

        if (strcmp(todays_comment, "end") == 0) {
            printf("今日の出来事は次のような内容でよろしいですか?\r\n");
            // 現在入力されたbyte数マックスまでを出力
            printf("%s", diary);
            // 入力を終了する場合
            printf("put any key y or n\r\n");
            todays_comment = get_command_line();
            if (strcmp(todays_comment, "y") == 0) {

                break;
            } else {
                continue;
            }
        }
        // 一行入力したら入力した文字列に対して改行文字を付与する
        printf("コマンドラインからの戻り値<%s>", todays_comment);
        length = strlen(todays_comment);
        if (length == 0) {
            continue;
        }
        todays_comment = add_any_words(todays_comment, "\n");
        length = strlen(todays_comment);
        total_length = total_length + length + 1;
        temp = (char *)realloc(diary, total_length * sizeof(char));
        if (temp == NULL) {
            printf("realloc失敗");
            exit(255);
        }
        if (temp != diary) {
            diary = temp;
        }
        // 現在のオフセット位置から新しい文章をコピーする
        strcat(diary, todays_comment);
        free(todays_comment);
    }

    fwrite(diary, strlen(diary), 1, fp);
    fclose(fp);
    return 0;
}

