
#ifndef DATA_H
#define DATA_H

#include <iostream>
#include <string>

//std::string fn;// = logger::CreateLogName();

namespace Num
{    
    const std::string fn="log2.txt";
    const std::string fn_logger_count_number="lognum.txt";
    //long cnt=0;    
};


struct Data{
    long x,y,n,m;
    Data() { }
    Data(long x, long y, long n, long m)
    {
        this->x=x;
        this->y=y;
        this->n=n;
        this->m=m;
    }
};

#endif