#include "cadenas.h"

int stringCmp(char* dest, char* src)
{
    char* tmpD = dest;
    char* tmpS = src;

    while(!(*tmpD - *tmpS) && *tmpD && *tmpS){
        tmpD++; 
        tmpS++;        
    }

    return (*tmpD - *tmpS);
}

int stringNCmp(char* dest, char* src, size_t n)
{
    char* tmpD = dest;
    char* tmpS = src;

    while(!(*tmpD - *tmpS) && *tmpD && *tmpS && n){
        tmpD++;
        tmpS++;
    }

    return (*tmpD - *tmpS);
}

int stringCmpi(char* dest, char* src)
{
    char* tmpD = dest;
    char* tmpS = src;

    while(!(tolower(*tmpD) - tolower(*tmpS) && *tmpD && *tmpS)){
        tmpD++;
        tmpS++;
    }

    return (tolower(*tmpD) - tolower(*tmpS));
}

int stringNCmpi(char* dest, char* src, size_t n)
{
    char* tmpD = dest;
    char* tmpS = src;

    while(!(tolower(*tmpD) - tolower(*tmpS)) && *tmpD && *tmpS && n){
        tmpD++;
        tmpS++;
    }

    return (tolower(*tmpD) - tolower(*tmpS));
}

char* stringCpy(char* dest, char* src)
{
    char* tmpD = dest;
    char* tmpS = src;

    while(*tmpS){
        *tmpD = *tmpS;
        tmpD++;
        tmpS++;
    }

    *tmpD = '\0';

    return dest;
}

char* stringNCpy(char* dest, char* src, size_t n)
{
    char* tmpD = dest;
    char* tmpS = src;

    while(*tmpS && n){
        *tmpD = *tmpS;
        tmpD++;
        tmpS++;
        n--;
    }

    *tmpD = '\0';

    return dest;
}

char* stringDup(char* str)
{
    char* tmp = malloc(strlen(str));

    if(!tmp){
        return NULL;
    }

    stringCpy(tmp, str);

    return tmp;
}

char* stringNDup(char* str, size_t n)
{
    if(n < 0){
        return NULL;
    }

    size_t len = stringLenght(str);

    if(len < n){
        return NULL;
    }

    char* tmp = malloc((len - n));

    if(!tmp){
        return NULL;
    }

    stringNCpy(tmp, str, n);

    return tmp;
}

char* stringCat(char* dest, char* src)
{
    char* tmpD = dest + stringLenght(dest) - 1;
    char* tmpS = src;

    while(*tmpS){
        *tmpD = *tmpS;
        tmpD++;
        tmpS++;
    }

    *tmpD = '\0';

    return dest;
}

char* stringNCat(char* dest, char* src, size_t n)
{
    char* tmpD = dest + stringLenght(dest) - 1;
    char* tmpS = src;

    while(*tmpS && n){
        *tmpD = *tmpS;
        tmpD++;
        tmpS++;
        n--;
    }

    *tmpD = '\0';

    return dest;
}

char* stringToUpper(char* str)
{
    char* tmp = str;

    while(*tmp){
        *tmp = toupper(*tmp);
        tmp++;
    }

    return str;
}

char* stringToLower(char* str)
{
    char* tmp = str;

    while(*tmp){
        *tmp = tolower(*tmp);
        tmp++;
    }

    return str;
}

char* stringReverse(char* str)
{
    char* i = str;
    char* j = str + stringLenght(str) - 2; // -1 es '\0'
    char tmp;

    while(i <= j){
        tmp = *i;
        *i = *j;
        *j = tmp;
        i++;
        j--;
    }

    return str;
}

char* stringChar(char* dest, int ch)
{
    char* tmpD = dest;

    while(*tmpD){
        if(*tmpD == ch){
            return tmpD;
        }

        tmpD++;
    }

    return NULL;
}

char* stringRChar(char* dest, int ch)
{
    char* tmpD = dest + stringLenght(dest) - 1;

    while(tmpD != dest){
        if(*tmpD == ch){
            return tmpD;
        }

        tmpD--;
    }

    return NULL;
}

// dest = "1234ABCD" src = "ABCDEFGH"
// stringCSpan(dest, src) = 4
int stringCSpan(char* dest, char* src)
{
    char* tmpD = dest;
    char* tmpS = src;
    int sum = 0;

    while(*tmpD){
        while(*tmpS != *tmpD && *tmpS){
            tmpS++;
        }

        if(*tmpS == *tmpD){
            return sum;
        }

        sum++;
        tmpD++;
        tmpS = src;
    }

    return sum;
}


// dest = "1234ABCD" src = "123456789"
// stringSpan(dest, src) = 4
int stringSpan(char* dest, char* src)
{
    char* tmpD = dest;
    char* tmpS = src;
    int sum = 0;

    while(*tmpD){
        while(*tmpS != *tmpD && *tmpS){
            tmpS++;
        }

        if(!(*tmpS)){
            return sum;
        }

        sum++;
        tmpD++;
        tmpS = src;
    }

    return sum;
}

char* stringPbreak(char* dest, char* src)
{
    char* tmpD = dest;
    size_t len = stringLenght(dest) - 1;
    int cspan = stringCSpan(dest, src);

    if(cspan == len){
        return NULL;
    }else{
        tmpD += cspan;
    }

    return tmpD;
}

char* stringSubstring(char* dest, char* src)
{
    char* tmpD = dest;
    char* tmpS = src;
    char* tmpAux = dest;

    while(*tmpD){

        while(*tmpAux == *tmpS && *tmpAux && *tmpS){
            tmpAux++;
            tmpS++;
        }

        if(!(*tmpS)){
            return tmpD;
        }

        tmpD++;
        tmpAux = tmpD;
        tmpS = src;
    }

    return NULL;
}

size_t stringLenght(char* str)
{
    char* tmp = str;
    size_t sum = 0;

    while(*tmp){
        sum++;
        tmp++;
    }

    return ++sum;
}
