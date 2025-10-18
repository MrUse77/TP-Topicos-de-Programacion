#include "graphics.h"

typedef struct Form_t{
    const char title[];
    Field_t* fields;
    int fieldNum;
}Form_t;