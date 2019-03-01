/*
https://blog.csdn.net/long12310225/article/details/77520224
https://www.jb51.net/article/101749.htm
https://blog.csdn.net/lq_fly_pig/article/details/81051183
使用ldd -r xxx.so或者 nm -A xxx.so等命令，查看so有哪些符号未定义的。
*/
#include<stdio.h>
#include"interface.h"
#include<iostream>
#include<dlfcn.h>
using namespace std;
typedef int(* pfun)(BaseInterface *& objj);
int main(){
    void * instance;
    instance = dlopen("./libprotocol2.so",RTLD_LAZY);
    if(instance == NULL){
        printf("ldopen error:%s\n",dlerror());
        return -1;
    }
    pfun getobj = (pfun)dlsym(instance,"CreatObj");
    if(getobj == NULL){
        printf("dlsym error\n");
        return -1;
    }
    BaseInterface * obj;
    getobj(obj);
    obj->out1();
    obj->out2();
    dlclose(instance);
    return 0;
}
