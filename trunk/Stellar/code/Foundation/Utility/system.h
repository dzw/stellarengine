
#ifndef UTIL_SYSTEM_H
#define UTIL_SYSTEM_H

#include "core/types.h"


FILE* s_open (const char* acFilename, const char* acMode);
int s_print2file (FILE* pkFile, const char* acFormat, ...);
int s_close (FILE* pkFile);
const char* s_getenv (const char* acEnvVarName);
void* s_memcpy (void* pvDst, size_t uiDstSize, const void* pvSrc, size_t uiSrcSize);
void s_sprintf (char* acDst, size_t uiDstSize, const char* acFormat, ...);
char* s_strcpy (char* acDst, size_t uiDstSize, const char* acSrc);
char* s_strcat (char* acDst, size_t uiDstSize, const char* acSrc);
char* s_strncpy (char* acDst, size_t uiDstSize, const char* acSrc, size_t uiSrcSize);
char* s_strtok (char* acToken, const char* acDelimiters, char*& racNextToken);


#endif