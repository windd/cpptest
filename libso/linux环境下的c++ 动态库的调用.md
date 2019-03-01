һ.������Ҫ��dlopen�򿪶�̬��.so��ص�API�ӿں�����

1. void* dlopen(const char* filename,int flag);

filename �Ƕ�̬���path·����flag�Ƕ�̬����صļ��ַ�����

RTLD_LAZY��dlopen����ǰ���Զ�̬���д��ڵ�δ����ı��������н�����ֻ�Ժ���������Ч���������Żᱻ�������ء�

RTLD_NOW�� ��Ҫ��dlopen����ǰ������������δ������ţ����������������dlopenʱ�᷵��NULL,��ʾerror��Ϣ: undefined symbol: xxxx.......

RTLD_GLOBAL��������еķ��ſ��Ա����Ķ�̬��ʹ�á�

2. char* dlerror(void)

?������صĴ�������ӳ�ʼ������������û�д����򷵻�NULL��

3. void * dlsym(void* handle,const char* symbol);

���ط���Ϊsymbol�ĺ����ĵ�ַ,��Ҫ�õ�dlopen�з��ص�handle,��Ҫʹ��dlerror���ж��Ƿ��Ǵ���

��.����ٸ��򵥵�����˵���¡�

����һ������CTestDL��test()��print() ���Ǵ��麯����C++֧���������ռ䡢���Լ����صȻ��ƣ����Է��ŵ����������C��ͬ����dl API����C�Ĳ��֣�ʹ�õ�C����������������ʵ��������������ʱ��Ҳ����˵��Ҫͨ����̬���ӵ�C++��������Ҫʹ��C�ķ���������������Ҫʵ��extern 'C'ʵ�֡�
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

extern "C" CTestDL* GetClass(int,int);  //����������
 
#endif
```
������һ��������Test,�̳�����Ļ���
```
#include <iostream>
 
#include "ctest.h"
 
using namespace std;
 
class Test :public CTestDL
{
	public:
		Test(int a,int b):CTestDL(a,b) //������Ѿ���ֵ��
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
Test���cpp�ļ���
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
���������: g++ -shared -lc -s -o libtdlc.so test.cpp -fPIC ? ?���ɶ�̬��libtdlc.so

���Գ���:
```
#include <stdio.h>
#include <unistd.h>
#include <dlfcn.h>
#include <iostream>
#include "ctest.h"
using namespace std;
 
 
typedef CTestDL* (*pTestDLObj)(int,int);  //����һ������ָ��
 
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
���룺?g++ main.cpp -rdynamic -ldl -s -o dl_main
```
����ִ�еĽ����

![Image text](https://img-blog.csdn.net/20180715115136366?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xxX2ZseV9waWc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

--------------------- 
���ߣ�lq_fly_pig 
��Դ��CSDN 
ԭ�ģ�https://blog.csdn.net/lq_fly_pig/article/details/81051183 
��Ȩ����������Ϊ����ԭ�����£�ת���븽�ϲ������ӣ�