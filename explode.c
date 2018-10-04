


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    temp = (char *)realloc(target, total_length * sizeof(char));
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

/**
 * 指定した文字列に任意の文字列が現れた最初のメモリアドレスを返却する
 * @param char* str
 * @param char* needle
 * @return char*
 */
char* checkByteSequence(char* str, char* needle) {
    // 本関数の戻り値
    char *return_value = NULL;
    int first_length = strlen(str);
    int second_length = strlen(needle);
    // 検査される被文字列よりも対象文字列のbyte数が大きい場合
    if (first_length < second_length) {
        return NULL;
    }
    int i;
    int match_count = 0;
    for (i = 0; i < first_length; i++)
    {
        // 非文字列のiバイト目とターゲット文字列のmatch_countバイト目がマッチした場合
        if (str[i] == needle[match_count]) {
            // 初回マッチ時に当該byteのアドレスを記憶
            if (return_value == NULL) {
                return_value = str + i;
            }
            // ターゲット文字列の全バイトがマッチした場合
            if (match_count == second_length - 1) {
                return return_value;
            }
            match_count++;
        } else {
            // 1byteでもマッチしなかったら戻り値をNULLにもどす
            return_value = NULL;
            match_count = 0;
        }
    }
    return return_value;
}

/**
 * 指定した文字列で任意の文字列を区切って配列に置換する。かつ第三引数のint型参照に
 * 配列のサイズを保持させる。
 * @param char* target
 * @param char* needle
 * @param int* count
 * @return char**
 */
char** explode(char* target, char* needle, int* count) {
    *count = 0;
    char* remember_memory_start = target;
    char** return_value = NULL;
    char* matched_address = NULL;
    char* dummy_str = NULL;
    while (1)
    {
        matched_address = checkByteSequence(target, needle);
        // 演算費文字列に対象文字列が見つからない場合
        if (matched_address == NULL) {
            if (strlen(target) > 0) {
                (*count)++;
                return_value = (char**)realloc(return_value, *count * sizeof(char *));
                if (return_value == NULL) {
                    return NULL;
                }
                dummy_str = (char*) malloc( (strlen(target) + 1) * sizeof(char)) ;
                if (dummy_str == NULL) {
                    return NULL;
                }
                strcpy(dummy_str, target);
                return_value[*count - 1] = dummy_str;
            }
            break;
        }
        (*count)++;
        return_value = (char**)realloc(return_value, *count * sizeof(char *));
        if (return_value == NULL) {
            return NULL;
        }
        *matched_address = 0;
        dummy_str = (char*) malloc( (strlen(target) + 1) * sizeof(char)) ;
        if (dummy_str == NULL) {
            return NULL;
        }
        strcpy(dummy_str, target);
        return_value[*count - 1] = dummy_str;
        target = matched_address +  strlen(needle);
    }
    free(remember_memory_start);
    return return_value;
}

int main() {

    // 被分割文字列
    char str[] = "こんばんわ、僕の名前はヤンマー";
    char * duplication = (char*)malloc(( strlen(str) + 1) * sizeof(char));
    strcpy(duplication, str);
    char** str_list = NULL;
    int length_ptr = NULL;
    str_list = explode(duplication, "、", &length_ptr);
    int i;
    for( i =0; i < length_ptr ; i++) {
        str_list[i] = add_any_words(str_list[i], " ");
        printf("<%s>\r\n", str_list[i]);
    }
    return 0;
}
