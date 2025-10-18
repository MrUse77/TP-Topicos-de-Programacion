#ifndef GRAPHICS_H_INCLUDED
#define GRAPHICS_H_INCLUDED

#define COLOR_RESET   "\033[0m"
#define COLOR_BOLD    "\033[1m"
#define COLOR_DIM     "\033[2m"
#define COLOR_RED     "\033[1;31m"
#define COLOR_YELLOW  "\033[1;33m"
#define COLOR_CYAN    "\033[1;36m"
#define COLOR_GRAY    "\033[0;37m"

#define CLEAR_SCREEN printf("\033[H\033[J")

#define PRINT_BIG_PRETTY(txt) do{\
        printf("+%.*s+\n", FORMULARIO_TITULO_LEN + 2, "--------------------------------");\
        printf("| %s |\n", (txt));\
        printf("+%.*s+\n", FORMULARIO_TITULO_LEN + 2, "--------------------------------");\
        printf("\n");\
    }while(0)

#define PRINT_SMALL_PRETTY(txt) \
    do{ \
        printf("+%.*s+\n", strlen(txt) + 2, "--------------------------------");\
        printf("| %s |\n", (txt));\
        printf("+%.*s+\n", strlen(txt) + 2, "--------------------------------");\
        printf("\n");\
    }while(0)

#define PRINT_HINT_PRETTY(txt) \
    do{ \
        printf(COLOR_DIM "Hint: %s" COLOR_RESET "\n", (txt));\
    }while(0)

#define PRINT_TEXT_BOX_PRETTY(txt) \
    do{ \
        printf(COLOR_GRAY "+%.*s+\n", strlen(txt) + 2, "--------------------------------");\
        printf("| %s |\n", (txt));\
        printf("+%.*s+" COLOR_RESET "\n", strlen(txt) + 2, "--------------------------------");\
        printf("\n");\
    }while(0)

#define PRINT_ERROR_PRETTY(txt) \
    do{ \
        printf(COLOR_RED "Error: %s" COLOR_RESET "\n", (txt));\
    }while(0)

#define PRINT_SUCCESS_PRETTY(txt) \
    do{ \
        printf(COLOR_CYAN "Success: %s" COLOR_RESET "\n", (txt));\
    }while(0)

#define PRINT_WARNING_PRETTY(txt) \
    do{ \
        printf(COLOR_YELLOW "Warning: %s" COLOR_RESET "\n", (txt));\
    }while(0)

#define PRINT_MIN_MAX_PRETTY(min, max) \
    do{ \
        printf(COLOR_DIM "Min: %s - Max: %s" COLOR_RESET "\n", (min), (max));\
    }while(0)

#define PRINT_LIST_ITEM_PRETTY(i, txt) \
    do{ \
        printf("| %d. %s |\n", (i), (txt));\
    }while(0)

#endif