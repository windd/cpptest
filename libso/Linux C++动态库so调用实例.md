test.cpp
```
#include "stdio.h"

extern "C"
{
 void print();
}

void print(){
 printf("songbai");
}


class Lynn{
 public:
 Lynn();
};
```

���룺

g++ test.cpp -fPIC -shared -o libtest.so


```
test_c.cpp



#include <stdio.h>
#include <dlfcn.h>

int main(int argc, char *argv[]){
 void * libm_handle = NULL;
 void (*print)();
 char *errorInfo;
 float result;

 // dlopen ���������Զ�����������е���������������������һ�����������������Ķ������ͻ��Զ��������ǡ�
 // ��������һ��������þ�����ں����� API ����
 libm_handle = dlopen("/home/saxon/work/Unix/libtest.so", RTLD_LAZY);
 // ������� NULL �������ʾ�޷��ҵ������ļ������̽���������Ļ�������õ������һ����������Խ�һ��ѯ�ʶ���
 if (!libm_handle){
 // ������� NULL ���,ͨ��dlerror��������ȡ���޷����ʶ����ԭ��
 printf("Open Error:%s.\n",dlerror());
 return 0;
 }

 // ʹ�� dlsym ���������Խ����´򿪵Ķ����ļ��еķ��š�������õ�һ����Ч��ָ��÷��ŵ�ָ�룬�����ǵõ�һ�� NULL ������һ������
 print = dlsym(libm_handle,"print");
 errorInfo = dlerror();// ����dlerror���������ش�����Ϣ��ͬʱ���ڴ��еĴ�����Ϣ�����
 if (errorInfo != NULL){
 printf("Dlsym Error:%s.\n",errorInfo);
 return 0;
 }

 // ִ�С�print������
 (*print)();
 printf("result = %f.\n",result);

 // ���� ELF �����е�Ŀ�꺯����ͨ������ dlclose ���رն����ķ���
 dlclose(libm_handle);

 return 0;
}
```

����;

g++ test_c.cpp -o test -ldl
--------------------- 
���ߣ�long12310225 
��Դ��CSDN 
ԭ�ģ�https://blog.csdn.net/long12310225/article/details/77520224 
��Ȩ����������Ϊ����ԭ�����£�ת���븽�ϲ������ӣ�