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

编译：

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

 // dlopen 函数还会自动解析共享库中的依赖项。这样，如果您打开了一个依赖于其他共享库的对象，它就会自动加载它们。
 // 函数返回一个句柄，该句柄用于后续的 API 调用
 libm_handle = dlopen("/home/saxon/work/Unix/libtest.so", RTLD_LAZY);
 // 如果返回 NULL 句柄，表示无法找到对象文件，过程结束。否则的话，将会得到对象的一个句柄，可以进一步询问对象
 if (!libm_handle){
 // 如果返回 NULL 句柄,通过dlerror方法可以取得无法访问对象的原因
 printf("Open Error:%s.\n",dlerror());
 return 0;
 }

 // 使用 dlsym 函数，尝试解析新打开的对象文件中的符号。您将会得到一个有效的指向该符号的指针，或者是得到一个 NULL 并返回一个错误
 print = dlsym(libm_handle,"print");
 errorInfo = dlerror();// 调用dlerror方法，返回错误信息的同时，内存中的错误信息被清空
 if (errorInfo != NULL){
 printf("Dlsym Error:%s.\n",errorInfo);
 return 0;
 }

 // 执行“print”方法
 (*print)();
 printf("result = %f.\n",result);

 // 调用 ELF 对象中的目标函数后，通过调用 dlclose 来关闭对它的访问
 dlclose(libm_handle);

 return 0;
}
```

编译;

g++ test_c.cpp -o test -ldl
--------------------- 
作者：long12310225 
来源：CSDN 
原文：https://blog.csdn.net/long12310225/article/details/77520224 
版权声明：本文为博主原创文章，转载请附上博文链接！