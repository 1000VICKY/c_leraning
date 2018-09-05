

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    char* file_name = "movie.mp4";

    FILE* fp = NULL;

    fp = fopen(file_name, "r");
    if (fp == NULL) {
        printf("%sのファイルオープンに失敗しました。",  file_name);
    }
    unsigned char read[1024];
    size_t read_size;
    int max_size = 0;


    time_t start  = time(NULL);
    printf("%ld", start);
    while (1) {
        read_size = fread (read, 1, 1024, fp);
        if (read_size == 0) {
            break;
        }
        max_size += (int)read_size;
        //printf("読み込んだ合計バイト数=>%d", max_size);
    }
    time_t end = time(NULL);
    printf("ファイルの読み取りにかかった時間 : %d", (int) end - (int) start);
    unsigned char* to_write;
    to_write = (unsigned char*)malloc(max_size);
    rewind(fp);
    if (fread(to_write, max_size, 1, fp) == 1) {

        char* new_file = "new_movie.mp4";
        fp = fopen(new_file, "w");
        if (fwrite(to_write, max_size, 1, fp) == 1) {
            fclose(fp);
    start =  time(NULL);
    printf("%ld", start);
            printf("ファイルのコピーが完了");
    int cli_read = 0;
    char cli_char =0;
    while ( cli_char = getchar()) {
        if (cli_char == EOF) {
            printf("読み取り完了");
        }
        printf("%d", cli_char);
    }
            exit(0);
        } else {
            printf("ファイルの書き込みに失敗");
            exit(255);
        }

    } else {
        printf("ファイルポインタからのバイトの読み込みに失敗しました");
        exit(255);
    }
}