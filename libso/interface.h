#ifndef DW_INTER
#define DW_INTER
#include <iostream>
using namespace std;
//class BaseInterface;



class BaseInterface 
{
    public:
    virtual void out1(void){};
    virtual void out2(void){};
    BaseInterface(){};
    virtual ~BaseInterface(){};
};

extern "C" int CreatObj(BaseInterface *& objj);
#endif