

#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "get_command_cli.h"


/**
 * 疑似添字配列を作成
 */
typedef struct dummy_array{
    char **value; // value => 値
    int size; // 配列のサイズ
    char* (*getValue)(struct dummy_array*, int); // 構造体に関数ポインタ
} DummyArray;

/**
 * 第一引数に渡した配列オブジェクトに対して任意のindexの値を取得する
 * indexは構造体メンバのsizeの値を満たすものとする
 * @param DummyArray* array
 * @param int index
 * @return char*
 */
char* getValue (DummyArray* array, int index) {
    if (array == NULL) {
        return NULL;
    }
    // 配列のインデックス 0 <= index < strlen(array)
    if (index >= 0 && index < array->size) {
        return (array->value[index]);
    }
}

/**
 * 指定したサイズの大きさを持つindex配列を生成する
 * @param int size
 * @return DummyArray*
 */
DummyArray* makeArray (int size) {
    DummyArray *array = NULL;
    int make_size = 1;
    // ひとつ分のDummyArrayオブジェクトのメモリを確保する
    array = malloc(make_size * sizeof(DummyArray));
    if (array == NULL) {
        return NULL;
    }
    // 第一引数に渡されたint型変数分の大きさをもつchar**型のメモリを確保
    array->value = (char **)malloc(size * sizeof(char *));
    if (array->value == NULL) {
        return NULL;
    }
    array->getValue = getValue;
    array->size = size;
    return array;
}

/**
 * 第一引数に渡されたDummyArrayオブジェクトのvalueメンバのサイズを1つ分拡張する
 * @param DummyArray* array
 * @return DummyArray
 */
DummyArray* extendArray (DummyArray* array) {
    char **temp;
    temp = (char **)realloc(array->value, (array->size + 1) * sizeof(char *));
    if (temp == NULL) {
        return array;
    }
    if (temp != array->value) {
        array->value = temp;
    }
    // 忘れずに配列のサイズを加算する
    array->size++;
    return array;
}

/**
 * 第一引数に渡したDummyArrayオブジェクトの最初のNULLの値に第二引数のtargetをpushする
 * @param DummyArray* array
 * @param char* target
 * @return int
 */
int appendValue (DummyArray* array, char* target) {
    int index;
    for (index = 0; index < array->size; index++) {
        if (array->value[index] == NULL) {
            array->value[index] = target;
            return 1;
        }
    }
    return 0;
}


int main()
{
    DummyArray* array;
    char *input = NULL;
    int array_index = 0;
    printf("初期配列のサイズを入力してください >>>\r\n");
    input = get_command_line();
    // 入力された文字列をint型へキャスト
    array_index = atoi(input);
    if (array_index == 0) {
        return 1;
    }
    // DummyArray配列を生成
    array = makeArray(array_index);

    int i = 0;
    printf("[0]から[%d]までの配列の値を入力してください >>>\r\n", array_index - 1);
    for (i = 0; i < array->size; i++)
    {
        printf("[%d]番目 >>>\r\n", i);
        input = get_command_line();
        if (appendValue(array, input) != 1 ) {
            printf("値のPushに失敗しました");
            exit(255);
        }
    }
    // すべての値を入力したあと,配列のサイズを拡張してみる
    array = extendArray(array);
    appendValue(array, "配列を拡張して、値を追加しました。");
    printf("DummyArrayオブジェクトが保持している値一覧を取得\r\n");
    for (i = 0; i < array->size; i++)
    {
        printf("%s\r\n", array->getValue(array, i));
    }
}
