#ifndef UTIL_HASHTABLE_H
#define UTIL_HASHTABLE_H
//------------------------------------------------------------------------------
/**
  @class HashTable 
  ʹ��std��չ�⣬�����м�㣬�����Ժ��ƽ̨ʱ�޸ġ�
*/

#include <hash_map>

namespace Util
{

template<class KEYTYPE, class VALUETYPE>
class HashTable : public stdext::hash_map<KEYTYPE, VALUETYPE>
{
};

}
#endif