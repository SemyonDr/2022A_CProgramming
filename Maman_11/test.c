#include <stdio.h>


int main() {
    char str[100];

    printf("please enter string:\n");
    fgets(str, 100, stdin);
    printf("Your string is: \n%s\n",str);

    return 0;
}