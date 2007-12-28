//------------------------------------------------------------------------------
//  filestream.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "io/filestream.h"

namespace IO
{
ImplementClass(IO::FileStream, 'FSTR', IO::Stream);

using namespace Util;
using namespace Core;

//------------------------------------------------------------------------------
/**
*/
FileStream::FileStream() :
    handle(0),
    mappedContent(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
FileStream::~FileStream()
{
    if (this->IsOpen())
    {
        this->Close();
    }
}

//------------------------------------------------------------------------------
/**
*/
bool
FileStream::CanRead() const
{
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
FileStream::CanWrite() const
{
    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
FileStream::CanSeek() const
{
    return true;
}

//------------------------------------------------------------------------------
/**
    FileStreams support mapping (only read access for now).
*/
bool
FileStream::CanBeMapped() const
{
    return true;
}

//------------------------------------------------------------------------------
/**
*/
Stream::Size
FileStream::GetSize() const
{
    s_assert(0 != this->handle);
    return Internal::FSWrapper::GetFileSize(this->handle);
}

//------------------------------------------------------------------------------
/**
*/
Stream::Position
FileStream::GetPosition() const
{
    s_assert(0 != this->handle);
    return Internal::FSWrapper::Tell(this->handle);
}

//------------------------------------------------------------------------------
/**
*/
bool
FileStream::Open()
{
    s_assert(!this->IsOpen());
    s_assert(0 == this->handle);
    //s_assert(this->uri.Scheme() == "file");
    //s_assert(this->uri.LocalPath().IsValid());
	s_assert(!this->path.empty());
    if (Stream::Open())
    {
        this->handle = Internal::FSWrapper::OpenFile(this->path, this->accessMode, this->accessPattern);
        if (0 != this->handle)
        {
            return true;
        }
    }

    // fallthrough: failure
    Stream::Close();
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void
FileStream::Close()
{
    s_assert(this->IsOpen());
    s_assert(0 != this->handle);
    if (this->IsMapped())
    {
        this->Unmap();
    }
    Internal::FSWrapper::CloseFile(this->handle);
    this->handle = 0;
    Stream::Close();
}

//------------------------------------------------------------------------------
/**
*/
void
FileStream::Write(const void* ptr, Size numBytes)
{
    s_assert(!this->IsMapped());
    if (numBytes > 0)
    {
        s_assert(this->IsOpen());
        s_assert(0 != this->handle);
        s_assert(0 != ptr);
        Internal::FSWrapper::Write(this->handle, ptr, numBytes);
    }
}

//------------------------------------------------------------------------------
/**
*/
Stream::Size
FileStream::Read(void* ptr, Size numBytes)
{
    s_assert(!this->IsMapped());
    s_assert(this->IsOpen());
    s_assert(0 != this->handle);
    s_assert(0 != ptr);
    s_assert(numBytes > 0);
    return Internal::FSWrapper::Read(this->handle, ptr, numBytes);
}

//------------------------------------------------------------------------------
/**
*/
void
FileStream::Seek(Offset offset, SeekOrigin origin)
{
    s_assert(!this->IsMapped());
    s_assert(this->IsOpen());
    s_assert(0 != this->handle);
    Internal::FSWrapper::Seek(this->handle, offset, origin);
}

//------------------------------------------------------------------------------
/**
*/
void
FileStream::Flush()
{
    s_assert(!this->IsMapped());
    s_assert(this->IsOpen());
    s_assert(0 != this->handle);
    Internal::FSWrapper::Flush(this->handle);
}

//------------------------------------------------------------------------------
/**
*/
bool
FileStream::Eof() const
{
    s_assert(!this->IsMapped());
    s_assert(this->IsOpen());
    s_assert(0 != this->handle);
    return Internal::FSWrapper::Eof(this->handle);
}

//------------------------------------------------------------------------------
/**
*/
void*
FileStream::Map()
{
    s_assert(0 == this->mappedContent);
    
    Size size = this->GetSize();
    s_assert(size > 0);
    this->mappedContent = Memory::Alloc(size);
    this->Seek(0, Begin);
    Size readSize = this->Read(this->mappedContent, size);
    s_assert(readSize == size);
    Stream::Map();
    return this->mappedContent;
}

//------------------------------------------------------------------------------
/**
*/
void
FileStream::Unmap()
{
    s_assert(0 != this->mappedContent);
    Stream::Unmap();
    Memory::Free(this->mappedContent);
    this->mappedContent = 0;
}

} // namespace IO
