这几天在写一个server，由于框架相同，仅仅是获取数据源的地方有区别，所以，研究了一下如何使用面向对象的方法来动态加载so。

主要思想就是：

1.通过一个函数能够获得一个基类的指针，这样在调用基类的函数的时候，就能自动调用子类的实现了。

2.存储so对象的指针应该是外层类的一个static变量。

详细还是看代码吧：

1）首先定义一个公共的头文件，里面存储的基类的定义：（需要注意的就是，只要不是纯虚函数，那么就一定要有实现；还有就是析构函数需要为虚函数）

so和主调程序都需要包含这个头文件。
```
source_base.h

#ifndef _SOURCE_BASE_H_
#define _SOURCE_BASE_H_
#include <iostream>
using namespace std;
class CSourceBase;
/** 
 * @brief  获取实例
 * 
 * @param  client    new出的指针
 * 
 * @return  0      succ
 *       else    fail
 */
extern “C” int CreatObj(CSourceBase *& client);
class CSourceBase
{
  public:
    CSourceBase(){};
    virtual ~CSourceBase(){};
  public:
    virtual int GetFriends(int iUin,char *data,int &iLen,int maxLen)
    {
      return 0;
    }
};
#endif
```
2）我们来看so的实现  
```
xy_source.h

#ifndef _XY_SOURCE_H_
#define _XY_SOURCE_H_
#include <iostream>
#include “sourcebase.h”
using namespace std;
class CXY_Source:public CSourceBase
{
  public:
    CXY_Source();
    virtual ~CXY_Source();
    {}
    int GetFriends(int iUin,char *data,int &iLen,int maxLen);
};
#endif
```
```
xy_source.cpp


#include “xy_source.h”
int CreatObj(CSourceBase *& client)
{
  client = new CXY_Source();
  return 0;
}
CXY_Source::CXY_Source() { }
CXY_Source::~CXY_Source() { }
int CXY_Source::GetFriends(int iUin,char *data,int &iLen,int maxLen)
{
  return 0;
}
```
3）调用者的实现

这里说明一下，因为这里想要对外封装成透明的，所以，采用了如下的方式。
```
xy_client.h

#ifndef _XY_CLIENT_H_
#define _XY_CLIENT_H_
#include <iostream>
#include “sourcebase.h”
using namespace std;
typedef int (*FunPtr)(CSourceBase *& client);
class CXY_Client
{
  public:
    static void *SoObj;
  public:
    CXY_Client();
    virtual ~CXY_Client();
    //载入so
    int Init(const char * soname);
    int GetFriends(int iUin,char *data,int &iLen,int maxLen);
  private:
    CSourceBase *m_Client;
};
```
```
xy_client.cpp

#include “xy_client.h”
void* CXY_Client::SoObj=NULL;
CXY_Client::CXY_Client()
{
  m_Client = NULL;
}
CXY_Client::~CXY_Client()
{
  if(m_Client)
  {
    delete m_Client;
  }
}
int CXY_Client::Init(const char * soname)
{
  string strSoName;
  if(soname==NULL)
  {
    strSoName = “../lib/xysource.so”;
  }
  else
  {
    strSoName = soname;
  }
  if(strSoName.size()==0)
  {
    strSoName = “../lib/xysource.so”;
  }
  if(CXY_Client::SoObj == NULL)
  {
    SoObj=dlopen((char*)strSoName.c_str(),RTLD_LAZY);
    if(SoObj==NULL)
    {
      return -1;
    }
  }
  if(m_Client==NULL)
  {
    FunPtr func;
    func = (FunPtr)dlsym(SoObj, “CreatObj”);
    int ret = (*func)(m_Client);
    if(ret)
    {
      return -2;
    }
  }
  return 0;
}
int CXY_Client::GetFriends(int iUin,char *data,int &iLen,int maxLen)
{
  return m_Client->GetFriends(iUin,data,iLen,maxLen);
}
```
OK，到此为止代码就结束了，大家可能会发现我没有调用dlclose，这是因为static变量没有必要来调用，在进程结束时会自动释放句柄，当然如果需要有释放的应用场景的话，可以通过增加计数来实现。