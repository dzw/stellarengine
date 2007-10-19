
#include "stdneb.h"
#include "utility/system.h"


//------------------------------------------------------------------------------
/**
*/
FILE* s_open (const char* acFilename, const char* acMode)
{
#ifdef STELLAR_USING_VC80
    FILE* pkFile;
    errno_t uiError = fopen_s(&pkFile,acFilename,acMode);
    if (uiError == 0)
    {
        return pkFile;
    }
    else
    {
        return 0;
    }
#else
    return fopen(acFilename,acMode);
#endif
}

//------------------------------------------------------------------------------
/**
*/
int s_print2file (FILE* pkFile, const char* acFormat, ...)
{
    if (!pkFile || !acFormat)
    {
        return -1;
    }

    va_list acArgs;
    va_start(acArgs,acFormat);

#ifdef STELLAR_USING_VC80
    int iNumWritten = vfprintf_s(pkFile,acFormat,acArgs);
#else
    int iNumWritten = vfprintf(pkFile,acFormat,acArgs);
#endif

    va_end(acArgs);
    return iNumWritten;
}

//------------------------------------------------------------------------------
/**
*/
int s_close (FILE* pkFile)
{
    return fclose(pkFile);
}

//------------------------------------------------------------------------------
/**
  获得Windows环境变量的值
*/
const char* s_getenv (const char* acEnvVarName)
{
//#ifdef STELLAR_USING_VC80
//   size_t uiRequiredSize;
//   errno_t uiError = getenv_s(&uiRequiredSize,0,0,acEnvVarName);
//   if (uiError > 0)
//   {
//       return 0;
//   }
//   getenv_s(&uiRequiredSize,ms_acEnvVar,SYSTEM_MAX_ENVVAR,acEnvVarName);
//#else
//    char* acEnvVar = getenv(acEnvVarName);
//    if (acEnvVar == 0)
//    {
//        return 0;
//    }
//    s_strcpy(ms_acEnvVar,SYSTEM_MAX_ENVVAR,getenv(acEnvVarName));
//#endif
//    return ms_acEnvVar;
  return 0;
}

//------------------------------------------------------------------------------
/**
*/
void* s_memcpy (void* pvDst, size_t uiDstSize, const void* pvSrc, size_t uiSrcSize)
{
#ifdef STELLAR_USING_VC80
    errno_t uiError = memcpy_s(pvDst,uiDstSize,pvSrc,uiSrcSize);
    if (uiError == 0)
    {
        return pvDst;
    }
    else
    {
        return 0;
    }
#else
    if (!pvDst || uiDstSize == 0 || !pvSrc || uiSrcSize == 0)
    {
        // Be consistent with the behavior of memcpy_s.
        return 0;
    }

    if (uiSrcSize > uiDstSize)
    {
        // The source memory is too large to copy to the destination.  To
        // be consistent with memcpy_s, return null as an indication that the
        // copy failed.
        return 0;
    }
    memcpy(pvDst,pvSrc,uiSrcSize);
    return pvDst;
#endif
}

//------------------------------------------------------------------------------
/**
*/
void s_sprintf (char* acDst, size_t uiDstSize, const char* acFormat, ...)
{
    if (!acDst || uiDstSize == 0 || !acFormat)
    {
        return ;
    }

    va_list acArgs;
    va_start(acArgs,acFormat);

#ifdef STELLAR_USING_VC80
    // need to use non-CRT thread safe function under Win32
    StringCchVPrintf(acDst, uiDstSize, acFormat, acArgs);
    //vsprintf_s(acDst,uiDstSize,acFormat,acArgs);
#else
    vsprintf(acDst,acFormat,acArgs);
#endif

    va_end(acArgs);
}

//------------------------------------------------------------------------------
/**
*/
char* s_strcpy (char* acDst, size_t uiDstSize, const char* acSrc)
{
#ifdef STELLAR_USING_VC80
    errno_t uiError = strcpy_s(acDst,uiDstSize,acSrc);
    if (uiError == 0)
    {
        return acDst;
    }
    else
    {
        return 0;
    }
#else
    if (!acDst || uiDstSize == 0 || !acSrc)
    {
        // Be consistent with the behavior of strcpy_s.
        return 0;
    }

    size_t uiSrcLen = strlen(acSrc);
    if (uiSrcLen + 1 > uiDstSize)
    {
        // The source string is too large to copy to the destination.  To
        // be consistent with strcpy_s, return null as an indication that the
        // copy failed.
        return 0;
    }
    strncpy(acDst,acSrc,uiSrcLen);
    acDst[uiSrcLen] = 0;
    return acDst;
#endif
}

//------------------------------------------------------------------------------
/**
*/
char* s_strcat (char* acDst, size_t uiDstSize, const char* acSrc)
{
#ifdef STELLAR_USING_VC80
    errno_t uiError = strcat_s(acDst,uiDstSize,acSrc);
    if (uiError == 0)
    {
        return acDst;
    }
    else
    {
        return 0;
    }
#else
    if (!acDst || uiDstSize == 0 || !acSrc)
    {
        // Be consistent with the behavior of strcat_s.
        return 0;
    }

    size_t uiSrcLen = strlen(acSrc);
    size_t uiDstLen = strlen(acDst);
    size_t uiSumLen = uiSrcLen + uiDstLen;
    if (uiSumLen + 1 > uiDstSize)
    {
        // The source string is too large to append to the destination.  To
        // be consistent with strcat_s, return null as an indication that
        // the concatenation failed.
        return 0;
    }
    strncat(acDst,acSrc,uiSrcLen);
    acDst[uiSumLen] = 0;
    return acDst;
#endif
}

//------------------------------------------------------------------------------
/**
*/
char* s_strncpy (char* acDst, size_t uiDstSize, const char* acSrc,
    size_t uiSrcSize)
{
#ifdef STELLAR_USING_VC80
    errno_t uiError = strncpy_s(acDst,uiDstSize,acSrc,uiSrcSize);
    if (uiError == 0)
    {
        return acDst;
    }
    else
    {
        return 0;
    }
#else
    if (!acDst || uiDstSize == 0 || !acSrc || uiSrcSize == 0)
    {
        // Be consistent with the behavior of strncpy_s.
        return 0;
    }

    if (uiSrcSize + 1 > uiDstSize)
    {
        // The source string is too large to copy to the destination.  To
        // be consistent with strncpy_s, return null as an indication that
        // the copy failed.
        return 0;
    }
    strncpy(acDst,acSrc,uiSrcSize);
    return acDst;
#endif
}

//------------------------------------------------------------------------------
/**
*/
char* s_strtok (char* acToken, const char* acDelimiters, char*& racNextToken)
{
#ifdef STELLAR_USING_VC80
    return strtok_s(acToken,acDelimiters,&racNextToken);
#else
    (void)racNextToken;  // avoid warning in release build
    return strtok(acToken,acDelimiters);
#endif
}


