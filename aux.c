#include <inttypes.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


int main(void) 
{
    // uint64_t valoare;
    // valoare = 1;
    // printf("%lu\n", sizeof(uint64_t));
    // printf("%ld", valoare);
    int a, b, c, d;
    scanf("%d %d %d %d", &a, &b, &c, &d);
    if (d < a || c > b) {
        printf("0\n");
    } else {
        printf("1\n");
    }
    return 0;
}