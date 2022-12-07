#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void menu();

int main() {
    menu();
    return 0;
}

void menu() {
    struct tm* ptr;
    time_t t;
    t = time(NULL);
    ptr = localtime(&t);
    printf("System: \n");
    printf("%s\n", asctime(ptr));
    printf("list");
}