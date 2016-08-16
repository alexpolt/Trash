#include <stdio.h>
#include <windows.h>
#include <eh.h>

void SEFunc();
void trans_func( unsigned int, EXCEPTION_POINTERS* );

class SE_Exception
{
private:
    unsigned int nSE;
    int a;
public:
    SE_Exception(int a_=0) : a(a_) {}
    SE_Exception( unsigned int n ) : nSE( n ) {}
    ~SE_Exception() {printf("~se %d\n",a);}
    unsigned int getSeNumber() { return nSE; }
};
int main( void )
{
//    try
    {
        _set_se_translator( trans_func );
        SEFunc();
    }
  //  catch( SE_Exception e )
    {
        printf( "Caught a __try exception with SE_Exception.\n" );
    }
}
void SEFunc()
{
SE_Exception test(1);
//    __try
    {
        int x, y=0;
        x = 5 / y;
    }
 //   __finally
    {
        printf( "In finally\n" );
    }
}
void trans_func( unsigned int u, EXCEPTION_POINTERS* pExp )
{
    printf( "In trans_func.\n" );
    throw SE_Exception();
}
