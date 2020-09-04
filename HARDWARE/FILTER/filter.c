#include "spi.h"
#include "filter.h"
#include "spi.h"
/*
限幅消抖滤波法
*/

//功能函数：返回两个整数差
unsigned int num_sub( unsigned int a, unsigned int b )
{
    return ( a >= b ? ( a - b ) : ( b - a ) );
}
//滤波函数 
unsigned int filter10( void )
{
    unsigned int count = 0;
    unsigned int new_value = 0;
    unsigned int last_value = 0;
    new_value = GetLTC2500Valuesg();
    if( num_sub( new_value, last_value ) < A )
    {
        new_value = new_value;	//误差小于A
    }
    else
    {
        new_value = last_value;	//误差大于A
    }
    
    
//    while( new_value !=  last_value)	//小于A的情况
//    {
//        count++;
//        if( count >= N )
//        {
//            last_value = new_value;
//            return new_value;
//        }
//        new_value = GetLTC2500Valuesg();
//        if( num_sub( new_value, last_value ) < A )
//        {
//            new_value = new_value;
//        }
//        else
//        {
//            new_value = last_value;
//        }
//    }
    return new_value;
}

