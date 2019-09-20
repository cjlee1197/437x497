/*==========================================================================+
|  Class    : CtmStrings                                      				|
|  Task     :                                                               |
|---------------------------------------------------------------------------|
|  Compile  : Gcc V3.2.2                                                    |
|  Link     : Gcc V3.2.2                                                    |
|  Call     :                                                               |
|---------------------------------------------------------------------------|
|  Author   : shousu fan                                                       |
|  Version  : V1.00                                                         |
|  Creation : 2006/06/16                                                    |
|  Remark	: 						                                    |
+==========================================================================*/
#ifndef D_TMSTRINS
#define D_TMSTRINS
#include <ext/hash_map>
#include <string.h>
using namespace __gnu_cxx;


//	hash表使用的比較函數
struct eqstr_tm
{
	bool operator()(const char* s1, const char* s2) const
    {
    	return strcmp(s1, s2) == 0;
    }
};

/*==========================================================================+
|           Class declaration - CtmStrings                                	|
|	NOTE:	需要注意的是，當字串添加到列表當中的時候，字串不能被釋放		|
|			因爲此類僅僅存儲的是字串的地址									|
+==========================================================================*/

//	其實不一定需要hash_map，只需要使用標準函數find，也應該是可以的
class CtmStrings
{

	/*--------------------------------------------------------------------------+
	|           Constructor and destructor                                      |
	+--------------------------------------------------------------------------*/
public:
	CtmStrings(); 
    virtual ~CtmStrings();
    
    /*--------------------------------------------------------------------------+
	|           Operations                                                      |
	+--------------------------------------------------------------------------*/
public:
	int AddString(char* psz);
	int GetIndex(char* psz);
	char* GetString(int nIndex);  
	int GetStrCount() 	{return m_StrTotalNum;};
	void InitStrings()	{m_StrTotalNum = 0;};
	
	/*--------------------------------------------------------------------------+
	|           Attributes                                                      |
	+--------------------------------------------------------------------------*/
private:
	int m_StrTotalNum;			//	列表中子串數
	hash_map<int, char*> m_strMap;
	hash_map<const char*, int,  hash<const char*>, eqstr_tm> m_indexMap;
    
};


#endif
