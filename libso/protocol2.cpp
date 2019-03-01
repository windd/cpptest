#include<stdio.h>
#include "protocol2.h"

void Protocol1::out1(){
    printf("zhe shi 2 de out1 \n");
}

void Protocol1::out2(){
    printf("zhe shi 2 de out2 \n");
}

//Protocol1::Protocol1(){}
//Protocol1::~Protocol1(){}

int CreatObj(BaseInterface *& objj){
    objj = new Protocol1();
    return 0;
}