


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <string>
#include <iostream>

typedef struct MyString {
    char *str;
} MyString;

using namespace std;
unsigned char *get_command_line(unsigned char *);
char *get_command_line_02(void);

int main()
{
    MyString *myString_ptr = (MyString *)malloc(sizeof(MyString) * 1);
    std::string c_string = "C言語の文字列である。";

    cout << c_string << endl;
    printf("%s", c_string.c_str());
    FILE *fp = NULL;
    char *str;

    while (1) {
        std::string inner_string = "";
        fp = fopen("./message_you_inputed.dat", "a");
        if (fp == NULL) {
            printf("任意のファイルオープンに失敗しました。");
            exit(255);
        }
        str = get_command_line_02();
        myString_ptr->str = str;
        inner_string = std::string(str);
        printf("あなたが入力した文字列を、自身で定義した構造体で出力[%s]", myString_ptr->str);
        printf("あなたが入力した文字列は => (%s)", str);
        printf("%d", strlen(str));
        printf("%d", fwrite(str, strlen(str), 1, fp));


        free(str);
        printf("あなたが入力した文字列を、自身で定義した構造体で出力[%s]", myString_ptr->str);
        printf("%s", str);
        printf("\n");
        fclose(fp);
    }
    return (0);
}

char* get_command_line_02() {
    char* inner_str = (char *)malloc(1 * sizeof(char));
    if (inner_str == NULL) {
        printf("メモリの確保に失敗しました。");
        exit(255);
    }
    char *temp_str = NULL;
    char read;
    int i = 1; // すでに1byte確保している
    while (read = getc(stdin)) {
        printf("%c", read);
        if (read == '\n')
        {
            inner_str[i - 1] = 0;
            return (inner_str);
        }
        inner_str[i - 1] = read;
        temp_str = (char*)realloc(inner_str, (++i) * sizeof(char));
        if (temp_str == NULL) {
            printf("メモリの拡充に失敗しました。");
            exit(255);
        }
        if (temp_str != inner_str) {
            inner_str = temp_str;
        }
    }
}
unsigned char* get_command_line (unsigned char* cp) {
    // コマンドラインから取得した1バイト分の文字
    unsigned char c ;
    // メモリ拡張時の一時的アドレス
    unsigned char* t_cp;
    int slen = 0;
    int index = 0;
    while(c = getchar()) {
        slen++;
        t_cp = (unsigned char*)realloc(cp, sizeof(unsigned char) * slen);
        // メモリ確保失敗時
        if (t_cp == NULL) {
            free(cp);
            return(NULL);
        }
        if (feof(stdin) == 1) {
            printf("EOFを検出");
        }
        // アドレスが変化した場合
        if (cp != t_cp) {
            cp = t_cp;
        }
        if (c == '\n') {
            *(cp + (index)) =  0;
            slen = 0;
            index = 0;
            //*(cp) = '\0';
            return(cp);
        }
        cp[index] = c;
        //*(cp + index) = c;
        index++;
    }
}

