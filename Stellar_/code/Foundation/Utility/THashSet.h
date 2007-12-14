
#ifndef UTIL_THASHSET_H
#define UTIL_THASHSET_H

// The class TKEY is either native data or is class data that has the
// following member functions:
//   TKEY::TKEY ()
//   TKEY& TKEY::operator= (const TKEY&)
//   bool TKEY::operator== (const TKEY&) const
//   bool TKEY::operator!= (const TKEY&) const
//   TKEY::operator unsigned int () const
// The implicit conversion to unsigned int is used to select a hash table
// index for the T object.  The return value need not be within the range of
// hash table indices.  THashSet will use modular arithmetic to make this
// happen.

namespace Util
{

template <class TKEY>
class THashSet
{
public:
    // construction and destruction
    THashSet (int iTableSize);
    ~THashSet ();

    // element access
    int GetQuantity () const;

    // A pointer to the actual storage is returned so that the caller has
    // direct access to it.  This allows a subset of TKEY members to be used
    // in key comparison.
    TKEY* Insert (const TKEY& rtKey);

    // If the input key exists, a pointer to the actual storage is returned.
    // This allows a subset of TKEY members to be used in key comparison,
    // but gives the caller a chance to modify other TKEY members.
    TKEY* Get (const TKEY& rtKey) const;

    bool Remove (const TKEY& rtKey);
    void RemoveAll ();

    // linear traversal of map
    TKEY* GetFirst () const;
    TKEY* GetNext () const;

    // user-specified key-to-index construction
    int (*UserHashFunction)(const TKEY&);

private:
    class HashItem
    {
    public:
        TKEY m_tKey;
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

template <class TKEY>
THashSet<TKEY>::THashSet (int iTableSize)
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
template <class TKEY>
THashSet<TKEY>::~THashSet ()
{
    RemoveAll();
    s_delete[] m_apkTable;
}
//----------------------------------------------------------------------------
template <class TKEY>
int THashSet<TKEY>::GetQuantity () const
{
    return m_iQuantity;
}
//----------------------------------------------------------------------------
template <class TKEY>
TKEY* THashSet<TKEY>::Insert (const TKEY& rtKey)
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
            return &pkItem->m_tKey;
        }
        pkItem = pkItem->m_pkNext;
    }

    // add item to beginning of list
    pkItem = s_new HashItem;
    pkItem->m_tKey = rtKey;
    pkItem->m_pkNext = m_apkTable[iIndex];
    m_apkTable[iIndex] = pkItem;
    m_iQuantity++;

    return &pkItem->m_tKey;
}
//----------------------------------------------------------------------------
template <class TKEY>
TKEY* THashSet<TKEY>::Get (const TKEY& rtKey) const
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
            return &pkItem->m_tKey;
        }
        pkItem = pkItem->m_pkNext;
    }

    return 0;
}
//----------------------------------------------------------------------------
template <class TKEY>
bool THashSet<TKEY>::Remove (const TKEY& rtKey)
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
template <class TKEY>
void THashSet<TKEY>::RemoveAll ()
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
template <class TKEY>
TKEY* THashSet<TKEY>::GetFirst () const
{
    if (m_iQuantity > 0)
    {
        for (m_iIndex = 0; m_iIndex < m_iTableSize; m_iIndex++)
        {
            if (m_apkTable[m_iIndex])
            {
                m_pkItem = m_apkTable[m_iIndex];
                return &m_pkItem->m_tKey;
            }
        }
    }

    return 0;
}
//----------------------------------------------------------------------------
template <class TKEY>
TKEY* THashSet<TKEY>::GetNext () const
{
    if (m_iQuantity > 0)
    {
        m_pkItem = m_pkItem->m_pkNext;
        if (m_pkItem)
        {
            return &m_pkItem->m_tKey;
        }
        
        for (m_iIndex++; m_iIndex < m_iTableSize; m_iIndex++)
        {
            if (m_apkTable[m_iIndex])
            {
                m_pkItem = m_apkTable[m_iIndex];
                return &m_pkItem->m_tKey;
            }
        }
    }

    return 0;
}
//----------------------------------------------------------------------------
template <class TKEY>
int THashSet<TKEY>::HashFunction (const TKEY& rtKey) const
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
