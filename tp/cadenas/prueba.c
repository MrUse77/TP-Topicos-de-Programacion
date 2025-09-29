#include "cadenas.h"
#include <stdio.h>
#include <string.h>

int main()
{
    char a[] = "Hello, World";
    char g[] = "Hello, World";
    char z[] = "01234abcd";
    char pal[] = "ORO";
    char frase[] = "MARRONAZULROJOOROAMARILLO";
    char b[] = "abcedfghijklmnopqrstuvwxyz";
    char c[] = "0123456789";
    char d[] = "";
    char e[] = "Goodbye ";
    char f[] = "Cruel World";
    char s[] = "123454321";

    char holder[50];

    //printf("%zu %zu %zu\n", stringLenght(c), stringLenght(d), stringLenght(e));
    
    //printf("%d\n", stringNCmp(a, g, 5));
    
    //printf("original: %s copia: %s\n", a, stringNCpy(holder, c, 5));

    //printf("A: %s\nB: %s\nA + B: %s\n", e, f, stringCat(f, e));

    //printf("%s\n", stringRChar(s, '4'));

    //printf("%d\n", stringSpan(e, b));

    //printf("%s\n", stringPbreak(z, c));

    //printf("%s\n", stringSubstring(frase, pal));

    printf("%s\n", stringReverse(b));

    return 0;
}