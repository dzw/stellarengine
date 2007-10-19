// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdio.h>

#include <stdlib.h>
#include <stdarg.h>

void test(char* szname, size_t num, const char * format, ... )
{
    

   va_list args;
   
   va_start( args, format );
   vsprintf_s( szname, num, format, args );
   puts( szname );
}
#define TEST(szname, num, format, ...) \
    test(szname, num, format, ... );

int main( void )
{
    char szname[4096] = {0};
    TEST( szname, 4096, "-> Running test: %s\n", "Test::testFactory" );
   //test( "%s", "This is a string" );
   getchar();
}

//int _tmain(int argc, _TCHAR* argv[])
//{
//
//	return 0;
//}

