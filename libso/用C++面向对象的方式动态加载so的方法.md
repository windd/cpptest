�⼸����дһ��server�����ڿ����ͬ�������ǻ�ȡ����Դ�ĵط����������ԣ��о���һ�����ʹ���������ķ�������̬����so��

��Ҫ˼����ǣ�

1.ͨ��һ�������ܹ����һ�������ָ�룬�����ڵ��û���ĺ�����ʱ�򣬾����Զ����������ʵ���ˡ�

2.�洢so�����ָ��Ӧ����������һ��static������

��ϸ���ǿ�����ɣ�

1�����ȶ���һ��������ͷ�ļ�������洢�Ļ���Ķ��壺����Ҫע��ľ��ǣ�ֻҪ���Ǵ��麯������ô��һ��Ҫ��ʵ�֣����о�������������ҪΪ�麯����

so������������Ҫ�������ͷ�ļ���
```
source_base.h

#ifndef _SOURCE_BASE_H_
#define _SOURCE_BASE_H_
#include <iostream>
using namespace std;
class CSourceBase;
/** 
 * @brief  ��ȡʵ��
 * 
 * @param  client    new����ָ��
 * 
 * @return  0      succ
 *       else    fail
 */
extern ��C�� int CreatObj(CSourceBase *& client);
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
2����������so��ʵ��  
```
xy_source.h

#ifndef _XY_SOURCE_H_
#define _XY_SOURCE_H_
#include <iostream>
#include ��sourcebase.h��
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


#include ��xy_source.h��
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
3�������ߵ�ʵ��

����˵��һ�£���Ϊ������Ҫ�����װ��͸���ģ����ԣ����������µķ�ʽ��
```
xy_client.h

#ifndef _XY_CLIENT_H_
#define _XY_CLIENT_H_
#include <iostream>
#include ��sourcebase.h��
using namespace std;
typedef int (*FunPtr)(CSourceBase *& client);
class CXY_Client
{
  public:
    static void *SoObj;
  public:
    CXY_Client();
    virtual ~CXY_Client();
    //����so
    int Init(const char * soname);
    int GetFriends(int iUin,char *data,int &iLen,int maxLen);
  private:
    CSourceBase *m_Client;
};
```
```
xy_client.cpp

#include ��xy_client.h��
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
    strSoName = ��../lib/xysource.so��;
  }
  else
  {
    strSoName = soname;
  }
  if(strSoName.size()==0)
  {
    strSoName = ��../lib/xysource.so��;
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
    func = (FunPtr)dlsym(SoObj, ��CreatObj��);
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
OK������Ϊֹ����ͽ����ˣ���ҿ��ܻᷢ����û�е���dlclose��������Ϊstatic����û�б�Ҫ�����ã��ڽ��̽���ʱ���Զ��ͷž������Ȼ�����Ҫ���ͷŵ�Ӧ�ó����Ļ�������ͨ�����Ӽ�����ʵ�֡�