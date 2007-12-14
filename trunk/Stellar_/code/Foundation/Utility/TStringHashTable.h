#ifndef UTIL_TSTRINGHASHTABLE_H
#define UTIL_TSTRINGHASHTABLE_H

#include "utility/string.h"
#include "core/types.h"
#include <hash_map>

// The class TVALUE is either native data or is class data that has the
// following member functions:
//   TVALUE::TVALUE ()
//   TVALUE& TVALUE::operator= (const TVALUE&)

namespace Util
{


template <class TVALUE>
class TStringHashTable
{
public:
    // construction and destruction
    TStringHashTable (int iTableSize);
    ~TStringHashTable ();

    // element access
    int GetQuantity () const;

    // insert a key-value pair into the hash table
    bool Insert (const Util::String& rkKey, const TVALUE& rtValue);

    // search for a key and returns it value (null, if key does not exist)
    TVALUE* Find (const Util::String& rkKey) const;

    // remove key-value pairs from the hash table
    bool Remove (const Util::String& rkKey);
    void RemoveAll ();

    // linear traversal of table
    TVALUE* GetFirst (Util::String* pkKey) const;
    TVALUE* GetNext (Util::String* pkKey) const;

private:
    class HashItem
    {
    public:
        HashItem () : m_kKey("") { /**/ }

        Util::String m_kKey;
        TVALUE m_tValue;
        HashItem* m_pkNext;
    };

    // key-to-index construction
    int HashFunction (const Util::String& rkKey) const;

    // hash table
    int m_iTableSize;
    int m_iQuantity;
    HashItem** m_apkTable;

    // iterator for traversal
    mutable int m_iIndex;
    mutable HashItem* m_pkItem;
};


//----------------------------------------------------------------------------
template <class TVALUE>
TStringHashTable<TVALUE>::TStringHashTable (int iTableSize)
{
    s_assert(iTableSize > 0);

    m_iTableSize = iTableSize;
    m_iQuantity = 0;
    m_iIndex = 0;
    m_pkItem = 0;
    m_apkTable = s_new HashItem*[m_iTableSize];
    memset(m_apkTable,0,m_iTableSize*sizeof(HashItem*));
}
//----------------------------------------------------------------------------
template <class TVALUE>
TStringHashTable<TVALUE>::~TStringHashTable ()
{
    RemoveAll();
    s_delete[] m_apkTable;
}
//----------------------------------------------------------------------------
template <class TVALUE>
int 
TStringHashTable<TVALUE>::GetQuantity () const
{
    return m_iQuantity;
}
//----------------------------------------------------------------------------
template <class TVALUE>
bool 
TStringHashTable<TVALUE>::Insert (const Util::String& rkKey,
    const TVALUE& rtValue)
{
    // find hash table entry for given key
    int iIndex = HashFunction(rkKey);
    HashItem* pkItem = m_apkTable[iIndex];

    // search for item in list associated with key
    while (pkItem)
    {
        if (rkKey == pkItem->m_kKey)
        {
            // item already in hash table
            return false;
        }
        pkItem = pkItem->m_pkNext;
    }

    // add item to beginning of list
    pkItem = s_new HashItem;
    pkItem->m_kKey = rkKey;
    pkItem->m_tValue = rtValue;
    pkItem->m_pkNext = m_apkTable[iIndex];
    m_apkTable[iIndex] = pkItem;
    m_iQuantity++;

    return true;
}
//----------------------------------------------------------------------------
template <class TVALUE>
TVALUE* 
TStringHashTable<TVALUE>::Find (const Util::String& rkKey) const
{
    // find hash table entry for given key
    int iIndex = HashFunction(rkKey);
    HashItem* pkItem = m_apkTable[iIndex];

    // search for item in list associated with key
    while (pkItem)
    {
        if (rkKey == pkItem->m_kKey)
        {
            // item is in hash table
            return &pkItem->m_tValue;
        }
        pkItem = pkItem->m_pkNext;
    }

    return 0;
}
//----------------------------------------------------------------------------
template <class TVALUE>
bool 
TStringHashTable<TVALUE>::Remove (const Util::String& rkKey)
{
    // find hash table entry for given key
    int iIndex = HashFunction(rkKey);
    HashItem* pkItem = m_apkTable[iIndex];

    if (!pkItem)
    {
        return false;
    }

    if (rkKey == pkItem->m_kKey)
    {
        // item is at front of list, strip it off
        HashItem* pkSave = pkItem;
        m_apkTable[iIndex] = pkItem->m_pkNext;
        s_delete pkSave;
        m_iQuantity--;
        return true;
    }

    // search for item in list
    HashItem* pkPrev = pkItem;
    HashItem* pkCurr = pkItem->m_pkNext;
    while (pkCurr && rkKey != pkCurr->m_kKey)
    {
        pkPrev = pkCurr;
        pkCurr = pkCurr->m_pkNext;
    }

    if (pkCurr)
    {
        // found the item
        pkPrev->m_pkNext = pkCurr->m_pkNext;
        s_delete pkCurr;
        m_iQuantity--;
        return true;
    }

    return false;
}
//----------------------------------------------------------------------------
template <class TVALUE>
void 
TStringHashTable<TVALUE>::RemoveAll ()
{
    if (m_iQuantity > 0)
    {
        for (int iIndex = 0; iIndex < m_iTableSize; iIndex++)
        {
            while (m_apkTable[iIndex])
            {
                HashItem* pkSave = m_apkTable[iIndex];
                m_apkTable[iIndex] = m_apkTable[iIndex]->m_pkNext;
                s_delete pkSave;
                if (--m_iQuantity == 0)
                {
                    return;
                }
            }
        }
    }
}
//----------------------------------------------------------------------------
template <class TVALUE>
TVALUE* 
TStringHashTable<TVALUE>::GetFirst (Util::String* pkKey) const
{
    if (m_iQuantity > 0)
    {
        for (m_iIndex = 0; m_iIndex < m_iTableSize; m_iIndex++)
        {
            if (m_apkTable[m_iIndex])
            {
                m_pkItem = m_apkTable[m_iIndex];
                *pkKey = m_pkItem->m_kKey;
                return &m_pkItem->m_tValue;
            }
        }
    }

    return 0;
}
//----------------------------------------------------------------------------
template <class TVALUE>
TVALUE* 
TStringHashTable<TVALUE>::GetNext (Util::String* pkKey) const
{
    if (m_iQuantity > 0)
    {
        m_pkItem = m_pkItem->m_pkNext;
        if (m_pkItem)
        {
            *pkKey = m_pkItem->m_kKey;
            return &m_pkItem->m_tValue;
        }
        
        for (m_iIndex++; m_iIndex < m_iTableSize; m_iIndex++)
        {
            if (m_apkTable[m_iIndex])
            {
                m_pkItem = m_apkTable[m_iIndex];
                *pkKey = m_pkItem->m_kKey;
                return &m_pkItem->m_tValue;
            }
        }
    }

    return 0;
}
//----------------------------------------------------------------------------
template <class TVALUE>
int 
TStringHashTable<TVALUE>::HashFunction (const Util::String& rkKey) const
{
    int iHiKey = 0, iLoKey = 0;
    for (int i = 0; i < (int)rkKey.length(); i++)
    {
        if (i & 1)
        {
            iHiKey += (int)rkKey[i];
        }
        else
        {
            iLoKey += (int)rkKey[i];
        }
    }
    return abs(iLoKey * iHiKey) % m_iTableSize;
}
//----------------------------------------------------------------------------

}

#endif
