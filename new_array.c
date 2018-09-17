#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define S 128


char* get_string_from_cli () {
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
            size = size + S;
            // 規定バイトである128byteずつ大きくしていく
            temp = (char *)realloc(input, size * sizeof(char));
            if (temp == NULL) {
                printf("メモリの拡充に失敗しました");
                exit(255);
            }
            if (temp != input ) {
                //ポインタ変数を入れ直す
                input = temp;
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
    return input;
}

// 雇用者構造体
typedef struct employee {
    char *name;
    char *address;
    int age;
    int salary;
} Employee;

// 雇用者オブジェクトを作成するための関数
Employee* make_employee(char* name, char* address, int age, int salary ) {
    // 関数内で Employee型オブジェクトを作成
    Employee *e = (Employee *)malloc(1 * sizeof(Employee));
    e->name = name;
    e->address = address;
    e->age = age;
    e->salary = salary;
    // Employee型のアドレスを返却
    return e;
}

// Employee型を解放する
void free_employee(Employee* e) {
    free(e->name);
    free(e->address);
    free(e);
}

int main()
{
    // 会社の人数を保持
    int number_of_empoyees = 0;
    // 非雇用者の配列を生成
    Employee **staff_list = NULL;
    // コマンドラインからの入力
    char *got_info = NULL;

    printf("会社の社員数を入力 >>>");
    got_info = get_string_from_cli();
    // コマンドラインから取得した文字列を10進基数に変換
    number_of_empoyees = strtol(got_info, NULL, 10);
    free(got_info);
    got_info = NULL;
    if (number_of_empoyees < 1)
    {
        printf("不正な人数でした");
        exit(255);
    }
    // 入力した人数分の非雇用者構造体の配列を作成
    // 入力した人数分の雇用者構造体型のポインタ配列を作成
    staff_list = (Employee **)malloc(number_of_empoyees * sizeof(Employee *));
    if (staff_list == NULL) {
        printf("被雇用者構造体のメモリ確保ができませんでした ");
        exit(255);
    }

    // コマンドラインからの入力を確保 & 社員情報の登録
    char *ensure_name;
    char *ensure_address;
    long *ensure_age = (long*)malloc(1 * sizeof(long));
    if (ensure_age == NULL) {
        printf("年齢確保用のメモリ確保に失敗しました.");
        exit(255);
    }
    long *ensure_salary = (long *)malloc(1 * sizeof(long));
    if (ensure_salary == NULL) {
        printf("年収確保用のメモリ確保に失敗しました.");
        exit(255);
    }
    int number;
    int i;
    for (i = 0; i < number_of_empoyees; i++)
    {
        printf("%d番目のスタッフ:名前を入力してください\r\n>>>", i);
        ensure_name = get_string_from_cli();
        printf("%d番目のスタッフ:住所を入力してください\r\n>>>", i);
        ensure_address = get_string_from_cli();
        printf("%d番目のスタッフ:年齢を入力してください\r\n>>>", i);
        got_info = get_string_from_cli();
        *ensure_age = strtol(got_info, NULL, 10);
        if (*ensure_age > 17 && *ensure_age < 61)
        {
        } else {
            printf("採用条件を満たしておりません。");
            printf("代替値を入力します");
            *ensure_age = 18;
        }
        printf("%d番目のスタッフ:給与を入力してください\r\n>>>", i);
        got_info = get_string_from_cli();
        *ensure_salary = strtol(got_info, NULL, 10);
        if (*ensure_salary < 1000000) {
        } else {
            printf("それ以上は支給できません");
            printf("代替値を入力します");
            *ensure_salary = 300000;
        }
        // 入力した内容をもって社員オブジェクトを作成
        staff_list[i] = make_employee(ensure_name, ensure_address, *ensure_age, *ensure_salary);
    }
    // 社員情報の登録終了 & 社員情報の参照を行う
    printf("閲覧したいスタッフの番号(%dから %dまでの間)を入力\r\n>>>", 0, number_of_empoyees - 1);
    int staff_number = 0;
    while (1)
    {
        printf("スタッフの番号 >>>");
        got_info = get_string_from_cli();
        // 入力した内容がexitであればアプリケーションの終了
        if (strcmp(got_info, "exit") == 0) {
            printf("終了します");
            // ヒープにあるメモリをすべて解放する
            free(got_info);
            got_info = NULL;
            for (i = 0; i < number_of_empoyees; i++) {
                free_employee(staff_list[i]);
            }
            exit(255);
        }
        staff_number = strtol(got_info, NULL, 10);
        if (staff_number >= 0 && staff_number < number_of_empoyees) {
            printf("お名前:%s", staff_list[staff_number]->name);
            printf("住所:%s", staff_list[staff_number]->address);
            printf("年齢:%d", staff_list[staff_number]->age);
            printf("給与:%d円/月", staff_list[staff_number]->salary);
        } else {
            printf("存在しないスタッフの番号です");
        }
    }
}