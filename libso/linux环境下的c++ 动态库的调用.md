一.下面主要是dlopen打开动态库.so相关的API接口函数。

1. void* dlopen(const char* filename,int flag);

filename 是动态库的path路径，flag是动态库加载的几种方法。

RTLD_LAZY：dlopen返回前，对动态库中存在的未定义的变量不进行解析，只对函数符号有效，变量符号会被立即加载。

RTLD_NOW： 需要在dlopen返回前，解析出所有未定义符号，如果解析不出来，dlopen时会返回NULL,提示error信息: undefined symbol: xxxx.......

RTLD_GLOBAL：这个库中的符号可以被随后的动态库使用。

2. char* dlerror(void)

?返回相关的错误，如果从初始化到最后结束都没有错误，则返回NULL。

3. void * dlsym(void* handle,const char* symbol);

返回符号为symbol的函数的地址,需要用到dlopen中返回的handle,需要使用dlerror来判断是否是错误，

二.下面举个简单的例子说明下。

定义一个基类CTestDL，test()和print() 都是纯虚函数。C++支持了命名空间、类以及重载等机制，所以符号的命名规则和C不同。而dl API属于C的部分，使用的C的命名规则，所以在实现上述两个函数时（也就是说需要通过动态链接的C++函数）需要使用C的符号命名规则。这需要实现extern 'C'实现。
```
#ifndef __CTEST_DL_H__  
#define __CTEST_DL_H__
 
class CTestDL
{
	public:
		CTestDL(int a,int b)
		{
			length =a;
			width =b;
		}
	    virtual ~ CTestDL()
		{
		}
	public:
		virtual void test(const char*) =0;
		virtual void print()=0; 
	protected:
		int length;
		int width;
};

extern "C" CTestDL* GetClass(int,int);  //导出函数名
 
#endif
```
下面是一个派生类Test,继承上面的基类
```
#include <iostream>
 
#include "ctest.h"
 
using namespace std;
 
class Test :public CTestDL
{
	public:
		Test(int a,int b):CTestDL(a,b) //虚基类已经赋值了
		{
			length = a;
			width = b;
		}
		~Test(){}
	public:
		void test(const char*);
		void print();
	private:
		int length ;
		int width;
};
```
Test类的cpp文件：
```
#include <iostream>
#include <stdio.h>
 
#include "test.h"
using namespace std;
 
CTestDL* GetClass(int a,int b)
{
	return new Test(a,b);
}
void Test::test(const char* data)
{
	cout << "--this is a test program !--" <<"**: "<< data << endl;
}
void Test::print()
{
  int area = length * width;
  cout << "**** the area is **** :" << area << endl;
}
```
编译的命令: g++ -shared -lc -s -o libtdlc.so test.cpp -fPIC ? ?生成动态库libtdlc.so

测试程序:
```
#include <stdio.h>
#include <unistd.h>
#include <dlfcn.h>
#include <iostream>
#include "ctest.h"
using namespace std;
 
 
typedef CTestDL* (*pTestDLObj)(int,int);  //定义一个函数指针
 
int main(int argc,char* argv[])
{
	//CTestDL *(*pTestDLObj)(int,int);
	dlerror();
	
	void *lib_dl = dlopen("./libtdlc.so",RTLD_LAZY);
    if(NULL == lib_dl)
    {
        printf("load library libtdlc.so error.\nErrmsg:%s\n",dlerror());
        return -1;
    }
    //pTestDLObj = (CTestDL * (*)(int,int))dlsym(lib_dl,"GetClass");
	pTestDLObj create_class = (pTestDLObj)dlsym(lib_dl,"GetClass");
	
    const char *dlmsg = dlerror();
    if(NULL != dlmsg)
    {
        printf("get class testdl error\nErrmsg:%s\n",dlmsg);
        dlclose(lib_dl);
        return -1;
    }
 
    //CTestDL *pTestdl = pTestDLObj(2,3);
	//CTestDL *pTestdl = (*pTestDLObj)();
	CTestDL* tmp = create_class(3,3);
	tmp->print();
	tmp->test("success full !");
    //pTestdl->print();
    //pTestdl->test("success full ");
 
    delete tmp;
    dlclose(lib_dl);
    return 0;
}
编译：?g++ main.cpp -rdynamic -ldl -s -o dl_main
```
程序执行的结果：

![Image text](https://img-blog.csdn.net/20180715115136366?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xxX2ZseV9waWc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

--------------------- 
作者：lq_fly_pig 
来源：CSDN 
原文：https://blog.csdn.net/lq_fly_pig/article/details/81051183 
版权声明：本文为博主原创文章，转载请附上博文链接！