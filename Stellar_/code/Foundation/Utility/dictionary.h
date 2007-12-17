#pragma once
#ifndef UTIL_DICTIONARY_H
#define UTIL_DICTIONARY_H
//------------------------------------------------------------------------------
/**
	@class Util::Dictionary

	°ü×°ÁËstd::map
*/
#include "core/types.h"

//------------------------------------------------------------------------------
namespace Util
{
template<class KEYTYPE, class VALUETYPE>
class Dictionary //: public std::map<KEYTYPE, VALUETYPE>
{
public:
	/// default constructor
	Dictionary();
	/// copy constructor
	Dictionary(const Dictionary<KEYTYPE, VALUETYPE>& rhs);
	/// assignment operator
	void operator=(const Dictionary<KEYTYPE, VALUETYPE>& rhs);
	/// read/write [] operator
	VALUETYPE& operator[](const KEYTYPE& key);
	/// read-only [] operator
	const VALUETYPE& operator[](const KEYTYPE& key) const;
	/// return number of key/value pairs in the dictionary
	SizeT Size() const;
	/// clear the dictionary
	void Clear();
	/// return true if empty
	bool IsEmpty() const;
	/// add a key/value pair
	void Add(const std::map<KEYTYPE, VALUETYPE>& kvp);
	/// add a key and associated value
	void Add(const KEYTYPE& key, const VALUETYPE& value);
	/// erase a key and its associated value
	void Erase(const KEYTYPE& key);
	/// erase a key at index
	void EraseAtIndex(IndexT index);
	/// find index of key/value pair (InvalidIndex if doesn't exist)
	IndexT FindIndex(const KEYTYPE& key) const;
	/// return true if key exists in the array
	bool Contains(const KEYTYPE& key) const;
	/// get a key at given index
	const KEYTYPE& KeyAtIndex(IndexT index) const;
	/// access to value at given index
	VALUETYPE& ValueAtIndex(IndexT index);
	/// get a value at given index
	const VALUETYPE& ValueAtIndex(IndexT index) const;
	/// get key/value pair at index
	std::map<KEYTYPE, VALUETYPE>& KeyValuePairAtIndex(IndexT index) const;
	/// get all keys as array (slow)
	Array<KEYTYPE> KeysAsArray() const;
	/// get all values as array (slow)
	Array<VALUETYPE> ValuesAsArray() const;

protected:
	std::map<KEYTYPE, VALUETYPE> keyValuePairs;
	typedef std::map<KEYTYPE, VALUETYPE> itr_type;
};

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE>
Dictionary<KEYTYPE, VALUETYPE>::Dictionary()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE>
Dictionary<KEYTYPE, VALUETYPE>::Dictionary(const Dictionary<KEYTYPE, VALUETYPE>& rhs) :
keyValuePairs(rhs.keyValuePairs),
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE> void
Dictionary<KEYTYPE, VALUETYPE>::operator=(const Dictionary<KEYTYPE, VALUETYPE>& rhs)
{
	this->keyValuePairs = rhs.keyValuePairs;
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE> void
Dictionary<KEYTYPE, VALUETYPE>::Clear()
{
	this->keyValuePairs.Clear();
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE> SizeT
Dictionary<KEYTYPE, VALUETYPE>::Size() const
{
	return (SizeT)this->keyValuePairs.size();
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE> bool
Dictionary<KEYTYPE, VALUETYPE>::IsEmpty() const
{
	return (0 == this->keyValuePairs.size());
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE> void
Dictionary<KEYTYPE, VALUETYPE>::Add(const std::map<KEYTYPE, VALUETYPE>& kvp)
{
	this->keyValuePairs.insert(kvp);
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE> void
Dictionary<KEYTYPE, VALUETYPE>::Add(const KEYTYPE& key, const VALUETYPE& value)
{
	this->keyValuePairs.insert(std::map<KEYTYPE, VALUETYPE>::value_type(key, value));
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE> void
Dictionary<KEYTYPE, VALUETYPE>::Erase(const KEYTYPE& key)
{
	itr_type::iterator itr = this->keyValuePairs.find(key);
	s_assert(itr != this->keyValuePairs.end());
	this->keyValuePairs.erase(itr);
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE> void
Dictionary<KEYTYPE, VALUETYPE>::EraseAtIndex(IndexT index)
{
	
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE> IndexT
Dictionary<KEYTYPE, VALUETYPE>::FindIndex(const KEYTYPE& key) const
{
	//this->SortIfDirty();
	//return this->keyValuePairs.BinarySearchIndex(key);
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE> bool
Dictionary<KEYTYPE, VALUETYPE>::Contains(const KEYTYPE& key) const
{
	itr_type::const_iterator itr = this->keyValuePairs.find(key);
	return (itr != this->keyValuePairs.end());
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE> const KEYTYPE&
Dictionary<KEYTYPE, VALUETYPE>::KeyAtIndex(IndexT index) const
{
	//return this->keyValuePairs[index];
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE> VALUETYPE&
Dictionary<KEYTYPE, VALUETYPE>::ValueAtIndex(IndexT index)
{
	//this->SortIfDirty();
	//return this->keyValuePairs[index].Value();
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE> const VALUETYPE&
Dictionary<KEYTYPE, VALUETYPE>::ValueAtIndex(IndexT index) const
{
	//this->SortIfDirty();
	//return this->keyValuePairs[index].Value();
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE> std::map<KEYTYPE, VALUETYPE>&
Dictionary<KEYTYPE, VALUETYPE>::KeyValuePairAtIndex(IndexT index) const
{
	//this->SortIfDirty();
	//return this->keyValuePairs[index];
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE> VALUETYPE&
Dictionary<KEYTYPE, VALUETYPE>::operator[](const KEYTYPE& key)
{
	s_assert(this->Contains(key));
	return this->keyValuePairs[key];
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE> const VALUETYPE&
Dictionary<KEYTYPE, VALUETYPE>::operator[](const KEYTYPE& key) const
{
	itr_type::const_iterator itr = this->keyValuePairs.find(key);
	s_assert(itr != this->keyValuePairs.end());
	return itr->second;
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE> Array<VALUETYPE>
Dictionary<KEYTYPE, VALUETYPE>::ValuesAsArray() const
{
	Array<VALUETYPE> result;
	itr_type::iterator itr = this->keyValuePairs.begin();
	for (; itr != this->keyValuePairs.end(); itr++)
	{
		result.push_back(itr->second);
	}
	return result;
}

//------------------------------------------------------------------------------
/**
*/
template<class KEYTYPE, class VALUETYPE> Array<KEYTYPE>
Dictionary<KEYTYPE, VALUETYPE>::KeysAsArray() const
{
	Array<VALUETYPE> result;
	itr_type::iterator itr = this->keyValuePairs.begin();
	for (; itr != this->keyValuePairs.end(); itr++)
	{
		result.push_back(itr->first);
	}
	return result;
}
}
#endif