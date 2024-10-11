#include <stdio.h>


struct MyMessageUnpacked
{
    int a;
    char b;
    int c;
    char d;
};


#pragma pack(1)
struct MyMessagePacked
{
    int a;
    char b;
    int c;
    char d;
};

#pragma pack()


int main()
{
    printf(
        "정렬된 구조체 크기: %d byte\n",
        sizeof(struct MyMessageUnpacked)
    );
    printf(
        "정렬되지 않은 구조체 크기: %d byte\n",
        sizeof(struct MyMessagePacked)
    );
    return 0;
}