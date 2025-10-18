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

char* stringCopy(char* dest, char* src)
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

char* stringNCopy(char* dest, char* src, size_t n)
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

    stringCopy(tmp, str);

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

    stringNCopy(tmp, str, n);

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

char* stringRemove(char* str, int ch)
{
    char* i = str;
    int len = stringLenght(str);

    while(*i){
        if(*i == ch){
            memmove(i, i + 1, len - (i - str));
        }

        i++;
    }

    return str;
}

char* stringReplace(char* str, int ori, int new)
{
    char* i = str;

    while(*i){
        if(*i == ori){
            *i = new;
        }
        i++;
    }

    return str;
}

char* stringTrim(char* dest, char* src, int i, int j)
{
    char* d = dest;
    char* s = src + i;

    while(i < j && *s){
        *d = *s;
        d++;
        s++;
        i++;
    }

    *d = '\0';

    return dest;
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

char* stringPBreak(char* dest, char* src)
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

inline int stringLenght(const char* str)
{
    const char* tmp = str;
    size_t sum = 0;

    while(*tmp){
        ++sum;
        ++tmp;
    }

    return ++sum;
}


int memzero(void* ptr, size_t n)
{
    char* tmp = ptr;

    while(n){
        *tmp = 0;
        tmp++;
    }

    return 0;
}

char* scanString(char* str, size_t n)
{
    if(n <= 0 || !str){
        return NULL;
    } 

    if(fgets(str, n, stdin) == NULL){
        return NULL;
    }

    str[stringCSpan(str, "\n")] = '\0';

    return str;
}

void flushStdin(void)
{
    int c;

    while((c = getchar()) != '\n' && c != EOF);
}