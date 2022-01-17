#ifndef _CTYPE_H
#define _CTYPE_H 1

#ifdef __cplusplus
extern "C" {
#endif

#include <stdcommon.h>

int toupper(int arg);
int tolower(int arg);

int isupper(int arg);
int islower(int arg);

int isalpha(int arg);
int isdigit(int arg);

#ifdef __cplusplus
}
#endif

#endif
