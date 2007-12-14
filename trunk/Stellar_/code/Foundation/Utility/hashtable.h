#ifndef UTIL_HASHTABLE_H
#define UTIL_HASHTABLE_H
//------------------------------------------------------------------------------
/**
  @class HashTable 
  使用std扩展库，加了中间层，方便以后跨平台时修改。
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