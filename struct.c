


#include <stdio.h>
#include <stdlib.h>
#include <string.h>



typedef struct MyType {

    int age;
    int weight;
    int height;
    char* name;
} MyType;
int main () {

    MyType* mytype_p = NULL;
    mytype_p = (MyType*)malloc(sizeof(MyType));
    printf("%d", sizeof(MyType));

    mytype_p->name = (char*)calloc(128, sizeof(char));
    *mytype_p->name = 'F';
    printf("%s", mytype_p->name);
    printf("\n");
    printf("%p", mytype_p);
    printf("\n");
    printf("%p", mytype_p->name);
    printf("\n");
    strcpy(mytype_p->name, "こんばんわあああああああああああああああああああああああああああああああああああああ");
    printf("\n");
    printf("%s", mytype_p->name);
    free(mytype_p->name);
    free(mytype_p);



    unsigned long f1 = 0;
    unsigned long f2 = 1;
    unsigned long f_now = 0;
    printf("%d", f1);
    printf("%d", f2);
    long i;
    for (i = 1 ; i <= 100; i++) {
        f_now = f1 + f2;
        printf("\n");
        printf("%ld", f_now);
        f1 = f2;
        f2 = f_now;
    }
    printf("\n");
    char* ss = "文字列をポインタでループさせて出力させる。";
    printf("%c", ss[5]);
    int ii;
    for ( ii= 0; ii < (int)strlen(ss); ii++) {
        printf("%c", ss[ii]);
        printf("\r\n");
    }
    char* sss = (char*)malloc(sizeof(char) * strlen(ss) + 1);
    strcpy(sss, ss);
    sss[10] = 'A';
    printf("%s", sss);
}
//2971215073
//2118290601
//2147483647
//18446744073709551615
//7540113804746346429