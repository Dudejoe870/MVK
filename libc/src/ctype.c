#include "ctype.h"

int toupper(int arg)
{
    if (!islower(arg)) return arg;
    return arg - 32;
}

int tolower(int arg)
{
    if (!isupper(arg)) return arg;
    return arg + 32;
}

int isupper(int arg)
{
    return arg >= 'A' && arg <= 'Z';
}

int islower(int arg)
{
    return arg >= 'a' && arg <= 'z';
}

int isalpha(int arg)
{
    return isupper(arg) || islower(arg);
}

int isdigit(int arg)
{
    return arg >= '0' && arg <= '9';
}
