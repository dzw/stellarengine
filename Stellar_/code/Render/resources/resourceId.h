#pragma once
#ifndef RESOURCES_RESOURCEID_H
#define RESOURCES_RESOURCEID_H
//------------------------------------------------------------------------------
/**
    @class Resources::ResourceId

	×ÊÔ´Id

    (C) 2007 by ctuo
*/
#include "core/refcounted.h"
#include "Utility/string.h"
#include "Utility/atom.h"
//#include "Utility/crc.h"

//------------------------------------------------------------------------------
namespace Resources
{
	typedef Util::Atom<Util::String> ResourceId;
////	typedef Util::String ResourceId;
//class ResourceId //: public Core::RefCounted
//{
//	//DeclareClass(ResourceId);
//public:
//	/// constructor
//	ResourceId();
//	/// constructor from string
//	ResourceId(Util::String& resName);
//
//protected:
//	void ConvToCrc(Util::String& res);
//
//	static std::set<uint> atomTable;
//	uint Id;
//};
//
//std::set<uint> ResourceId::atomTable;
//
////------------------------------------------------------------------------------
///**
//*/
//inline
//ResourceId::ResourceId() :
//Id(0)
//{
//}
//
////------------------------------------------------------------------------------
///**
//*/
//inline
//ResourceId::ResourceId(Util::String& resName)
//{
//	ConvToCrc(resName);
//}
//
////------------------------------------------------------------------------------
///**
//*/
//inline void 
//ResourceId::ConvToCrc(Util::String& res)
//{
//	Util::Crc cc;
//	cc.Begin();
//	cc.Compute((unsigned char*)res.c_str(), (uint)res.size());
//	cc.End();
//
//	this->Id = cc.GetResult();
//	atomTable.insert(this->Id);
//}

} // namespace Resources
//------------------------------------------------------------------------------
#endif
