


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// フィボナッチ数列の関数
unsigned int fib (int);
int main () {

    int any_number = 50;
    int selected_number = 0;
    selected_number = fib(any_number);
    printf("あなたが指定したフィボナッチ数列の値は[%d]", selected_number);
}

unsigned int fib (int any_number) {
    int index = 3;
    if (any_number < 3) {
        return (0);
    }
    unsigned int first = 0;
    unsigned int second = 1;
    unsigned int result = 0;
    while (1){
        result = first + second;
        first = second;
        second = result;
        if (index == any_number) {
            break;
        }
        index++;
    }
    return (result);
}
