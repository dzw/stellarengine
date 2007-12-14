
#ifndef UTIL_THASHTABLE_H
#define UTIL_THASHTABLE_H

// The class TKEY is either native data or is class data that has the
// following member functions:
//   TKEY::TKEY ()
//   TKEY& TKEY::operator= (const TKEY&)
//   bool TKEY::operator== (const TKEY&) const
//   bool TKEY::operator!= (const TKEY&) const
//   TKEY::operator unsigned int () const
// The implicit conversion to unsigned int is used to select a hash table
// index for the T object.  The return value need not be within the range of
// hash table indices.  THashTable will use modular arithmetic to make this
// happen.
//
// The class TVALUE is either native data or is class data that has the
// following member functions:
//   TVALUE::TVALUE ()
//   TVALUE& TVALUE::operator= (const TVALUE&)

namespace Util
{

template <class TKEY, class TVALUE>
class THashTable
{
public:
    // construction and destruction
    THashTable (int iTableSize);
    ~THashTable ();

    // element access
    int GetQuantity () const;

    // insert a key-value pair into the hash table
    bool Insert (const TKEY& rtKey, const TVALUE& rtValue);

    // search for a key and returns it value (null, if key does not exist)
    TVALUE* Find (const TKEY& rtKey) const;

    // remove key-value pairs from the hash table
    bool Remove (const TKEY& rtKey);
    void RemoveAll ();

    // linear traversal of table
    TVALUE* GetFirst (TKEY* ptKey) const;
    TVALUE* GetNext (TKEY* ptKey) const;

    // user-specified key-to-index construction
    int (*UserHashFunction)(const TKEY&);

private:
    class HashItem
    {
    public:
        TKEY m_tKey;
        TVALUE m_tValue;
        HashItem* m_pkNext;
    };

    // Default key-to-index construction (override by user-specified when
    // requested).
    int HashFunction (const TKEY& rtKey) const;

    // hash table
    int m_iTableSize;
    int m_iQuantity;
    HashItem** m_apkTable;

    // iterator for traversal
    mutable int m_iIndex;
    mutable HashItem* m_pkItem;
};

//----------------------------------------------------------------------------
template <class TKEY, class TVALUE>
THashTable<TKEY,TVALUE>::THashTable (int iTableSize)
{
    s_assert(iTableSize > 0);

    m_iTableSize = iTableSize;
    m_iQuantity = 0;
    m_iIndex = 0;
    m_pkItem = 0;
    m_apkTable = s_new HashItem*[m_iTableSize];
    memset(m_apkTable,0,m_iTableSize*sizeof(HashItem*));
    UserHashFunction = 0;
}
//----------------------------------------------------------------------------
template <class TKEY, class TVALUE>
THashTable<TKEY,TVALUE>::~THashTable ()
{
    RemoveAll();
    s_delete[] m_apkTable;
}
//----------------------------------------------------------------------------
template <class TKEY, class TVALUE>
int THashTable<TKEY,TVALUE>::GetQuantity () const
{
    return m_iQuantity;
}
//----------------------------------------------------------------------------
template <class TKEY, class TVALUE>
bool THashTable<TKEY,TVALUE>::Insert (const TKEY& rtKey,
    const TVALUE& rtValue)
{
    // find hash table entry for given key
    int iIndex = HashFunction(rtKey);
    HashItem* pkItem = m_apkTable[iIndex];

    // search for item in list associated with key
    while (pkItem)
    {
        if (rtKey == pkItem->m_tKey)
        {
            // item already in hash table
            return false;
        }
        pkItem = pkItem->m_pkNext;
    }

    // add item to beginning of list
    pkItem = s_new HashItem;
    pkItem->m_tKey = rtKey;
    pkItem->m_tValue = rtValue;
    pkItem->m_pkNext = m_apkTable[iIndex];
    m_apkTable[iIndex] = pkItem;
    m_iQuantity++;

    return true;
}
//----------------------------------------------------------------------------
template <class TKEY, class TVALUE>
TVALUE* THashTable<TKEY,TVALUE>::Find (const TKEY& rtKey) const
{
    // find hash table entry for given key
    int iIndex = HashFunction(rtKey);
    HashItem* pkItem = m_apkTable[iIndex];

    // search for item in list associated with key
    while (pkItem)
    {
        if (rtKey == pkItem->m_tKey)
        {
            // item is in hash table
            return &pkItem->m_tValue;
        }
        pkItem = pkItem->m_pkNext;
    }

    return 0;
}
//----------------------------------------------------------------------------
template <class TKEY, class TVALUE>
bool THashTable<TKEY,TVALUE>::Remove (const TKEY& rtKey)
{
    // find hash table entry for given key
    int iIndex = HashFunction(rtKey);
    HashItem* pkItem = m_apkTable[iIndex];

    if (!pkItem)
    {
        return false;
    }

    if (rtKey == pkItem->m_tKey)
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
    while (pkCurr && rtKey != pkCurr->m_tKey)
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
template <class TKEY, class TVALUE>
void THashTable<TKEY,TVALUE>::RemoveAll ()
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
template <class TKEY, class TVALUE>
TVALUE* THashTable<TKEY,TVALUE>::GetFirst (TKEY* ptKey) const
{
    if (m_iQuantity > 0)
    {
        for (m_iIndex = 0; m_iIndex < m_iTableSize; m_iIndex++)
        {
            if (m_apkTable[m_iIndex])
            {
                m_pkItem = m_apkTable[m_iIndex];
                *ptKey = m_pkItem->m_tKey;
                return &m_pkItem->m_tValue;
            }
        }
    }

    return 0;
}
//----------------------------------------------------------------------------
template <class TKEY, class TVALUE>
TVALUE* THashTable<TKEY,TVALUE>::GetNext (TKEY* ptKey) const
{
    if (m_iQuantity > 0)
    {
        m_pkItem = m_pkItem->m_pkNext;
        if (m_pkItem)
        {
            *ptKey = m_pkItem->m_tKey;
            return &m_pkItem->m_tValue;
        }
        
        for (m_iIndex++; m_iIndex < m_iTableSize; m_iIndex++)
        {
            if (m_apkTable[m_iIndex])
            {
                m_pkItem = m_apkTable[m_iIndex];
                *ptKey = m_pkItem->m_tKey;
                return &m_pkItem->m_tValue;
            }
        }
    }

    return 0;
}
//----------------------------------------------------------------------------
template <class TKEY, class TVALUE>
int THashTable<TKEY,TVALUE>::HashFunction (const TKEY& rtKey) const
{
    if (UserHashFunction)
    {
        return (*UserHashFunction)(rtKey);
    }

    // default hash function
    static double s_dHashMultiplier = 0.5*(sqrt(5.0)-1.0);
    unsigned int uiKey;
    System::Memcpy(&uiKey,sizeof(unsigned int),&rtKey,sizeof(unsigned int));
    uiKey %= m_iTableSize;
    double dFraction = fmod(s_dHashMultiplier*uiKey,1.0);
    return (int)floor(m_iTableSize*dFraction);
}
//----------------------------------------------------------------------------


}

#endif
