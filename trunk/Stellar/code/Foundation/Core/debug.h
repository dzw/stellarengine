#pragma once
#ifndef CORE_DEBUG_H
#define CORE_DEBUG_H
//------------------------------------------------------------------------------
/**
    @file core/debug.h

    Stellar debug macros.
  
    s_assert()  - the vanilla assert() Macro
    s_verify()  - like assert() except that the statement in parens is simply evaluated, unchecked, if __NEBULA_NO_ASSERT is set
    s_assert2() - an assert() plus a message from the programmer
*/

void __cdecl s_printf(const char *, ...) __attribute__((format(printf,1,2)));
void __cdecl s_error(const char*, ...) __attribute__((noreturn)) __attribute__((format(printf,1,2)));
void __cdecl s_dbgout(const char*, ...) __attribute__((format(printf,1,2)));
void __cdecl s_warning(const char*, ...) __attribute__((format(printf,1,2)));
void s_sleep(double);
void s_barf(const char *, const char *, int) __attribute__((noreturn));
void s_barf2(const char*, const char*, const char*, int) __attribute__((noreturn));

#ifdef __STELLAR_NO_ASSERT__
#define s_assert(exp)
#define s_verify(exp) (exp)
#define s_verify2(exp,imsg) (exp)
#define s_assert2(exp, msg)
#define s_dxtrace(hr, msg)
#else
#define s_assert(exp) { if (!(exp)) s_barf(#exp,__FILE__,__LINE__); }

// an assert with a verbose hint from the programmer
#define s_assert2(exp, msg) { if (!(exp)) s_barf2(#exp,msg,__FILE__,__LINE__); }

// dx9 specific: check HRESULT and display DX9 specific message box
#define s_dxtrace(hr, msg) { if (FAILED(hr)) DXTrace(__FILE__,__LINE__,hr,msg,true); }

#endif
//------------------------------------------------------------------------------
#endif
